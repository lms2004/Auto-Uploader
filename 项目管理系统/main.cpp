#include"ProjectManament.h"
#include"ProjectSorting.h"
#include"GitHubUploader.h"
#include"Authentication.h"


int main() {
	// 创建一个AuthenticationService对象
	AuthenticationService authService;

	//创建一个GitHubUploader对象
	GitHubUploader Uploader;

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

	if (Uploader.isCurlConfigured()) 
	{
		std::cout << "cURL库已经配置过，无需重复配置。" << std::endl;
	}
	else
	{
		std::cout << "cURL库尚未配置。" << std::endl;
		// 进行 cURL 库的配置...

		// 下载、解压和配置 cURL 库--用于GitHubUploader.cpp中
		std::string curlSourceUrl = "https://curl.se/download/curl-8.7.1.zip";//cURL源代码下载链接
		std::string curlSourceZip = "curl-8.7.1.zip";//定义压缩文件名称
		std::string destinationFolder = "curl";//定义存放解压后cURL库的文件夹名称
		std::string installPrefix = "/path/to/your/project/curl"; // cURL库的安装路径（可修改）
		// 设置 cURL 库
		Uploader.setupCurlLibrary(curlSourceUrl, curlSourceZip, destinationFolder, installPrefix);
	}

	return 0;
}