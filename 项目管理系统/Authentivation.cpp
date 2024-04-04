#include "Authentication.h"
#include <iostream>
using namespace std;

void AuthenticationService::AuthenticationService()
{

}

// 用于验证用户登录。首先检查用户是否存在，然后对比用户输入的密码经过哈希后与保存的密码是否一致，如果一致则登录成功。
bool AuthenticationService::login(const std::string& username, const std::string& password)
{
	auto it = users.find(username);
	if (it != user.end())
	{
		//用户存在，检查密码
		return it->second == hashPassword(password);
	}
	return false;
}

//用于注册新用户。首先检查用户名是否已存在，如果不存在则将用户名和经过哈希处理的密码存储到 users 哈希表中，并调用 saveUsersToFile() 将用户信息保存到文件中。
bool AuthenticationService::registerUser(const std::string& username, const std::string& password)
{
	//用户名不存在，可以注册
	if (users.find(username) == users.end())
	{
		users[username] = hashPassword(password);
		saveUsersToFile();//更新外部文件
		return true;
	}
	//用户名已存在
	return false;
}