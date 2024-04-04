#include "Authentication.h"
#include <iostream>
using namespace std;

void AuthenticationService::AuthenticationService()
{

}

// ������֤�û���¼�����ȼ���û��Ƿ���ڣ�Ȼ��Ա��û���������뾭����ϣ���뱣��������Ƿ�һ�£����һ�����¼�ɹ���
bool AuthenticationService::login(const std::string& username, const std::string& password)
{
	auto it = users.find(username);
	if (it != user.end())
	{
		//�û����ڣ��������
		return it->second == hashPassword(password);
	}
	return false;
}

//����ע�����û������ȼ���û����Ƿ��Ѵ��ڣ�������������û����;�����ϣ���������洢�� users ��ϣ���У������� saveUsersToFile() ���û���Ϣ���浽�ļ��С�
bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
{
	//�û��������ڣ�����ע��
	if (users.find(username) == users.end())
	{
		users[username] = hashPassword(password);
		saveUsersToFile();//�����ⲿ�ļ�
		return true;
	}
	//�û����Ѵ���
	return false;
}