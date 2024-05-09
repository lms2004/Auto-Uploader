#ifndef AUTHENTICATION_SERVICE_H
#define AUTHENTICATION_SERVICE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

//����ʹ�����ݿ�Ŀ�
#include <mysql.h>

//��������������ܵĿ�
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
     * ����Ӵ洢�м����û����ݡ�����������������л�ȡ�ļ��е����ݣ�Ȼ��洢�������users�
     */
    AuthenticationService();

    /**
     * ����������
     * �����û����ݱ��浽�洢�С�����������������а������users�ļ��е����ݴ洢���ļ��
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

    /**
     * ���ļ�ϵͳ�����û���Ϣ��
     */
    void loadUsersFromFile();

    /**
     * ���û���Ϣ���浽�ļ�ϵͳ(���磺txt�ļ�)��
     * ע����û���Ϣ��������ʽ�洢
     */
    void saveUsersToFile();

private:
    std::unordered_map<std::string, std::string> users; // �����ϣ��ӳ�䵽�û���

    ///**
    // * ���ļ�ϵͳ�����û���Ϣ��
    // */
    //void loadUsersFromFile();

    ///**
    // * ���û���Ϣ���浽�ļ�ϵͳ(���磺txt�ļ�)��
    // * ע����û���Ϣ��������ʽ�洢
    // */
    //void saveUsersToFile();

    /**
     * ��������м��ܡ�
     *
     * @param password �������롣
     * @return ���ܺ�����롣
     */
    std::string hashPassword(const std::string& password);

};

#endif // AUTHENTICATION_SERVICE_H