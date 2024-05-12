#include"ProjectManament.h"
#include"ProjectSorting.h"
#include"GitHubUploader.h"
#include"Authentication.h"


int main() {
	// ����һ��AuthenticationService����
	AuthenticationService authService;

	//����һ��GitHubUploader����
	GitHubUploader Uploader;

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

	// ��֤�û���¼
	std::string loginUsername = "alice";
	std::string loginPassword = "password123";
	if (authService.login(loginUsername, loginPassword)) {
		std::cout << "�û���¼�ɹ���" << std::endl;
	}
	else {
		std::cout << "�û�����������󣬵�¼ʧ�ܡ�" << std::endl;
	}

	// �����û���Ϣ���ļ������ݿ�
	authService.saveUsersToFile();

	if (Uploader.isCurlConfigured()) 
	{
		std::cout << "cURL���Ѿ����ù��������ظ����á�" << std::endl;
	}
	else
	{
		std::cout << "cURL����δ���á�" << std::endl;
		// ���� cURL �������...

		// ���ء���ѹ������ cURL ��--����GitHubUploader.cpp��
		std::string curlSourceUrl = "https://curl.se/download/curl-8.7.1.zip";//cURLԴ������������
		std::string curlSourceZip = "curl-8.7.1.zip";//����ѹ���ļ�����
		std::string destinationFolder = "curl";//�����Ž�ѹ��cURL����ļ�������
		std::string installPrefix = "/path/to/your/project/curl"; // cURL��İ�װ·�������޸ģ�
		// ���� cURL ��
		Uploader.setupCurlLibrary(curlSourceUrl, curlSourceZip, destinationFolder, installPrefix);
	}

	return 0;
}