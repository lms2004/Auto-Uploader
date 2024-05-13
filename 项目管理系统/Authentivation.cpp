#include "Authentication.h"

// 定义一个固定长度的salt值，用于密码哈希处理
unsigned char salt[50] = "Gk4^r8@qF0o!Z3Vb$2y$10$V5F9zgYjZLZ1%sZ3Zq5u8Nw8e";

// AuthenticationService类的构造函数
AuthenticationService::AuthenticationService()
{
    // 初始化MySQL连接
    MYSQL mysql;
    mysql_init(&mysql);
    // 尝试连接到数据库
    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "data", 3306, NULL, 0) == NULL) 
    {
        // 如果连接失败，打印错误信息并退出程序
        printf("连接数据库失败: %s\\n", mysql_error(&mysql));
        exit(-1);
    }

    // 执行SQL查询，获取用户信息
    if (mysql_query(&mysql, "SELECT username, hashed_password, salt FROM users;") != 0) 
    {
        // 如果查询失败，打印错误信息并退出程序
        printf("查询数据库失败: %s\\n", mysql_error(&mysql));
        exit(-1);
    }

    // 获取查询结果
    MYSQL_RES* res = mysql_store_result(&mysql);
    if (res) 
    {
        // 遍历查询结果，将用户信息存储到本地缓存中
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            users[row[0]] = std::string(row[1]) + ":" + std::string(row[2]);
        }
        // 释放查询结果内存
        mysql_free_result(res);
    }

    // 关闭数据库连接
    mysql_close(&mysql);
}

// AuthenticationService类的析构函数
AuthenticationService::~AuthenticationService()
{
}


// 用户登录验证函数
bool AuthenticationService::login(const std::string& username, const std::string& password) 
{
    // 检查用户名是否存在于本地缓存中
    auto it = users.find(username);
    if (it == users.end()) 
    {
        // 如果用户名不存在，打印信息并返回登录失败
        std::cout << "用户名不存在。" << std::endl;
        return false;
    }

    // 从本地缓存中获取用户的哈希密码和salt
    std::string hashedPasswordWithSalt = it->second;
    size_t delimiterPos = hashedPasswordWithSalt.find(':');
    std::string storedHash = hashedPasswordWithSalt.substr(0, delimiterPos);

    // 使用输入的密码和salt计算哈希值
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, EVP_sha256());
    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
    EVP_DigestUpdate(ctx, salt, sizeof(salt));
    EVP_DigestFinal(ctx, hash, nullptr);
    EVP_MD_CTX_free(ctx);

    // 将哈希值转换为字符串格式
    std::string hashedPassword;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        char buf[3];
        sprintf_s(buf, "%02x", hash[i]);
        hashedPassword += buf;
    }

    // 比较计算出的哈希值和存储的哈希值是否一致
    return hashedPassword == storedHash;
}

// 用户注册函数
bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
{
    // 检查用户名是否已存在
    if (users.find(username) != users.end())
        return false;

    // 对密码进行哈希处理
    std::string hashedPassword = hashPassword(password);

    // 初始化MySQL连接
    MYSQL mysql;
    mysql_init(&mysql);
    // 尝试连接到数据库
    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "data", 3306, NULL, 0) == NULL) 
    {
        // 如果连接失败，打印错误信息并退出程序
        printf("连接数据库失败: %s\\\\n", mysql_error(&mysql));
        exit(-1);
    }

    // 从哈希密码中提取salt值
    std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
    std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));

    // 构造插入数据库的SQL语句
    std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
    // 执行SQL插入操作
    if (mysql_query(&mysql, query.c_str()) == 0)
    {
        // 如果插入成功，更新本地缓存并关闭数据库连接
        users[username] = hashedPassword;
        mysql_close(&mysql);
        return true;
    }
    else 
    {
        // 如果插入失败，打印错误信息并关闭数据库连接
        printf("插入数据失败: %s\\\\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return false;
    }
}

// 密码哈希处理函数
std::string AuthenticationService::hashPassword(const std::string& password)
{
    // 初始化哈希上下文
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, EVP_sha256());
    // 更新哈希上下文，使用输入的密码和salt
    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
    EVP_DigestUpdate(ctx, salt, sizeof(salt));
    // 完成哈希计算
    EVP_DigestFinal(ctx, hash, nullptr);
    // 释放哈希上下文内存
    EVP_MD_CTX_free(ctx);

    // 将哈希值转换为字符串格式
    std::string hashedPassword;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        char buf[3];
        sprintf_s(buf, "%02x", hash[i]);
        hashedPassword += buf;
    }

    // 将salt值也转换为字符串格式，并与哈希值拼接
    hashedPassword += ":";
    for (int i = 0; i < sizeof(salt); i++) {
        char buf[3];
        sprintf_s(buf, "%02x", salt[i]);
        hashedPassword += buf;
    }

    // 返回拼接后的哈希密码
    return hashedPassword;
}