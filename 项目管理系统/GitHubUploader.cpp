#include "ProjectSorting.h"
#include "GitHubUploader.h"
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <curl/curl.h>


std::string GitHubUploader::getTopGradedProjectFolder(const std::vector<Project>& projects)
{
    // ʵ�ֻ�ȡ�ɼ���ߵ���Ŀ�ļ��е��߼�
    // �������д����...

    //�ҵ���߳ɼ�����Ŀ
    auto maxGradeProject = std::max_element(projects.begin(), projects.end(),
        [](const Project& a, const Project& b)
        {
            return a.grade > b.grade;
        }
    );

    //���ú���outputGitHubUploader������߳ɼ���Ŀ���ļ���·��
    return outputFolderPath( maxGradeProject->name , maxGradeProject->type); // ���سɼ���ߵ���Ŀ�ļ���·��

}

bool GitHubUploader::uploadProjectFolderToGitHub(const std::string& folderPath) //�����ϴ���������ע����Щ��Ҫ�޸ģ�����������
{
    // ʵ���ϴ���Ŀ�ļ��е�GitHub���߼�
    // �������д����...

    // ���� GitHub �ֿ����ƺͷ�������
    std::string repositoryName = "YourGitHubUsername/YourRepositoryName"; //repositoryName ����Ӧ������ΪĿ�� GitHub �ֿ���û����Ͳֿ����ƣ���ʽΪ "�û���/�ֿ�����"������ `"yourusername/yourrepository"!!!!!!!!
    std::string accessToken = "YourGitHubPersonalAccessToken";//accessToken ����Ӧ������ΪĿ���GitHub ���˷������ƣ��������������ĳ������ GitHub API ִ���ϴ��ļ��еȲ���!!!!!!!!!

     // ���� GitHub API �˵� URL
    std::string apiUrl = "https://api.github.com/repos/" + repositoryName + "/contents/";

    // ���� cURL ����
    CURL* curl = curl_easy_init();
    if (curl) 
    {
        // ���� URL�����ڷ����ϴ��ļ�������
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());

        // ���� HTTP ����ͷ�������˸��˷������������������֤
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: token " + accessToken).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // ���� POST ����
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // ���� POST ����
        std::string postData = "{\"path\": \"" + folderPath + "\", \"message\": \"Upload folder\", \"content\": \"\", \"branch\": \"main\"}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());  //������һ�� JSON ���ݶ������а�����Ҫ�ϴ����ļ���·�����ύ��Ϣ�ͷ�֧��Ϣ��

        // ��������
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "Failed to upload folder to GitHub: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }

        // ������Դ
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    return true;

}

void GitHubUploader::deleteRemainingProjectFolders(const std::vector<Project>& projects, const std::string& topFolderPath) {
    // ʵ��ɾ��δ�ϴ���������Ŀ�ļ��е��߼�
    // �������д����...

    for (const auto& project : projects)
    {
        if (outputFolderPath(project.name, project.type) != topFolderPath)
        {
            //ʹ���ļ���ɾ����Ŀʣ���ļ���
            std::filesystem::remove_all(outputFolderPath(project.name , project.type));
        }
    }

}

std::string GitHubUploader::outputFolderPath(const std::string& projectName, const std::string& projectType)
{
    return "project/" + projectName + "_" + projectType;
}

