#include "Authentication.h"

// ���캯��������Ӵ洢�м����û�����
AuthenticationService::AuthenticationService()
{
    // ���ļ������û����ݵ������
    std::ifstream file("userdata.txt");
    if (file.is_open())
    {
        std::string username;
        std::string password;
        while (file >> username >> password)
        {
            users[username] = password;
        }
        file.close();
    }
}

// ��������,�����û����ݱ��浽�洢��
AuthenticationService::~AuthenticationService()
{
    // ��������е��û����ݱ��浽�ļ���
    std::ofstream file("userdata.txt");
    if (file.is_open())
    {
        for (const auto& user : users)
        {
            file << user.first << " " << user.second << std::endl;
        }
        file.close();
    }
}

// ������֤�û���¼
bool AuthenticationService::login(const std::string& username, const std::string& password)
{
    auto it = users.find(username);
    if (it != users.end())
    {
        // �û����ڣ��������
        std::string hashedPassword = it->second;
        std::vector<unsigned char> salt(SHA256_DIGEST_LENGTH);
        std::string saltStr;
        std::istringstream iss(hashedPassword);
        std::getline(iss, saltStr, ':');
        for (int i = 0; i < saltStr.length() / 2; i++)
        {
            std::string byteStr = saltStr.substr(2 * i, 2);
            unsigned char byte = (unsigned char)strtol(byteStr.c_str(), nullptr, 16);
            salt[i] = byte;
        }

        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestInit(ctx, EVP_sha256());
        EVP_DigestUpdate(ctx, password.c_str(), password.length());
        EVP_DigestUpdate(ctx, salt.data(), salt.size());
        unsigned char hash[SHA256_DIGEST_LENGTH];
        EVP_DigestFinal(ctx, hash, nullptr);
        EVP_MD_CTX_free(ctx);

        std::string hashedInputPassword;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            char buf[3];
            sprintf_s(buf, "%02x", hash[i]);
            hashedInputPassword += buf;
        }
        return hashedInputPassword == saltStr.substr(saltStr.find(':') + 1);
    }
    return false;
}

// ����ע�����û�
bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
{
    if (users.find(username) == users.end())
    {
        std::string hashedPassword = hashPassword(password);
        users[username] = hashedPassword;
        saveUsersToFile();
        return true;
    }
    return false;
}

// ����Ϊ users.txt ���ļ��м����û���Ϣ���ڴ��е� users ��ϣ����
void AuthenticationService::loadUsersFromFile()
{
    std::ifstream file("users.txt");
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string username;
        std::string hashedPassword;
        if (std::getline(iss, username, ':') && std::getline(iss, hashedPassword))
        {
            users[username] = hashedPassword;
        }
    }
}

// ���ڴ��е��û���Ϣ���浽��Ϊ users.txt ���ļ���
void AuthenticationService::saveUsersToFile()
{
    std::ofstream file("user.txt");
    for (const auto& pair : users)
    {
        file << pair.first << ":" << pair.second << std::endl;
    }
}

// ��ϣ�㷨����
std::string hashPassword(const std::string& password)
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
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        char buf[3];
        sprintf_s(buf, "%02x", hash[i]);
        hashedPassword += buf;
    }

    hashedPassword += ":";
    for (int i = 0; i < sizeof(salt); i++)
    {
        char buf[3];
        sprintf_s(buf, "%02x", salt[i]);
        hashedPassword += buf;
    }

    return hashedPassword;
}
