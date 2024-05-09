#include"ProjectManament.h"
#include"ProjectSorting.h"
#include"GitHubUploader.h"
#include"Authentication.h"


int main() {
	// 创建一个AuthenticationService对象
	AuthenticationService authService;

	// 加载用户信息到内存中的哈希表
	authService.loadUsersFromFile();

	// 注册新用户
	std::string username = "alice";
	std::string password = "password123";
	if (authService.registerUser(username, password)) {
		std::cout << "用户注册成功！" << std::endl;
	}
	else {
		std::cout << "用户已存在，注册失败。" << std::endl;
	}

	// 验证用户登录
	std::string loginUsername = "alice";
	std::string loginPassword = "password123";
	if (authService.login(loginUsername, loginPassword)) {
		std::cout << "用户登录成功！" << std::endl;
	}
	else {
		std::cout << "用户名或密码错误，登录失败。" << std::endl;
	}

	// 保存用户信息到文件和数据库
	authService.saveUsersToFile();

	return 0;
}