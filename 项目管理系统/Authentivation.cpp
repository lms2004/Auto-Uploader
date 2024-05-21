#include "AuthenticationService.h"

// ���캯������������ݿ�����û�����
AuthenticationService::AuthenticationService()
{
    checkAndCreateDatabase();
    loadUsersFromDB();
}

// ��������,�����û����ݱ��浽���ݿ���
AuthenticationService::~AuthenticationService()
{
    saveUsersToDB();
}

// ��鲢�������ݿ�ͱ�
void AuthenticationService::checkAndCreateDatabase()
{
    MYSQL* mysql = mysql_init(nullptr);
    if (mysql == nullptr) {
        printf("mysql_init() ʧ��\n");
        return;
    }

    if (mysql_real_connect(mysql, "127.0.0.1", "root", "123456", nullptr, 3306, nullptr, 0) == nullptr) {
        printf("�������ݿ�ʧ��: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return;
    }

    // �������ݿ�
    if (mysql_query(mysql, "CREATE DATABASE IF NOT EXISTS data;") != 0) {
        printf("�������ݿ�ʧ��: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return;
    }

    // ���ӵ����������ݿ�
    if (mysql_select_db(mysql, "data") != 0) {
        printf("ѡ�����ݿ�ʧ��: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return;
    }

    // ������
    const char* createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS users (
            username VARCHAR(20) NOT NULL,
            hashed_password VARCHAR(128) NOT NULL,
            salt VARCHAR(100) NOT NULL,
            PRIMARY KEY (username)
        );
    )";
    if (mysql_query(mysql, createTableQuery) != 0) {
        printf("������ʧ��: %s\n", mysql_error(mysql));
    }

    mysql_close(mysql);
}

// �����ݿ�����û����ݵ���ϣ��
void AuthenticationService::loadUsersFromDB()
{
    MYSQL* mysql = connectDB();
    if (!mysql) return;

    if (mysql_query(mysql, "SELECT username, hashed_password, salt FROM users;") != 0) {
        printf("��ѯ���ݿ�ʧ��: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return;
    }

    MYSQL_RES* res = mysql_store_result(mysql);
    if (res) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            users[row[0]] = std::string(row[1]) + ":" + std::string(row[2]);
        }
        mysql_free_result(res);
    }

    mysql_close(mysql);
}

// ���û����ݱ��浽���ݿ���
void AuthenticationService::saveUsersToDB()
{
    MYSQL* mysql = connectDB();
    if (!mysql) return;

    mysql_query(mysql, "DELETE FROM users;");

    for (const auto& pair : users) {
        size_t delimiterPos = pair.second.find(':');
        std::string hashedPassword = pair.second.substr(0, delimiterPos);
        std::string salt = pair.second.substr(delimiterPos + 1);
        std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + pair.first + "', '" + hashedPassword + "', '" + salt + "');";
        mysql_query(mysql, query.c_str());
    }

    mysql_close(mysql);
}

// �û���¼
bool AuthenticationService::login(const std::string& username, const std::string& password)
{
    auto it = users.find(username);
    if (it == users.end()) {
        std::cout << "�û�������\n";
        return false;
    }

    std::string hashedPasswordWithSalt = it->second;
    size_t delimiterPos = hashedPasswordWithSalt.find(':');
    if (delimiterPos == std::string::npos) {
        std::cout << "��Ч�Ĺ�ϣ��ʽ\n";
        return false;
    }

    std::string hashedPassword = hashedPasswordWithSalt.substr(0, delimiterPos);
    std::string saltString = hashedPasswordWithSalt.substr(delimiterPos + 1);

    unsigned char salt[SHA256_DIGEST_LENGTH];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        std::string byteString = saltString.substr(i * 2, 2);
        salt[i] = (unsigned char)strtol(byteString.c_str(), nullptr, 16);
    }

    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, EVP_sha256());
    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
    EVP_DigestUpdate(ctx, salt, sizeof(salt));
    EVP_DigestFinal(ctx, hash, nullptr);
    EVP_MD_CTX_free(ctx);

    std::string computedHashedPassword;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        char buf[3];
        sprintf(buf, "%02x", hash[i]);
        computedHashedPassword += buf;
    }

    std::cout << "computedHashedPassword: " << computedHashedPassword << "\n";
    std::cout << "stored hashedPassword: " << hashedPassword << "\n";

    return computedHashedPassword == hashedPassword;
}


// ע�����û�
bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
{
    if (users.find(username) == users.end()) {
        std::string hashedPassword = hashPassword(password);
        users[username] = hashedPassword;

        MYSQL* mysql = connectDB();
        if (!mysql) return false;

        size_t delimiterPos = hashedPassword.find(':');
        std::string hashed = hashedPassword.substr(0, delimiterPos);
        std::string salt = hashedPassword.substr(delimiterPos + 1);
        std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashed + "', '" + salt + "');";
        if (mysql_query(mysql, query.c_str()) != 0) {
            printf("��������ʧ��: %s\n", mysql_error(mysql));
            mysql_close(mysql);
            return false;
        }

        mysql_close(mysql);
        return true;
    }
    return false;
}

// ��ϣ�㷨����
std::string AuthenticationService::hashPassword(const std::string& password)
{
    unsigned char salt[SHA256_DIGEST_LENGTH];
    RAND_bytes(salt, sizeof(salt));

    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, EVP_sha256());
    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
    EVP_DigestUpdate(ctx, salt, sizeof(salt));
    EVP_DigestFinal(ctx, hash, nullptr);
    EVP_MD_CTX_free(ctx);

    std::string hashedPassword;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        char buf[3];
        sprintf(buf, "%02x", hash[i]);
        hashedPassword += buf;
    }

    std::string saltString;
    for (int i = 0; i < sizeof(salt); i++) {
        char buf[3];
        sprintf(buf, "%02x", salt[i]);
        saltString += buf;
    }

    std::string hashedPasswordWithSalt = hashedPassword + ":" + saltString;

    std::cout << "hashedPassword: " << hashedPasswordWithSalt << "\n";

    return hashedPasswordWithSalt;
}


// ���ݿ�����
MYSQL* AuthenticationService::connectDB()
{
    MYSQL* mysql = mysql_init(nullptr);
    if (mysql == nullptr) {
        printf("mysql_init() ʧ��\n");
        return nullptr;
    }

    if (mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "data", 3306, nullptr, 0) == nullptr) {
        printf("�������ݿ�ʧ��: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return nullptr;
    }

    return mysql;
}
