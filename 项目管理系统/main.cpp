#include <iostream>
#include "AuthenticationService.h"
#include "DocumentService.h"

int main() {
    AuthenticationService authService;

    // ע�����û�
    std::string username, password;
    std::cout << "ע�����û�\n";
    std::cout << "�û���: ";
    std::cin >> username;
    std::cout << "����: ";
    std::cin >> password;

    if (authService.registerUser(username, password)) {
        std::cout << "�û�ע��ɹ�\n";
    }
    else {
        std::cout << "�û�ע��ʧ�ܣ��û����Ѵ���\n";
    }

    // �û���¼
    std::cout << "�û���¼\n";
    std::cout << "�û���: ";
    std::cin >> username;
    std::cout << "����: ";
    std::cin >> password;

    if (authService.login(username, password)) {
        std::cout << "��¼�ɹ�\n";
    }
    else {
        std::cout << "��¼ʧ�ܣ��û������������\n";
    }


    DocumentService docService;

    std::string rootPath;
    std::cout << "��������Ŀ��Ŀ¼·����";
    std::cin >> rootPath;

    docService.loadProjects(rootPath);
    docService.selectProjectTypes();
    docService.displayProjects();
    docService.saveProjectsToDB();



    return 0;
}
