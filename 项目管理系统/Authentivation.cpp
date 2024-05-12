#include "Authentication.h"

unsigned char salt[50] = "Gk4^r8@qF0o!Z3Vb$2y$10$V5F9zgYjZLZ1%sZ3Zq5u8Nw8e";

AuthenticationService::AuthenticationService()
{
    MYSQL mysql;
    mysql_init(&mysql);
    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
        printf("连接数据库失败: %s\\n", mysql_error(&mysql));
        exit(-1);
    }

    if (mysql_query(&mysql, "SELECT username, hashed_password, salt FROM users;") != 0) {
        printf("查询数据库失败: %s\\n", mysql_error(&mysql));
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

AuthenticationService::~AuthenticationService()
{
}


bool AuthenticationService::login(const std::string& username, const std::string& password) {
    auto it = users.find(username);
    if (it == users.end()) {
        std::cout << "用户名不存在。" << std::endl;
        return false;
    }

    //从数据库检索哈希密码
    std::string hashedPasswordWithSalt = it->second;
    size_t delimiterPos = hashedPasswordWithSalt.find(':');
    std::string storedHash = hashedPasswordWithSalt.substr(0, delimiterPos);

    //unsigned char salt[17] = "Gk4^r8@qF0o!Z3Vb";
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
        sprintf_s(buf, "%02x", hash[i]);
        hashedPassword += buf;
    }

    return hashedPassword == storedHash;
}

bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
{
    if (users.find(username) != users.end())
        return false;

    std::string hashedPassword = hashPassword(password);

    MYSQL mysql;
    mysql_init(&mysql);
    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
        printf("连接数据库失败: %s\\\\n", mysql_error(&mysql));
        exit(-1);
    }

    std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
    std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));

    std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
    if (mysql_query(&mysql, query.c_str()) == 0) {
        users[username] = hashedPassword;
        mysql_close(&mysql);
        return true;
    }
    else {
        printf("插入数据失败: %s\\\\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return false;
    }
}

std::string AuthenticationService::hashPassword(const std::string& password)
{

    //unsigned char salt[80] = "Gk4^r8@qF0o!Z3Vb$2y$10$V5F9zgYjZLZ1XxqzZ7jJbe47YKjJFJ1YD7k8v1sZ3Zq5u8Nw8eTz7O";

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
        sprintf_s(buf, "%02x", hash[i]);
        hashedPassword += buf;
    }

    hashedPassword += ":";
    for (int i = 0; i < sizeof(salt); i++) {
        char buf[3];
        sprintf_s(buf, "%02x", salt[i]);
        hashedPassword += buf;
    }

    return hashedPassword;
}