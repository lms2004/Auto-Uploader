#include "Authentication.h"

// ����һ���̶����ȵ�saltֵ�����������ϣ����
unsigned char salt[50] = "Gk4^r8@qF0o!Z3Vb$2y$10$V5F9zgYjZLZ1%sZ3Zq5u8Nw8e";

// AuthenticationService��Ĺ��캯��
AuthenticationService::AuthenticationService()
{
    // ��ʼ��MySQL����
    MYSQL mysql;
    mysql_init(&mysql);
    // �������ӵ����ݿ�
    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "data", 3306, NULL, 0) == NULL) 
    {
        // �������ʧ�ܣ���ӡ������Ϣ���˳�����
        printf("�������ݿ�ʧ��: %s\\n", mysql_error(&mysql));
        exit(-1);
    }

    // ִ��SQL��ѯ����ȡ�û���Ϣ
    if (mysql_query(&mysql, "SELECT username, hashed_password, salt FROM users;") != 0) 
    {
        // �����ѯʧ�ܣ���ӡ������Ϣ���˳�����
        printf("��ѯ���ݿ�ʧ��: %s\\n", mysql_error(&mysql));
        exit(-1);
    }

    // ��ȡ��ѯ���
    MYSQL_RES* res = mysql_store_result(&mysql);
    if (res) 
    {
        // ������ѯ��������û���Ϣ�洢�����ػ�����
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            users[row[0]] = std::string(row[1]) + ":" + std::string(row[2]);
        }
        // �ͷŲ�ѯ����ڴ�
        mysql_free_result(res);
    }

    // �ر����ݿ�����
    mysql_close(&mysql);
}

// AuthenticationService�����������
AuthenticationService::~AuthenticationService()
{
}


// �û���¼��֤����
bool AuthenticationService::login(const std::string& username, const std::string& password) 
{
    // ����û����Ƿ�����ڱ��ػ�����
    auto it = users.find(username);
    if (it == users.end()) 
    {
        // ����û��������ڣ���ӡ��Ϣ�����ص�¼ʧ��
        std::cout << "�û��������ڡ�" << std::endl;
        return false;
    }

    // �ӱ��ػ����л�ȡ�û��Ĺ�ϣ�����salt
    std::string hashedPasswordWithSalt = it->second;
    size_t delimiterPos = hashedPasswordWithSalt.find(':');
    std::string storedHash = hashedPasswordWithSalt.substr(0, delimiterPos);

    // ʹ������������salt�����ϣֵ
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, EVP_sha256());
    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
    EVP_DigestUpdate(ctx, salt, sizeof(salt));
    EVP_DigestFinal(ctx, hash, nullptr);
    EVP_MD_CTX_free(ctx);

    // ����ϣֵת��Ϊ�ַ�����ʽ
    std::string hashedPassword;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        char buf[3];
        sprintf_s(buf, "%02x", hash[i]);
        hashedPassword += buf;
    }

    // �Ƚϼ�����Ĺ�ϣֵ�ʹ洢�Ĺ�ϣֵ�Ƿ�һ��
    return hashedPassword == storedHash;
}

// �û�ע�ắ��
bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
{
    // ����û����Ƿ��Ѵ���
    if (users.find(username) != users.end())
        return false;

    // ��������й�ϣ����
    std::string hashedPassword = hashPassword(password);

    // ��ʼ��MySQL����
    MYSQL mysql;
    mysql_init(&mysql);
    // �������ӵ����ݿ�
    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "data", 3306, NULL, 0) == NULL) 
    {
        // �������ʧ�ܣ���ӡ������Ϣ���˳�����
        printf("�������ݿ�ʧ��: %s\\\\n", mysql_error(&mysql));
        exit(-1);
    }

    // �ӹ�ϣ��������ȡsaltֵ
    std::string salt = hashedPassword.substr(hashedPassword.find(':') + 1);
    std::string hashedPasswordWithoutSalt = hashedPassword.substr(0, hashedPassword.find(':'));

    // ����������ݿ��SQL���
    std::string query = "INSERT INTO users (username, hashed_password, salt) VALUES ('" + username + "', '" + hashedPasswordWithoutSalt + "', '" + salt + "');";
    // ִ��SQL�������
    if (mysql_query(&mysql, query.c_str()) == 0)
    {
        // �������ɹ������±��ػ��沢�ر����ݿ�����
        users[username] = hashedPassword;
        mysql_close(&mysql);
        return true;
    }
    else 
    {
        // �������ʧ�ܣ���ӡ������Ϣ���ر����ݿ�����
        printf("��������ʧ��: %s\\\\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return false;
    }
}

// �����ϣ������
std::string AuthenticationService::hashPassword(const std::string& password)
{
    // ��ʼ����ϣ������
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, EVP_sha256());
    // ���¹�ϣ�����ģ�ʹ������������salt
    EVP_DigestUpdate(ctx, (const unsigned char*)password.c_str(), password.length());
    EVP_DigestUpdate(ctx, salt, sizeof(salt));
    // ��ɹ�ϣ����
    EVP_DigestFinal(ctx, hash, nullptr);
    // �ͷŹ�ϣ�������ڴ�
    EVP_MD_CTX_free(ctx);

    // ����ϣֵת��Ϊ�ַ�����ʽ
    std::string hashedPassword;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        char buf[3];
        sprintf_s(buf, "%02x", hash[i]);
        hashedPassword += buf;
    }

    // ��saltֵҲת��Ϊ�ַ�����ʽ�������ϣֵƴ��
    hashedPassword += ":";
    for (int i = 0; i < sizeof(salt); i++) {
        char buf[3];
        sprintf_s(buf, "%02x", salt[i]);
        hashedPassword += buf;
    }

    // ����ƴ�Ӻ�Ĺ�ϣ����
    return hashedPassword;
}