#include "ProjectSorting.h"
#include "GitHubUploader.h"


std::string GitHubUploader::getTopGradedProjectFolder(const std::vector<Project>& projects) {
    // 实现获取成绩最高的项目文件夹的逻辑
    // 在这里编写代码...
    return ""; // 返回成绩最高的项目文件夹路径
}

bool GitHubUploader::uploadProjectFolderToGitHub(const std::string& folderPath) {
    // 实现上传项目文件夹到GitHub的逻辑
    // 在这里编写代码...
    return true; // 或者返回相应的逻辑结果
}

void GitHubUploader::deleteRemainingProjectFolders(const std::vector<Project>& projects, const std::string& topFolderPath) {
    // 实现删除未上传的其他项目文件夹的逻辑
    // 在这里编写代码...
}
