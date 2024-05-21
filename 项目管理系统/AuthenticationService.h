#define _CRT_SECURE_NO_WARNINGS

#ifndef AUTHENTICATION_SERVICE_H
#define AUTHENTICATION_SERVICE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <mysql.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

/**
 * AuthenticationService 类用于管理用户认证。
 * 包括注册新用户、用户登录、密码加密和验证以及用户信息的存取。
 */
class AuthenticationService {
public:
    /**
     * 构造函数。
     * 负责从数据库中加载用户数据。
     */
    AuthenticationService();

    /**
     * 析构函数。
     * 负责将用户数据保存到数据库中。
     */
    ~AuthenticationService();

    /**
     * 用户登录。
     *
     * @param username 用户名。
     * @param password 用户密码。
     * @return 成功返回true，否则返回false。
     */
    bool login(const std::string& username, const std::string& password);

    /**
     * 注册新用户。
     *
     * @param username 新用户名。
     * @param password 新用户密码。
     * @return 如果用户名已存在返回false，否则注册用户并返回true。
     */
    bool registerUser(const std::string& username, const std::string& password);

private:
    std::unordered_map<std::string, std::string> users; // 密码哈希的映射到用户名

    /**
     * 从数据库加载用户信息。
     */
    void loadUsersFromDB();

    /**
     * 将用户信息保存到数据库。
     */
    void saveUsersToDB();

    /**
     * 对密码进行加密。
     *
     * @param password 明文密码。
     * @return 加密后的密码。
     */
    std::string hashPassword(const std::string& password);

    /**
     * 数据库连接信息
     */
    MYSQL* connectDB();

    /**
     * 检查并创建数据库和表。
     */
    void checkAndCreateDatabase();
};

#endif // AUTHENTICATION_SERVICE_H
