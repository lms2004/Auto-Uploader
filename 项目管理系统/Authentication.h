#ifndef AUTHENTICATION_SERVICE_H
#define AUTHENTICATION_SERVICE_H

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
// #include "SomeHashLibrary.h" // 示例：引入用于密码加密的库

/**
 * AuthenticationService 类用于管理用户认证。
 * 包括注册新用户、用户登录、密码加密和解密以及用户信息的存取。
 *
 * 注意：本示例使用文件系统存储用户信息，实际应用中推荐使用数据库系统。
 */
class AuthenticationService {
public:
    /**
     * 构造函数。
     * 负责从存储中加载用户数据。（就是在这个函数中获取文件中的数据，然后存储到无序表users里）
     */
    AuthenticationService();

    /**
     * 析构函数。
     * 负责将用户数据保存到存储中。（就是在这个函数中把无序表users文件中的数据存储到文件里）
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
     * 从文件系统加载用户信息。
     * 实际应用应根据使用的数据库调整此函数。
     */
    void loadUsersFromFile();

    /**
     * 将用户信息保存到文件系统(例如：txt文件)。
     * 注意把用户信息以密文形式存储
     */
    void saveUsersToFile();

    /**
     * 对密码进行加密。
     *
     * @param password 明文密码。
     * @return 加密后的密码。
     */
    std::string hashPassword(const std::string& password);

    /**
     * 对密码进行解密（如果需要）。
     *
     * @param hashedPassword 加密后的密码。
     * @return 解密后的密码。
     */
    std::string decryptPassword(const std::string& hashedPassword); // 根据需要实现
};

#endif // AUTHENTICATION_SERVICE_H
