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
//        std::cout << "�û�ע��ɹ���" << std::endl;
//    }
//    else {
//        std::cout << "�û��Ѵ��ڣ�ע��ʧ�ܡ�" << std::endl;
//    }
//
//    std::string loginUsername = "1234567890000";
//    std::string loginPassword = "password123";
//    if (authService.login(loginUsername, loginPassword)) {
//        std::cout << "�û���¼�ɹ���" << std::endl;
//    }
//    else {
//        std::cout << "�û�����������󣬵�¼ʧ�ܡ�" << std::endl;
//    }
//
//    return 0;
//}

#include "Authentication.h"

int main() {
    AuthenticationService authService;

	// �����û���Ϣ���ڴ��еĹ�ϣ��
	authService.loadUsersFromFile();

	// ע�����û�
	std::string username = "alice";
	std::string password = "password123";
	if (authService.registerUser(username, password)) {
		std::cout << "�û�ע��ɹ���" << std::endl;
	}
	else {
		std::cout << "�û��Ѵ��ڣ�ע��ʧ�ܡ�" << std::endl;
	}

    std::string loginUsername = "12345678900000";
    std::string loginPassword = "password123";
    if (authService.login(loginUsername, loginPassword)) {
        std::cout << "�û���¼�ɹ���" << std::endl;
    }
    else {
        std::cout << "�û�����������󣬵�¼ʧ�ܡ�" << std::endl;
    }

	// �����û���Ϣ���ļ������ݿ�
	authService.saveUsersToFile();

	return 0;
}