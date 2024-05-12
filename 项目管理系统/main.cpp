//#include"ProjectManament.h"
//#include"ProjectSorting.h"
//#include"GitHubUploader.h"
//#include"Authentication.h"
//
//int main() {
//    AuthenticationService authService;
//
//    std::string username = "1234567890000";
//    std::string password = "password123";
//    if (authService.registerUser(username, password)) {
//        std::cout << "用户注册成功！" << std::endl;
//    }
//    else {
//        std::cout << "用户已存在，注册失败。" << std::endl;
//    }
//
//    std::string loginUsername = "1234567890000";
//    std::string loginPassword = "password123";
//    if (authService.login(loginUsername, loginPassword)) {
//        std::cout << "用户登录成功！" << std::endl;
//    }
//    else {
//        std::cout << "用户名或密码错误，登录失败。" << std::endl;
//    }
//
//    return 0;
//}

#include "Authentication.h"

int main() {
    AuthenticationService authService;

    std::string username = "12345678900000";
    std::string password = "password123";
    if (authService.registerUser(username, password)) {
        std::cout << "用户注册成功！" << std::endl;
    }
    else {
        std::cout << "用户已存在，注册失败。" << std::endl;
    }

    std::string loginUsername = "12345678900000";
    std::string loginPassword = "password123";
    if (authService.login(loginUsername, loginPassword)) {
        std::cout << "用户登录成功！" << std::endl;
    }
    else {
        std::cout << "用户名或密码错误，登录失败。" << std::endl;
    }

    return 0;
}

