#include "ProjectSorting.h"
#include "GitHubUploader.h"
#include <algorithm>

std::string GitHubUploader::getTopGradedProjectFolder(const std::vector<Project>& projects) {
    // 实现获取成绩最高的项目文件夹的逻辑
    // 在这里编写代码...

    //找到最高成绩的项目
    auto maxGradeProject = std::max_element(projects.begin(), projects.end(),
        [](const Project& a, const Project& b)
        {
            return a > b;
        }
    );

    //利用函数outputGitHubUploader返回最高成绩项目的文件夹路径
    return outputFolderPath( maxGradeProject->name , maxGradeProject->type); // 返回成绩最高的项目文件夹路径

}

bool GitHubUploader::uploadProjectFolderToGitHub(const std::string& folderPath) {
    // 实现上传项目文件夹到GitHub的逻辑
    // 在这里编写代码...



    return true; // 或者返回相应的逻辑结果
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