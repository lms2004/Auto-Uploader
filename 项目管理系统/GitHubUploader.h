#ifndef GITHUB_UPLOADER_H
#define GITHUB_UPLOADER_H

#include <vector>
#include <string>
#include "ProjectManament.h"

/**
 * GitHubUploader 类负责处理与GitHub相关的上传操作。
 */
class GitHubUploader {
public:
    /**
     * 获取成绩最高的项目文件夹。
     *
     * @param projects 项目列表。
     * @return 成绩最高的项目文件夹路径。
     *
     * 开发注意事项：此函数假设每个Project对象中包含一个路径成员，指向其对应的文件夹。需要遍历项目列表，找出成绩最高的项目。
     */
    std::string getTopGradedProjectFolder(const std::vector<Project>& projects);

    /**
     * 上传目标项目文件夹到GitHub。
     *
     *   @param folderPath 要上传的项目文件夹路径。
     *   @return 上传成功返回true，否则返回false。
     *
     *    开发注意事项：  实际的上传操作需要使用GitHub API或Git命令。这可能涉及到调用外部命令或使用第三方库。
     */
    bool uploadProjectFolderToGitHub(const std::string& folderPath);

    /**
     * 在成功上传后自动删除其余项目文件夹。
     *
     * @param projects 全部项目列表。
     * @param topFolderPath 成功上传的项目文件夹路径。
     *
     * 开发注意事项：需要确保只删除那些未被上传的项目文件夹，并且在删除之前应该有适当的确认过程或备份机制。
     */
    void deleteRemainingProjectFolders(const std::vector<Project>& projects, const std::string& topFolderPath);

    static std::string outputFolderPath(const std::string& projectName, const std::string& projectType);
};

#endif // GITHUB_UPLOADER_H
