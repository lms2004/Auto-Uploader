#include"ProjectManament.h"
#include"ProjectSorting.h"
#include"GitHubUploader.h"
#include"Authentication.h"

int main() {
    // ������֤����ʵ��
    AuthenticationService authService;

    // ע�����û�
    std::string username = "tim";
    std::string password = "password123";
    if (authService.registerUser(username, password)) {
        std::cout << "�û�ע��ɹ���" << std::endl;
    }
    else {
        std::cout << "�û��Ѵ��ڣ�ע��ʧ�ܡ�" << std::endl;
    }

    // �û���¼
    std::string loginUsername = "tim";
    std::string loginPassword = "password123";
    if (authService.login(loginUsername, loginPassword)) {
        std::cout << "�û���¼�ɹ���" << std::endl;
    }
    else {
        std::cout << "�û�����������󣬵�¼ʧ�ܡ�" << std::endl;
    }

    return 0;
}