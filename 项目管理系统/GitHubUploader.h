#ifndef GITHUB_UPLOADER_H
#define GITHUB_UPLOADER_H

#include <vector>
#include <string>
#include "ProjectManament.h"

/**
 * GitHubUploader 类负责处理与GitHub相关的上传操作。
 */
class GitHubUploader
{

    bool curlConfigured; // 表示是否已经配置过 cURL 库

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

    //为实现上传文件到github仓库，需要使用cURL库来进行调用gitAPI，下面为实现cURL库的自动配置
    void downloadFile(const std::string& url, const std::string& destination) //实现从指定URL下载文件，并将其保存在指定的目的地
    {
        std::string command = "curl -o " + destination + " " + url;
        std::system(command.c_str());
    }

    void unzipFile(const std::string& zipFile, const std::string& destinationFolder) //实现解压下载的cURL源代码压缩文件到指定的目的地文件夹
    {
        std::string command = "unzip " + zipFile + " -d " + destinationFolder;
        std::system(command.c_str());
    }

    void runConfigure(const std::string& sourceFolder, const std::string& installPrefix) //实现运行配置脚本，配置cURL库的安装路径
    {
        std::string command = "cd " + sourceFolder + " && ./configure --prefix=" + installPrefix;
        std::system(command.c_str());
    }

    void setupCurlLibrary(const std::string& curlSourceUrl, const std::string& curlSourceZip,
        const std::string& destinationFolder, const std::string& installPrefix)
    {
        // 下载cURL源代码
        downloadFile(curlSourceUrl, curlSourceZip);

        // 解压cURL源代码
        unzipFile(curlSourceZip, destinationFolder);

        // 运行配置脚本
        runConfigure(destinationFolder, installPrefix);

        std::cout << "cURL source code downloaded, extracted, and configured." << std::endl;

        curlConfigured = true;//增加了返回值来表示配置成功
    }

    GitHubUploader() : curlConfigured(false) {} // 构造函数初始化

    // 检查是否已经配置过 cURL 库
    bool isCurlConfigured() 
    {
        // 如果 curlConfigured 标志已经为 true，直接返回
        if (curlConfigured)
            return true;

        // 检查 cURL 头文件是否存在，如果存在则认为 cURL 库已经配置过
        if (std::filesystem::exists("/path/to/your/project/curl/include/curl/curl.h")) 
        {
            curlConfigured = true; // 设置标志为 true，表示 cURL 库已经配置过
            return true;
        }

        return false; // 如果文件不存在，则表示 cURL 库尚未配置过
    }

    
};

#endif // GITHUB_UPLOADER_H
