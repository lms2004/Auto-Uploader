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
    // 实现获取成绩最高的项目文件夹的逻辑
    // 在这里编写代码...

    //找到最高成绩的项目
    auto maxGradeProject = std::max_element(projects.begin(), projects.end(),
        [](const Project& a, const Project& b)
        {
            return a.grade > b.grade;
        }
    );

    //利用函数outputGitHubUploader返回最高成绩项目的文件夹路径
    return outputFolderPath( maxGradeProject->name , maxGradeProject->type); // 返回成绩最高的项目文件夹路径

}

bool GitHubUploader::uploadProjectFolderToGitHub(const std::string& folderPath) //定义上传函数，但注意有些需要修改！！！！！！
{
    // 实现上传项目文件夹到GitHub的逻辑
    // 在这里编写代码...

    // 设置 GitHub 仓库名称和访问令牌
    std::string repositoryName = "YourGitHubUsername/YourRepositoryName"; //repositoryName 变量应该设置为目标 GitHub 仓库的用户名和仓库名称，格式为 "用户名/仓库名称"，例如 `"yourusername/yourrepository"!!!!!!!!
    std::string accessToken = "YourGitHubPersonalAccessToken";//accessToken 变量应该设置为目标的GitHub 个人访问令牌，这个令牌允许你的程序访问 GitHub API 执行上传文件夹等操作!!!!!!!!!

     // 构建 GitHub API 端点 URL
    std::string apiUrl = "https://api.github.com/repos/" + repositoryName + "/contents/";

    // 构建 cURL 请求
    CURL* curl = curl_easy_init();
    if (curl) 
    {
        // 设置 URL，用于发送上传文件的请求
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());

        // 设置 HTTP 请求头，包含了个人访问令牌来进行身份验证
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: token " + accessToken).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // 设置 POST 请求
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // 设置 POST 数据
        std::string postData = "{\"path\": \"" + folderPath + "\", \"message\": \"Upload folder\", \"content\": \"\", \"branch\": \"main\"}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());  //构建了一个 JSON 数据对象，其中包含了要上传的文件夹路径、提交信息和分支信息。

        // 发送请求
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "Failed to upload folder to GitHub: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }

        // 清理资源
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    return true;

}

void GitHubUploader::deleteRemainingProjectFolders(const std::vector<Project>& projects, const std::string& topFolderPath) {
    // 实现删除未上传的其他项目文件夹的逻辑
    // 在这里编写代码...

    for (const auto& project : projects)
    {
        if (outputFolderPath(project.name, project.type) != topFolderPath)
        {
            //使用文件库删除项目剩余文件夹
            std::filesystem::remove_all(outputFolderPath(project.name , project.type));
        }
    }

}

std::string GitHubUploader::outputFolderPath(const std::string& projectName, const std::string& projectType)
{
    return "project/" + projectName + "_" + projectType;
}

