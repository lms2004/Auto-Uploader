
//#include "Authentication.h"

//// ���캯��������Ӵ洢�м����û�����
//AuthenticationService::AuthenticationService()
//{
//    // ���ļ������û����ݵ������
//    std::ifstream file("userdata.txt");
//    if (file.is_open())
//    {
//        std::string username;
//        std::string password;
//        while (file >> username >> password)
//        {
//            users[username] = password;
//        }
//        file.close();
//    }
//}
//
//// ��������,�����û����ݱ��浽�洢��
//AuthenticationService::~AuthenticationService()
//{
//    // ��������е��û����ݱ��浽�ļ���
//    std::ofstream file("userdata.txt");
//    if (file.is_open())
//    {
//        for (const auto& user : users)
//        {
//            file << user.first << " " << user.second << std::endl;
//        }
//        file.close();
//    }
//}
//
//// ������֤�û���¼
//bool AuthenticationService::login(const std::string& username, const std::string& password)
//{
//    auto it = users.find(username);
//    if (it == users.end())
//    {
//        // �û������ڣ�ֱ�ӷ��� false
//        return false;
//    }
//
//    // �û����ڣ��������
//    std::string hashedPassword = it->second;
//    std::vector<unsigned char> salt(SHA256_DIGEST_LENGTH);
//    std::string saltStr;
//    std::istringstream iss(hashedPassword);
//    std::getline(iss, saltStr, ':');
//    for (int i = 0; i < saltStr.length() / 2; i++)
//    {
//        std::string byteStr = saltStr.substr(2 * i, 2);
//        unsigned char byte = (unsigned char)strtol(byteStr.c_str(), nullptr, 16);
//        salt[i] = byte;
//    }
//
//    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
//    EVP_DigestInit(ctx, EVP_sha256());
//    EVP_DigestUpdate(ctx, password.c_str(), password.length());
//    EVP_DigestUpdate(ctx, salt.data(), salt.size());
//    unsigned char hash[SHA256_DIGEST_LENGTH];
//    EVP_DigestFinal(ctx, hash, nullptr);
//    EVP_MD_CTX_free(ctx);
//
//    std::string hashedInputPassword;
//    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
//    {
//        char buf[3];
//        sprintf_s(buf, "%02x", hash[i]);
//        hashedInputPassword += buf;
//    }
//    return hashedInputPassword == saltStr.substr(saltStr.find(':') + 1);
//}
//
//// ����ע�����û�
//bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
//{
//    if (users.find(username) == users.end())
//    {
//        std::string hashedPassword = hashPassword(password);
//        users[username] = hashedPassword;
//        saveUsersToFile();
//        return true;
//    }
//    return false;
//}
//
//// ����Ϊ userdata.txt ���ļ��м����û���Ϣ���ڴ��е� users ��ϣ����
//void AuthenticationService::loadUsersFromFile()
//{
//    std::ifstream file("userdata.txt");
//    std::string line;
//    while (std::getline(file, line))
//    {
//        std::istringstream iss(line);
//        std::string username;
//        std::string hashedPassword;
//        if (std::getline(iss, username, ':') && std::getline(iss, hashedPassword))
//        {
//            users[username] = hashedPassword;
//        }
//    }
//}
//
//// ���ڴ��е��û���Ϣ���浽��Ϊ userdata.txt ���ļ���
//void AuthenticationService::saveUsersToFile()
//{
//    std::ofstream file("userdata.txt");
//    for (const auto& pair : users)
//    {
//        file << pair.first << ":" << pair.second << std::endl;
//    }
//}
//
//// ��ϣ�㷨����
//std::string AuthenticationService::hashPassword(const std::string& password)
//{
//    unsigned char salt[SHA256_DIGEST_LENGTH];
//    RAND_bytes(salt, sizeof(salt));
//
//    unsigned char hash[SHA256_DIGEST_LENGTH];
//    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
//    EVP_DigestInit(ctx, EVP_sha256());
//    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
//    EVP_DigestUpdate(ctx, salt, sizeof(salt));
//    EVP_DigestFinal(ctx, hash, nullptr);
//    EVP_MD_CTX_free(ctx);
//
//    std::string hashedPassword;
//    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
//    {
//        char buf[3];
//        sprintf_s(buf, "%02x", hash[i]);
//        hashedPassword += buf;
//    }
//
//    hashedPassword += ":";
//    for (int i = 0; i < sizeof(salt); i++)
//    {
//        char buf[3];
//        sprintf_s(buf, "%02x", salt[i]);
//        hashedPassword += buf;
//    }
//
//    return hashedPassword;
//}

