#ifndef AUTHENTICATION_SERVICE_H
#define AUTHENTICATION_SERVICE_H

#include <iostream>
#include <unordered_map>

// ����MySQL���ݿ��
#include <mysql.h>

// ����OpenSSL�⣬�����������
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>


// ��֤������
class AuthenticationService {
public:
    AuthenticationService();
    ~AuthenticationService();

    // ��¼����
    bool login(const std::string& username, const std::string& password);
    // ע�ắ��
    bool registerUser(const std::string& username, const std::string& password);

private:
    // ���ػ�����û���Ϣ
    std::unordered_map<std::string, std::string> users;
    // �����ϣ������
    std::string hashPassword(const std::string& password);
};

#endif // AUTHENTICATION_SERVICE_H

