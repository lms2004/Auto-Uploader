#include "AuthenticationService.h"

// 构造函数，负责从数据库加载用户数据
AuthenticationService::AuthenticationService()
{
    checkAndCreateDatabase();
    loadUsersFromDB();
}

// 析构函数,负责将用户数据保存到数据库中
AuthenticationService::~AuthenticationService()
{
    saveUsersToDB();
}

// 检查并创建数据库和表
void AuthenticationService::checkAndCreateDatabase()
{
    MYSQL* mysql = mysql_init(nullptr);
    if (mysql == nullptr) {
        printf("mysql_init() 失败\n");
        return;
    }

    if (mysql_real_connect(mysql, "127.0.0.1", "root", "123456", nullptr, 3306, nullptr, 0) == nullptr) {
        printf("连接数据库失败: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return;
    }

    // 创建数据库
    if (mysql_query(mysql, "CREATE DATABASE IF NOT EXISTS data;") != 0) {
        printf("创建数据库失败: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return;
    }

    // 连接到创建的数据库
    if (mysql_select_db(mysql, "data") != 0) {
        printf("选择数据库失败: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return;
    }

    // 创建表
    const char* createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS users (
            username VARCHAR(20) NOT NULL,
            hashed_password VARCHAR(128) NOT NULL,
            salt VARCHAR(100) NOT NULL,
            PRIMARY KEY (username)
        );
    )";
    if (mysql_query(mysql, createTableQuery) != 0) {
        printf("创建表失败: %s\n", mysql_error(mysql));
    }

    mysql_close(mysql);
}

// 从数据库加载用户数据到哈希表
void AuthenticationService::loadUsersFromDB()
{
    MYSQL* mysql = connectDB();
    if (!mysql) return;

    if (mysql_query(mysql, "SELECT username, hashed_password, salt FROM users;") != 0) {
        printf("查询数据库失败: %s\n", mysql_error(mysql));
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

// 将用户数据保存到数据库中
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

// 用户登录
bool AuthenticationService::login(const std::string& username, const std::string& password)
{
    auto it = users.find(username);
    if (it == users.end()) {
        std::cout << "用户不存在\n";
        return false;
    }

    std::string hashedPasswordWithSalt = it->second;
    size_t delimiterPos = hashedPasswordWithSalt.find(':');
    if (delimiterPos == std::string::npos) {
        std::cout << "无效的哈希格式\n";
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


// 注册新用户
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
            printf("插入数据失败: %s\n", mysql_error(mysql));
            mysql_close(mysql);
            return false;
        }

        mysql_close(mysql);
        return true;
    }
    return false;
}

// 哈希算法加密
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


// 数据库连接
MYSQL* AuthenticationService::connectDB()
{
    MYSQL* mysql = mysql_init(nullptr);
    if (mysql == nullptr) {
        printf("mysql_init() 失败\n");
        return nullptr;
    }

    if (mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "data", 3306, nullptr, 0) == nullptr) {
        printf("连接数据库失败: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return nullptr;
    }

    return mysql;
}
