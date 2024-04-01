#ifndef AUTHENTICATION_SERVICE_H
#define AUTHENTICATION_SERVICE_H

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
// #include "SomeHashLibrary.h" // ʾ������������������ܵĿ�

/**
 * AuthenticationService �����ڹ����û���֤��
 * ����ע�����û����û���¼��������ܺͽ����Լ��û���Ϣ�Ĵ�ȡ��
 *
 * ע�⣺��ʾ��ʹ���ļ�ϵͳ�洢�û���Ϣ��ʵ��Ӧ�����Ƽ�ʹ�����ݿ�ϵͳ��
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

private:
    std::unordered_map<std::string, std::string> users; // �����ϣ��ӳ�䵽�û���

    /**
     * ���ļ�ϵͳ�����û���Ϣ��
     * ʵ��Ӧ��Ӧ����ʹ�õ����ݿ�����˺�����
     */
    void loadUsersFromFile();

    /**
     * ���û���Ϣ���浽�ļ�ϵͳ(���磺txt�ļ�)��
     * ע����û���Ϣ��������ʽ�洢
     */
    void saveUsersToFile();

    /**
     * ��������м��ܡ�
     *
     * @param password �������롣
     * @return ���ܺ�����롣
     */
    std::string hashPassword(const std::string& password);

    /**
     * ��������н��ܣ������Ҫ����
     *
     * @param hashedPassword ���ܺ�����롣
     * @return ���ܺ�����롣
     */
    std::string decryptPassword(const std::string& hashedPassword); // ������Ҫʵ��
};

#endif // AUTHENTICATION_SERVICE_H
