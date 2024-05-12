#ifndef AUTHENTICATION_SERVICE_H
#define AUTHENTICATION_SERVICE_H

#include <iostream>
#include <unordered_map>

//����ʹ�����ݿ�Ŀ�
#include <mysql.h>

//��������������ܵĿ�
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

class AuthenticationService {
public:
    AuthenticationService();
    ~AuthenticationService();

    bool login(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password);

private:
    std::unordered_map<std::string, std::string> users;
    std::string hashPassword(const std::string& password);
};

#endif // AUTHENTICATION_SERVICE_H
