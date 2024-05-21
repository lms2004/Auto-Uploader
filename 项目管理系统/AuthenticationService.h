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
 * AuthenticationService �����ڹ����û���֤��
 * ����ע�����û����û���¼��������ܺ���֤�Լ��û���Ϣ�Ĵ�ȡ��
 */
class AuthenticationService {
public:
    /**
     * ���캯����
     * ��������ݿ��м����û����ݡ�
     */
    AuthenticationService();

    /**
     * ����������
     * �����û����ݱ��浽���ݿ��С�
     */
    ~AuthenticationService();

    /**
     * �û���¼��
     *
     * @param username �û�����
     * @param password �û����롣
     * @return �ɹ�����true�����򷵻�false��
     */
    bool login(const std::string& username, const std::string& password);

    /**
     * ע�����û���
     *
     * @param username ���û�����
     * @param password ���û����롣
     * @return ����û����Ѵ��ڷ���false������ע���û�������true��
     */
    bool registerUser(const std::string& username, const std::string& password);

private:
    std::unordered_map<std::string, std::string> users; // �����ϣ��ӳ�䵽�û���

    /**
     * �����ݿ�����û���Ϣ��
     */
    void loadUsersFromDB();

    /**
     * ���û���Ϣ���浽���ݿ⡣
     */
    void saveUsersToDB();

    /**
     * ��������м��ܡ�
     *
     * @param password �������롣
     * @return ���ܺ�����롣
     */
    std::string hashPassword(const std::string& password);

    /**
     * ���ݿ�������Ϣ
     */
    MYSQL* connectDB();

    /**
     * ��鲢�������ݿ�ͱ�
     */
    void checkAndCreateDatabase();
};

#endif // AUTHENTICATION_SERVICE_H
