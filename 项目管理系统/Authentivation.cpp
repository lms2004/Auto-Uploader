//#include "Authentication.h"
//
//// 构造函数，负责从数据库加载用户数据
//AuthenticationService::AuthenticationService()
//{
//    // 从数据库加载用户数据到哈希表
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("连接数据库失败: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    if (mysql_query(&mysql, "SELECT username, hashed_password, salt FROM users;") != 0) {
//        printf("查询数据库失败: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    MYSQL_RES* res = mysql_store_result(&mysql);
//    if (res) {
//        MYSQL_ROW row;
//        while ((row = mysql_fetch_row(res))) {
//            users[row[0]] = std::string(row[1]) + ":" + std::string(row[2]);
//        }
//        mysql_free_result(res);
//    }
//
//    mysql_close(&mysql);
//}
//
//// 析构函数,负责将用户数据保存到存储中
//AuthenticationService::~AuthenticationService()
//{
//    // 将无序表中的用户数据保存到文件中
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
//// 用于验证用户登录
//bool AuthenticationService::login(const std::string& username, const std::string& password)
//{
//    auto it = users.find(username);
//    if (it == users.end())
//    {
//        return false;
//    }
//
//    std::string hashedPasswordWithSalt = it->second;
//    size_t delimiterPos = hashedPasswordWithSalt.find(':');
//    if (delimiterPos == std::string::npos)
//    {
//        return false; // 错误的哈希格式
//    }
//
//    std::string hashedPassword = hashedPasswordWithSalt.substr(0, delimiterPos);
//    std::string salt = hashedPasswordWithSalt.substr(delimiterPos + 1);
//
//    // 重新计算哈希值
//    unsigned char hash[SHA256_DIGEST_LENGTH];
//    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
//    EVP_DigestInit(ctx, EVP_sha256());
//    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
//    EVP_DigestUpdate(ctx, (const unsigned char*)salt.c_str(), salt.length());
//    EVP_DigestFinal(ctx, hash, nullptr);
//    EVP_MD_CTX_free(ctx);
//
//    std::string computedHashedPassword;
//    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
//    {
//        char buf[3];
//        sprintf_s(buf, "%02x", hash[i]);
//        computedHashedPassword += buf;
//    }
//
//    // 比较哈希值
//    return computedHashedPassword == hashedPassword;
//}
//
//// 用于注册新用户
//bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
//{
//    if (users.find(username) == users.end())
//    {
//        std::string hashedPassword = hashPassword(password);
//
//        // 将用户信息插入数据库
//        MYSQL mysql;
//        mysql_init(&mysql);
//        if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//            printf("连接数据库失败: %s\\\\n", mysql_error(&mysql));
//            exit(-1);
//        }
//
//        std::string query = "SELECT username, hashed_password, salt FROM users WHERE username = '" + username + "';";
//        if (mysql_query(&mysql, query.c_str()) != 0) {
//            printf("查询数据库失败: %s\\\\n", mysql_error(&mysql));
//            exit(-1);
//        }
//
//        MYSQL_RES* res = mysql_store_result(&mysql);
//        if (res && mysql_num_rows(res) > 0) {
//            mysql_free_result(res);
//            mysql_close(&mysql);
//            return false; // 用户名已存在
//        }
//        mysql_free_result(res);
//
//        std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
//        std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));
//
//        query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
//        if (mysql_query(&mysql, query.c_str()) != 0) {
//            printf("插入数据失败: %s\\\\n", mysql_error(&mysql));
//            exit(-1);
//        }
//
//        mysql_close(&mysql);
//        return true;
//    }
//    return false;
//}
//
//// 从名为 userdata.txt 的文件中加载用户信息到内存中的 users 哈希表中
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
//// 将内存中的用户信息保存到数据库
//void AuthenticationService::saveUsersToFile()
//{
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("连接数据库失败: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    // 删除原有数据
//    mysql_query(&mysql, "DELETE FROM users;");
//
//    // 插入新数据
//    for (const auto& pair : users)
//    {
//        std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + pair.first + "', '" + pair.second + "');";
//        mysql_query(&mysql, query.c_str());
//    }
//
//    mysql_close(&mysql);
//}
//
//// 哈希算法加密
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


//// 用于注册新用户
//bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
//{
//    if (users.find(username) == users.end())
//    {
//        std::string hashedPassword = hashPassword(password);
//
//        // 将用户信息插入数据库
//        MYSQL mysql;
//        mysql_init(&mysql);
//        if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//            printf("连接数据库失败: %s\\\\n", mysql_error(&mysql));
//            exit(-1);
//        }
//
//        std::string query = "SELECT username, hashed_password, salt FROM users WHERE username = '" + username + "';";
//        if (mysql_query(&mysql, query.c_str()) != 0) {
//            printf("查询数据库失败: %s\\\\n", mysql_error(&mysql));
//            exit(-1);
//        }
//
//        MYSQL_RES* res = mysql_store_result(&mysql);
//        if (res && mysql_num_rows(res) > 0) {
//            mysql_free_result(res);
//            mysql_close(&mysql);
//            return false; // 用户名已存在
//        }
//        mysql_free_result(res);
//
//        std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
//        std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));
//
//        query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
//        if (mysql_query(&mysql, query.c_str()) != 0) {
//            printf("插入数据失败: %s\\\\n", mysql_error(&mysql));
//            exit(-1);
//        }
//
//        mysql_close(&mysql);
//        return true;
//    }
//    return false;
//}

//bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
//{
//    if (users.find(username) != users.end())
//    {
//        std::cout << "用户名已存在，注册失败。" << std::endl;
//        return false;
//    }
//
//    std::string hashedPassword = hashPassword(password);
//
//    // 直接尝试插入新用户，不再预先查询
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("连接数据库失败: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
//    std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));
//
//    std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
//    if (mysql_query(&mysql, query.c_str()) == 0) {
//        std::cout << "用户注册成功！" << std::endl;
//        users[username] = hashedPassword; // 注册成功后更新内存中的用户列表
//        mysql_close(&mysql);
//        return true;
//    }
//    else {
//        printf("插入数据失败: %s\\n", mysql_error(&mysql));
//        mysql_close(&mysql);
//        return false;
//    }
//}


// 用于验证用户登录
//bool AuthenticationService::login(const std::string& username, const std::string& password)
//{
//    auto it = users.find(username);
//    if (it == users.end())
//    {
//        std::cerr << "用户名不存在，请先注册。" << std::endl;
//        return false;
//    }
//
//    std::string hashedPasswordWithSalt = it->second;
//    size_t delimiterPos = hashedPasswordWithSalt.find(':');
//    if (delimiterPos == std::string::npos)
//    {
//        return false; // 错误的哈希格式
//    }
//
//    std::string hashedPassword = hashedPasswordWithSalt.substr(0, delimiterPos);
//    std::string salt = hashedPasswordWithSalt.substr(delimiterPos + 1);
//
//    // 重新计算哈希值
//    unsigned char hash[SHA256_DIGEST_LENGTH];
//    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
//    EVP_DigestInit(ctx, EVP_sha256());
//    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
//    EVP_DigestUpdate(ctx, (const unsigned char*)salt.c_str(), salt.length());
//    EVP_DigestFinal(ctx, hash, nullptr);
//    EVP_MD_CTX_free(ctx);
//
//    std::string computedHashedPassword;
//    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
//    {
//        char buf[3];
//        sprintf_s(buf, "%02x", hash[i]);
//        computedHashedPassword += buf;
//    }
//
//    // 比较哈希值
//    return computedHashedPassword == hashedPassword;
//}

//// Authentication.cpp
//#include "Authentication.h"
//
//// 构造函数，负责从数据库加载用户数据
//AuthenticationService::AuthenticationService()
//{
//    // 从数据库加载用户数据到哈希表
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("连接数据库失败: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    if (mysql_query(&mysql, "SELECT username, hashed_password, salt FROM users;") != 0) {
//        printf("查询数据库失败: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    MYSQL_RES* res = mysql_store_result(&mysql);
//    if (res) {
//        MYSQL_ROW row;
//        while ((row = mysql_fetch_row(res))) {
//            users[row[0]] = std::string(row[1]) + ":" + std::string(row[2]);
//        }
//        mysql_free_result(res);
//    }
//
//    mysql_close(&mysql);
//}
//
//// 析构函数,负责将用户数据保存到存储中
//AuthenticationService::~AuthenticationService()
//{
//    // 将无序表中的用户数据保存到文件中
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
////用于注册新用户
//bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
//{
//    if (users.find(username) != users.end())
//    {
//        return false; // 用户名已存在，无需打印信息，直接返回
//    }
//
//    std::string hashedPassword = hashPassword(password);
//
//    // 连接数据库并尝试插入
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("连接数据库失败: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
//    std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));
//
//    std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
//    if (mysql_query(&mysql, query.c_str()) == 0) {
//        //std::cout << "用户注册成功！" << std::endl; // 只在这里打印成功信息
//        users[username] = hashedPassword; // 更新内存中的用户列表
//        mysql_close(&mysql);
//        return true;
//    }
//    else {
//        printf("插入数据失败: %s\\n", mysql_error(&mysql));
//        mysql_close(&mysql);
//        return false;
//    }
//}
//
////用于验证用户登录
//bool AuthenticationService::login(const std::string& username, const std::string& password)
//{
//    auto it = users.find(username);
//    if (it == users.end())
//    {
//        std::cout << "用户名不存在。" << std::endl;
//        return false;
//    }
//
//    std::string hashedPasswordWithSalt = it->second;
//    size_t delimiterPos = hashedPasswordWithSalt.find(':');
//    if (delimiterPos == std::string::npos)
//    {
//        std::cout << "错误的哈希格式。" << std::endl;
//        return false;
//    }
//
//    std::string storedHash = hashedPasswordWithSalt.substr(0, delimiterPos);
//    std::string storedSalt = hashedPasswordWithSalt.substr(delimiterPos + 1);
//
//    // 计算输入密码的哈希值
//    unsigned char computedHash[SHA256_DIGEST_LENGTH];
//    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
//    EVP_DigestInit(ctx, EVP_sha256());
//    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
//    EVP_DigestUpdate(ctx, (const unsigned char*)storedSalt.c_str(), storedSalt.length());
//    EVP_DigestFinal(ctx, computedHash, nullptr);
//    EVP_MD_CTX_free(ctx);
//
//    std::string computedHashStr;
//    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
//        char buf[3];
//        sprintf_s(buf, "%02x", computedHash[i]);
//        computedHashStr += buf;
//    }
//
//    std::cout << "计算的哈希: " << computedHashStr << ", 存储的哈希: " << storedHash << std::endl;
//
//    // 比较哈希值
//    return computedHashStr == storedHash;
//}
//
////bool AuthenticationService::login(const std::string& username, const std::string& password)
////{
////    auto it = users.find(username);
////    if (it == users.end())
////    {
////        std::cout << "用户名不存在。" << std::endl;
////        return false;
////    }
////
////    std::string storedHashedPasswordWithSalt = it->second;
////    size_t delimiterPos = storedHashedPasswordWithSalt.find(':');
////    if (delimiterPos == std::string::npos)
////    {
////        std::cout << "错误的哈希格式。" << std::endl;
////        return false;
////    }
////
////    std::string storedHash = storedHashedPasswordWithSalt.substr(0, delimiterPos);
////    std::string storedSalt = storedHashedPasswordWithSalt.substr(delimiterPos + 1);
////
////    // 计算输入密码的哈希值，包括盐
////    unsigned char computedHash[SHA256_DIGEST_LENGTH];
////    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
////    if (!ctx) {
////        std::cerr << "EVP_MD_CTX_new failed." << std::endl;
////        return false;
////    }
////    EVP_DigestInit(ctx, EVP_sha256());
////    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
////    EVP_DigestUpdate(ctx, (const unsigned char*)storedSalt.c_str(), storedSalt.length());
////    EVP_DigestFinal(ctx, computedHash, nullptr);
////    EVP_MD_CTX_free(ctx);
////
////    // 将计算出的哈希值转换为字符串
////    std::string computedHashStr;
////    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
////        char buf[3];
////        sprintf_s(buf, "%02x", computedHash[i]); // 使用 sprintf 而不是 sprintf_s
////        computedHashStr += buf;
////    }
////
////    // 比较哈希值
////    bool success = (computedHashStr == storedHash);
////    if (!success) {
////        std::cout << "用户名或密码错误，登录失败。" << std::endl;
////    }
////    return success;
////}
//
//
//// 从名为 userdata.txt 的文件中加载用户信息到内存中的 users 哈希表中
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
//// 将内存中的用户信息保存到数据库
//void AuthenticationService::saveUsersToFile()
//{
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("连接数据库失败: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    // 删除原有数据
//    mysql_query(&mysql, "DELETE FROM users;");
//
//    // 插入新数据
//    for (const auto& pair : users)
//    {
//        std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + pair.first + "', '" + pair.second + "');";
//        mysql_query(&mysql, query.c_str());
//    }
//
//    mysql_close(&mysql);
//}
//
//// 哈希算法加密
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

//bool AuthenticationService::login(const std::string& username, const std::string& password)
//{
//    auto it = users.find(username);
//    if (it == users.end()) {
//        std::cout << "用户名不存在。" << std::endl;
//        return false;
//    }
//
//    std::string hashedPasswordWithSalt = it->second;
//    size_t delimiterPos = hashedPasswordWithSalt.find(':');
//    if (delimiterPos == std::string::npos) {
//        std::cout << "错误的哈希格式。" << std::endl;
//        return false;
//    }
//
//    std::string storedHash = hashedPasswordWithSalt.substr(0, delimiterPos);
//    std::string storedSalt = hashedPasswordWithSalt.substr(delimiterPos + 1);
//
//    unsigned char computedHash[SHA256_DIGEST_LENGTH];
//    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
//    EVP_DigestInit(ctx, EVP_sha256());
//    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
//    EVP_DigestUpdate(ctx, (const unsigned char*)storedSalt.c_str(), storedSalt.length());
//    EVP_DigestFinal(ctx, computedHash, nullptr);
//    EVP_MD_CTX_free(ctx);
//
//    std::string computedHashStr;
//    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
//        char buf[3];
//        sprintf_s(buf, "%02x", computedHash[i]);
//        computedHashStr += buf;
//    }
//
//    return computedHashStr == storedHash;
//}


bool AuthenticationService::login(const std::string& username, const std::string& password)
{
    auto it = users.find(username);
    if (it == users.end()) {
        std::cout << "用户名不存在。" << std::endl;
        return false;
    }

    // 显示从数据库检索到的哈希密码和盐值
    std::string hashedPasswordWithSalt = it->second;
    size_t delimiterPos = hashedPasswordWithSalt.find(':');
    std::string storedHash = hashedPasswordWithSalt.substr(0, delimiterPos);
    std::string storedSalt = hashedPasswordWithSalt.substr(delimiterPos + 1);
    std::cout << "从数据库检索到的哈希密码：" << storedHash << std::endl;
    std::cout << "从数据库检索到的盐值：" << storedSalt << std::endl;

    unsigned char salt[SHA256_DIGEST_LENGTH];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sscanf_s(storedSalt.c_str() + i * 2, "%02x", &salt[i]);
    }

    //unsigned char salt[17] = "Gk4^r8@qF0o!Z3Vb";
    unsigned char hash[SHA256_DIGEST_LENGTH];
    /*EVP_MD_CTX* ctx = EVP_MD_CTX_new();
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
    std::cout << "当前密码的哈希密码：" << hashedPassword << std::endl;

    return hashedPassword == storedHash;*/
    std::string hashedPassword;
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, EVP_sha256());
    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
    EVP_DigestUpdate(ctx, salt, sizeof(salt));
    EVP_DigestFinal(ctx, hash, nullptr);
    EVP_MD_CTX_free(ctx);

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

    printf("注册用户：%s, 哈希后的密码：%s, 盐值：%s\\\\n", username.c_str(), hashedPasswordWithoutSalt.c_str(), salt.c_str());

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


//bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
//{
//    if (users.find(username) != users.end())
//        return false;
//
//    std::string hashedPassword = hashPassword(password);
//
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("连接数据库失败: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
//    std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));
//
//    std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
//    if (mysql_query(&mysql, query.c_str()) == 0) {
//        users[username] = hashedPassword;
//        mysql_close(&mysql);
//        return true;
//    }
//    else {
//        printf("插入数据失败: %s\\n", mysql_error(&mysql));
//        mysql_close(&mysql);
//        return false;
//    }
//}



std::string AuthenticationService::hashPassword(const std::string& password)
{
    unsigned char salt[SHA256_DIGEST_LENGTH];
    RAND_bytes(salt, sizeof(salt));
    //unsigned char salt[17] = "Gk4^r8@qF0o!Z3Vb";
    std::string saltStr;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        char buf[3];
        sprintf_s(buf, "%02x", salt[i]);
        saltStr += buf;
    }

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
    for (int i = 0; i < sizeof(saltStr); i++) {
        char buf[3];
        sprintf_s(buf, "%02x", saltStr[i]);
        hashedPassword += buf;
    }

    return hashedPassword;
}

