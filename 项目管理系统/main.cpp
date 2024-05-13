#include"ProjectManament.h"
#include"ProjectSorting.h"
#include"GitHubUploader.h"
#include"Authentication.h"

int main() {
    // 创建认证服务实例
    AuthenticationService authService;

    // 注册新用户
    std::string username = "tim";
    std::string password = "password123";
    if (authService.registerUser(username, password)) {
        std::cout << "用户注册成功！" << std::endl;
    }
    else {
        std::cout << "用户已存在，注册失败。" << std::endl;
    }

    // 用户登录
    std::string loginUsername = "tim";
    std::string loginPassword = "password123";
    if (authService.login(loginUsername, loginPassword)) {
        std::cout << "用户登录成功！" << std::endl;
    }
    else {
        std::cout << "用户名或密码错误，登录失败。" << std::endl;
    }

    return 0;
}