#include <iostream>
#include "AuthenticationService.h"
#include "DocumentService.h"

int main() {
    AuthenticationService authService;

    // 注册新用户
    std::string username, password;
    std::cout << "注册新用户\n";
    std::cout << "用户名: ";
    std::cin >> username;
    std::cout << "密码: ";
    std::cin >> password;

    if (authService.registerUser(username, password)) {
        std::cout << "用户注册成功\n";
    }
    else {
        std::cout << "用户注册失败，用户名已存在\n";
    }

    // 用户登录
    std::cout << "用户登录\n";
    std::cout << "用户名: ";
    std::cin >> username;
    std::cout << "密码: ";
    std::cin >> password;

    if (authService.login(username, password)) {
        std::cout << "登录成功\n";
    }
    else {
        std::cout << "登录失败，用户名或密码错误\n";
    }


    DocumentService docService;

    std::string rootPath;
    std::cout << "请输入项目根目录路径：";
    std::cin >> rootPath;

    docService.loadProjects(rootPath);
    docService.selectProjectTypes();
    docService.displayProjects();
    docService.saveProjectsToDB();



    return 0;
}