#include "Authentication.h"

// ���캯������������ݿ�����û�����
AuthenticationService::AuthenticationService()
{
    // �����ݿ�����û����ݵ���ϣ��
    MYSQL mysql;
    mysql_init(&mysql);
    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
        printf("�������ݿ�ʧ��: %s\\n", mysql_error(&mysql));
        exit(-1);
    }

    if (mysql_query(&mysql, "SELECT username, hashed_password, salt FROM users;") != 0) {
        printf("��ѯ���ݿ�ʧ��: %s\\n", mysql_error(&mysql));
        exit(-1);
    }

    MYSQL_RES* res = mysql_store_result(&mysql);
    if (res) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            users[row[0]] = std::string(row[1]) + ":" + std::string(row[2]);
        }
        mysql_free_result(res);
    }

    mysql_close(&mysql);
}

// ��������,�����û����ݱ��浽�洢��
AuthenticationService::~AuthenticationService()
{
    // ��������е��û����ݱ��浽�ļ���
    std::ofstream file("userdata.txt");
    if (file.is_open())
    {
        for (const auto& user : users)
        {
            file << user.first << " " << user.second << std::endl;
        }
        file.close();
    }
}

// ������֤�û���¼
bool AuthenticationService::login(const std::string& username, const std::string& password)
{
    auto it = users.find(username);
    if (it == users.end())
    {
        return false;
    }

    std::string hashedPasswordWithSalt = it->second;
    size_t delimiterPos = hashedPasswordWithSalt.find(':');
    if (delimiterPos == std::string::npos)
    {
        return false; // ����Ĺ�ϣ��ʽ
    }

    std::string hashedPassword = hashedPasswordWithSalt.substr(0, delimiterPos);
    std::string salt = hashedPasswordWithSalt.substr(delimiterPos + 1);

    // ���¼����ϣֵ
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, EVP_sha256());
    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
    EVP_DigestUpdate(ctx, (const unsigned char*)salt.c_str(), salt.length());
    EVP_DigestFinal(ctx, hash, nullptr);
    EVP_MD_CTX_free(ctx);

    std::string computedHashedPassword;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        char buf[3];
        sprintf_s(buf, "%02x", hash[i]);
        computedHashedPassword += buf;
    }

    // �ȽϹ�ϣֵ
    return computedHashedPassword == hashedPassword;
}

// ����ע�����û�
bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
{
    if (users.find(username) == users.end())
    {
        std::string hashedPassword = hashPassword(password);
        users[username] = hashedPassword;

        // ���û���Ϣ�������ݿ�
        MYSQL mysql;
        mysql_init(&mysql);
        if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
            printf("�������ݿ�ʧ��: %s\\\\n", mysql_error(&mysql));
            exit(-1);
        }

        std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPassword + "');";
        if (mysql_query(&mysql, query.c_str()) != 0) {
            printf("��������ʧ��: %s\\\\n", mysql_error(&mysql));
            exit(-1);
        }

        mysql_close(&mysql);
        return true;
    }
    return false;
}

// ����Ϊ userdata.txt ���ļ��м����û���Ϣ���ڴ��е� users ��ϣ����
void AuthenticationService::loadUsersFromFile()
{
    std::ifstream file("userdata.txt");
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string username;
        std::string hashedPassword;
        if (std::getline(iss, username, ':') && std::getline(iss, hashedPassword))
        {
            users[username] = hashedPassword;
        }
    }
}

// ���ڴ��е��û���Ϣ���浽���ݿ�
void AuthenticationService::saveUsersToFile()
{
    MYSQL mysql;
    mysql_init(&mysql);
    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
        printf("�������ݿ�ʧ��: %s\\n", mysql_error(&mysql));
        exit(-1);
    }

    // ɾ��ԭ������
    mysql_query(&mysql, "DELETE FROM users;");

    // ����������
    for (const auto& pair : users)
    {
        std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + pair.first + "', '" + pair.second + "');";
        mysql_query(&mysql, query.c_str());
    }

    mysql_close(&mysql);
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
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        char buf[3];
        sprintf_s(buf, "%02x", hash[i]);
        hashedPassword += buf;
    }

    hashedPassword += ":";
    for (int i = 0; i < sizeof(salt); i++)
    {
        char buf[3];
        sprintf_s(buf, "%02x", salt[i]);
        hashedPassword += buf;
    }

    return hashedPassword;
}