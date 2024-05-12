#include"ProjectManament.h"
#include"ProjectSorting.h"
#include"GitHubUploader.h"
#include"Authentication.h"

int main() {
    AuthenticationService authService;

    std::string username = "l1111";
    std::string password = "password123";
    if (authService.registerUser(username, password)) {
        std::cout << "�û�ע��ɹ���" << std::endl;
    }
    else {
        std::cout << "�û��Ѵ��ڣ�ע��ʧ�ܡ�" << std::endl;
    }

    std::string loginUsername = "l1111";
    std::string loginPassword = "password123";
    if (authService.login(loginUsername, loginPassword)) {
        std::cout << "�û���¼�ɹ���" << std::endl;
    }
    else {
        std::cout << "�û�����������󣬵�¼ʧ�ܡ�" << std::endl;
    }

    return 0;
}

//int main()
//{
//	AuthenticationService authService;
//	std::string a=authService.hashPassword("123456");
//	std::string b = authService.hashPassword("123456");
//	std::cout << a << "\n";
//	std::cout << b << "\n";
//	return 0;
//}