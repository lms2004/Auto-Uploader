#include "ProjectSorting.h"
#include "GitHubUploader.h"
#include <algorithm>

std::string GitHubUploader::getTopGradedProjectFolder(const std::vector<Project>& projects) {
    // ʵ�ֻ�ȡ�ɼ���ߵ���Ŀ�ļ��е��߼�
    // �������д����...

    //�ҵ���߳ɼ�����Ŀ
    auto maxGradeProject = std::max_element(projects.begin(), projects.end(),
        [](const Project& a, const Project& b)
        {
            return a > b;
        }
    );

    //���ú���outputGitHubUploader������߳ɼ���Ŀ���ļ���·��
    return outputFolderPath( maxGradeProject->name , maxGradeProject->type); // ���سɼ���ߵ���Ŀ�ļ���·��

}

bool GitHubUploader::uploadProjectFolderToGitHub(const std::string& folderPath) {
    // ʵ���ϴ���Ŀ�ļ��е�GitHub���߼�
    // �������д����...



    return true; // ���߷�����Ӧ���߼����
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