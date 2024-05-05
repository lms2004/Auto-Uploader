#ifndef AUTHENTICATION_SERVICE_H
#define AUTHENTICATION_SERVICE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

//引入用于密码加密的库
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
     */
    void loadUsersFromFile();

    /**
     * 将用户信息保存到文件系统(例如：txt文件)。
     * 注意把用户信息以密文形式存储
     */
    void saveUsersToFile();


    /**
    * 生成一个随机的盐值，并将其以十六进制的形式返回。
    * 在密码存储过程中，盐值的作用是增加密码的复杂度。
    * 使得相同的密码在不同用户之间存储的哈希值也不同，增加了破解的难度。
    */
    std::vector<unsigned char> generateSalt(size_t length);
    
    /**
     * 对密码进行加密。
     *
     * @param password 明文密码。
     * @return 加密后的密码。
     */
    std::string hashPassword(const std::string& password);

};

#endif // AUTHENTICATION_SERVICE_H
