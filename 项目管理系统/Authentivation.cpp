//#include "Authentication.h"
//
//// ���캯������������ݿ�����û�����
//AuthenticationService::AuthenticationService()
//{
//    // �����ݿ�����û����ݵ���ϣ��
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("�������ݿ�ʧ��: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    if (mysql_query(&mysql, "SELECT username, hashed_password, salt FROM users;") != 0) {
//        printf("��ѯ���ݿ�ʧ��: %s\\n", mysql_error(&mysql));
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
//        return false;
//    }
//
//    std::string hashedPasswordWithSalt = it->second;
//    size_t delimiterPos = hashedPasswordWithSalt.find(':');
//    if (delimiterPos == std::string::npos)
//    {
//        return false; // ����Ĺ�ϣ��ʽ
//    }
//
//    std::string hashedPassword = hashedPasswordWithSalt.substr(0, delimiterPos);
//    std::string salt = hashedPasswordWithSalt.substr(delimiterPos + 1);
//
//    // ���¼����ϣֵ
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
//    // �ȽϹ�ϣֵ
//    return computedHashedPassword == hashedPassword;
//}
//
//// ����ע�����û�
//bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
//{
//    if (users.find(username) == users.end())
//    {
//        std::string hashedPassword = hashPassword(password);
//
//        // ���û���Ϣ�������ݿ�
//        MYSQL mysql;
//        mysql_init(&mysql);
//        if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//            printf("�������ݿ�ʧ��: %s\\\\n", mysql_error(&mysql));
//            exit(-1);
//        }
//
//        std::string query = "SELECT username, hashed_password, salt FROM users WHERE username = '" + username + "';";
//        if (mysql_query(&mysql, query.c_str()) != 0) {
//            printf("��ѯ���ݿ�ʧ��: %s\\\\n", mysql_error(&mysql));
//            exit(-1);
//        }
//
//        MYSQL_RES* res = mysql_store_result(&mysql);
//        if (res && mysql_num_rows(res) > 0) {
//            mysql_free_result(res);
//            mysql_close(&mysql);
//            return false; // �û����Ѵ���
//        }
//        mysql_free_result(res);
//
//        std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
//        std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));
//
//        query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
//        if (mysql_query(&mysql, query.c_str()) != 0) {
//            printf("��������ʧ��: %s\\\\n", mysql_error(&mysql));
//            exit(-1);
//        }
//
//        mysql_close(&mysql);
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
//// ���ڴ��е��û���Ϣ���浽���ݿ�
//void AuthenticationService::saveUsersToFile()
//{
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("�������ݿ�ʧ��: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    // ɾ��ԭ������
//    mysql_query(&mysql, "DELETE FROM users;");
//
//    // ����������
//    for (const auto& pair : users)
//    {
//        std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + pair.first + "', '" + pair.second + "');";
//        mysql_query(&mysql, query.c_str());
//    }
//
//    mysql_close(&mysql);
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


//// ����ע�����û�
//bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
//{
//    if (users.find(username) == users.end())
//    {
//        std::string hashedPassword = hashPassword(password);
//
//        // ���û���Ϣ�������ݿ�
//        MYSQL mysql;
//        mysql_init(&mysql);
//        if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//            printf("�������ݿ�ʧ��: %s\\\\n", mysql_error(&mysql));
//            exit(-1);
//        }
//
//        std::string query = "SELECT username, hashed_password, salt FROM users WHERE username = '" + username + "';";
//        if (mysql_query(&mysql, query.c_str()) != 0) {
//            printf("��ѯ���ݿ�ʧ��: %s\\\\n", mysql_error(&mysql));
//            exit(-1);
//        }
//
//        MYSQL_RES* res = mysql_store_result(&mysql);
//        if (res && mysql_num_rows(res) > 0) {
//            mysql_free_result(res);
//            mysql_close(&mysql);
//            return false; // �û����Ѵ���
//        }
//        mysql_free_result(res);
//
//        std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
//        std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));
//
//        query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
//        if (mysql_query(&mysql, query.c_str()) != 0) {
//            printf("��������ʧ��: %s\\\\n", mysql_error(&mysql));
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
//        std::cout << "�û����Ѵ��ڣ�ע��ʧ�ܡ�" << std::endl;
//        return false;
//    }
//
//    std::string hashedPassword = hashPassword(password);
//
//    // ֱ�ӳ��Բ������û�������Ԥ�Ȳ�ѯ
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("�������ݿ�ʧ��: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
//    std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));
//
//    std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
//    if (mysql_query(&mysql, query.c_str()) == 0) {
//        std::cout << "�û�ע��ɹ���" << std::endl;
//        users[username] = hashedPassword; // ע��ɹ�������ڴ��е��û��б�
//        mysql_close(&mysql);
//        return true;
//    }
//    else {
//        printf("��������ʧ��: %s\\n", mysql_error(&mysql));
//        mysql_close(&mysql);
//        return false;
//    }
//}


// ������֤�û���¼
//bool AuthenticationService::login(const std::string& username, const std::string& password)
//{
//    auto it = users.find(username);
//    if (it == users.end())
//    {
//        std::cerr << "�û��������ڣ�����ע�ᡣ" << std::endl;
//        return false;
//    }
//
//    std::string hashedPasswordWithSalt = it->second;
//    size_t delimiterPos = hashedPasswordWithSalt.find(':');
//    if (delimiterPos == std::string::npos)
//    {
//        return false; // ����Ĺ�ϣ��ʽ
//    }
//
//    std::string hashedPassword = hashedPasswordWithSalt.substr(0, delimiterPos);
//    std::string salt = hashedPasswordWithSalt.substr(delimiterPos + 1);
//
//    // ���¼����ϣֵ
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
//    // �ȽϹ�ϣֵ
//    return computedHashedPassword == hashedPassword;
//}

//// Authentication.cpp
//#include "Authentication.h"
//
//// ���캯������������ݿ�����û�����
//AuthenticationService::AuthenticationService()
//{
//    // �����ݿ�����û����ݵ���ϣ��
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("�������ݿ�ʧ��: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    if (mysql_query(&mysql, "SELECT username, hashed_password, salt FROM users;") != 0) {
//        printf("��ѯ���ݿ�ʧ��: %s\\n", mysql_error(&mysql));
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
////����ע�����û�
//bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
//{
//    if (users.find(username) != users.end())
//    {
//        return false; // �û����Ѵ��ڣ������ӡ��Ϣ��ֱ�ӷ���
//    }
//
//    std::string hashedPassword = hashPassword(password);
//
//    // �������ݿⲢ���Բ���
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("�������ݿ�ʧ��: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
//    std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));
//
//    std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
//    if (mysql_query(&mysql, query.c_str()) == 0) {
//        //std::cout << "�û�ע��ɹ���" << std::endl; // ֻ�������ӡ�ɹ���Ϣ
//        users[username] = hashedPassword; // �����ڴ��е��û��б�
//        mysql_close(&mysql);
//        return true;
//    }
//    else {
//        printf("��������ʧ��: %s\\n", mysql_error(&mysql));
//        mysql_close(&mysql);
//        return false;
//    }
//}
//
////������֤�û���¼
//bool AuthenticationService::login(const std::string& username, const std::string& password)
//{
//    auto it = users.find(username);
//    if (it == users.end())
//    {
//        std::cout << "�û��������ڡ�" << std::endl;
//        return false;
//    }
//
//    std::string hashedPasswordWithSalt = it->second;
//    size_t delimiterPos = hashedPasswordWithSalt.find(':');
//    if (delimiterPos == std::string::npos)
//    {
//        std::cout << "����Ĺ�ϣ��ʽ��" << std::endl;
//        return false;
//    }
//
//    std::string storedHash = hashedPasswordWithSalt.substr(0, delimiterPos);
//    std::string storedSalt = hashedPasswordWithSalt.substr(delimiterPos + 1);
//
//    // ������������Ĺ�ϣֵ
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
//    std::cout << "����Ĺ�ϣ: " << computedHashStr << ", �洢�Ĺ�ϣ: " << storedHash << std::endl;
//
//    // �ȽϹ�ϣֵ
//    return computedHashStr == storedHash;
//}
//
////bool AuthenticationService::login(const std::string& username, const std::string& password)
////{
////    auto it = users.find(username);
////    if (it == users.end())
////    {
////        std::cout << "�û��������ڡ�" << std::endl;
////        return false;
////    }
////
////    std::string storedHashedPasswordWithSalt = it->second;
////    size_t delimiterPos = storedHashedPasswordWithSalt.find(':');
////    if (delimiterPos == std::string::npos)
////    {
////        std::cout << "����Ĺ�ϣ��ʽ��" << std::endl;
////        return false;
////    }
////
////    std::string storedHash = storedHashedPasswordWithSalt.substr(0, delimiterPos);
////    std::string storedSalt = storedHashedPasswordWithSalt.substr(delimiterPos + 1);
////
////    // ������������Ĺ�ϣֵ��������
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
////    // ��������Ĺ�ϣֵת��Ϊ�ַ���
////    std::string computedHashStr;
////    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
////        char buf[3];
////        sprintf_s(buf, "%02x", computedHash[i]); // ʹ�� sprintf ������ sprintf_s
////        computedHashStr += buf;
////    }
////
////    // �ȽϹ�ϣֵ
////    bool success = (computedHashStr == storedHash);
////    if (!success) {
////        std::cout << "�û�����������󣬵�¼ʧ�ܡ�" << std::endl;
////    }
////    return success;
////}
//
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
//// ���ڴ��е��û���Ϣ���浽���ݿ�
//void AuthenticationService::saveUsersToFile()
//{
//    MYSQL mysql;
//    mysql_init(&mysql);
//    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0) == NULL) {
//        printf("�������ݿ�ʧ��: %s\\n", mysql_error(&mysql));
//        exit(-1);
//    }
//
//    // ɾ��ԭ������
//    mysql_query(&mysql, "DELETE FROM users;");
//
//    // ����������
//    for (const auto& pair : users)
//    {
//        std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + pair.first + "', '" + pair.second + "');";
//        mysql_query(&mysql, query.c_str());
//    }
//
//    mysql_close(&mysql);
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

AuthenticationService::AuthenticationService()
{
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

AuthenticationService::~AuthenticationService()
{
}

//bool AuthenticationService::login(const std::string& username, const std::string& password)
//{
//    auto it = users.find(username);
//    if (it == users.end()) {
//        std::cout << "�û��������ڡ�" << std::endl;
//        return false;
//    }
//
//    std::string hashedPasswordWithSalt = it->second;
//    size_t delimiterPos = hashedPasswordWithSalt.find(':');
//    if (delimiterPos == std::string::npos) {
//        std::cout << "����Ĺ�ϣ��ʽ��" << std::endl;
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
        std::cout << "�û��������ڡ�" << std::endl;
        return false;
    }

    // ��ʾ�����ݿ�������Ĺ�ϣ�������ֵ
    std::string hashedPasswordWithSalt = it->second;
    size_t delimiterPos = hashedPasswordWithSalt.find(':');
    std::string storedHash = hashedPasswordWithSalt.substr(0, delimiterPos);
    std::string storedSalt = hashedPasswordWithSalt.substr(delimiterPos + 1);
    std::cout << "�����ݿ�������Ĺ�ϣ���룺" << storedHash << std::endl;
    std::cout << "�����ݿ����������ֵ��" << storedSalt << std::endl;

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
    std::cout << "��ǰ����Ĺ�ϣ���룺" << hashedPassword << std::endl;

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
        printf("�������ݿ�ʧ��: %s\\\\n", mysql_error(&mysql));
        exit(-1);
    }

    std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
    std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));

    printf("ע���û���%s, ��ϣ������룺%s, ��ֵ��%s\\\\n", username.c_str(), hashedPasswordWithoutSalt.c_str(), salt.c_str());

    std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
    if (mysql_query(&mysql, query.c_str()) == 0) {
        users[username] = hashedPassword;
        mysql_close(&mysql);
        return true;
    }
    else {
        printf("��������ʧ��: %s\\\\n", mysql_error(&mysql));
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
//        printf("�������ݿ�ʧ��: %s\\n", mysql_error(&mysql));
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
//        printf("��������ʧ��: %s\\n", mysql_error(&mysql));
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

