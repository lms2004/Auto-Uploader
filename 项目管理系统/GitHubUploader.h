#ifndef GITHUB_UPLOADER_H
#define GITHUB_UPLOADER_H

#include <vector>
#include <string>
#include "ProjectManament.h"

/**
 * GitHubUploader �ฺ������GitHub��ص��ϴ�������
 */
class GitHubUploader
{

    bool curlConfigured; // ��ʾ�Ƿ��Ѿ����ù� cURL ��

public:
    /**
     * ��ȡ�ɼ���ߵ���Ŀ�ļ��С�
     *
     * @param projects ��Ŀ�б�
     * @return �ɼ���ߵ���Ŀ�ļ���·����
     *
     * ����ע������˺�������ÿ��Project�����а���һ��·����Ա��ָ�����Ӧ���ļ��С���Ҫ������Ŀ�б��ҳ��ɼ���ߵ���Ŀ��
     */
    std::string getTopGradedProjectFolder(const std::vector<Project>& projects);

    /**
     * �ϴ�Ŀ����Ŀ�ļ��е�GitHub��
     *
     *   @param folderPath Ҫ�ϴ�����Ŀ�ļ���·����
     *   @return �ϴ��ɹ�����true�����򷵻�false��
     *
     *    ����ע�����  ʵ�ʵ��ϴ�������Ҫʹ��GitHub API��Git���������漰�������ⲿ�����ʹ�õ������⡣
     */
    bool uploadProjectFolderToGitHub(const std::string& folderPath);

    /**
     * �ڳɹ��ϴ����Զ�ɾ��������Ŀ�ļ��С�
     *
     * @param projects ȫ����Ŀ�б�
     * @param topFolderPath �ɹ��ϴ�����Ŀ�ļ���·����
     *
     * ����ע�������Ҫȷ��ֻɾ����Щδ���ϴ�����Ŀ�ļ��У�������ɾ��֮ǰӦ�����ʵ���ȷ�Ϲ��̻򱸷ݻ��ơ�
     */
    void deleteRemainingProjectFolders(const std::vector<Project>& projects, const std::string& topFolderPath);

    static std::string outputFolderPath(const std::string& projectName, const std::string& projectType);

    //Ϊʵ���ϴ��ļ���github�ֿ⣬��Ҫʹ��cURL�������е���gitAPI������Ϊʵ��cURL����Զ�����
    void downloadFile(const std::string& url, const std::string& destination) //ʵ�ִ�ָ��URL�����ļ��������䱣����ָ����Ŀ�ĵ�
    {
        std::string command = "curl -o " + destination + " " + url;
        std::system(command.c_str());
    }

    void unzipFile(const std::string& zipFile, const std::string& destinationFolder) //ʵ�ֽ�ѹ���ص�cURLԴ����ѹ���ļ���ָ����Ŀ�ĵ��ļ���
    {
        std::string command = "unzip " + zipFile + " -d " + destinationFolder;
        std::system(command.c_str());
    }

    void runConfigure(const std::string& sourceFolder, const std::string& installPrefix) //ʵ���������ýű�������cURL��İ�װ·��
    {
        std::string command = "cd " + sourceFolder + " && ./configure --prefix=" + installPrefix;
        std::system(command.c_str());
    }

    void setupCurlLibrary(const std::string& curlSourceUrl, const std::string& curlSourceZip,
        const std::string& destinationFolder, const std::string& installPrefix)
    {
        // ����cURLԴ����
        downloadFile(curlSourceUrl, curlSourceZip);

        // ��ѹcURLԴ����
        unzipFile(curlSourceZip, destinationFolder);

        // �������ýű�
        runConfigure(destinationFolder, installPrefix);

        std::cout << "cURL source code downloaded, extracted, and configured." << std::endl;

        curlConfigured = true;//�����˷���ֵ����ʾ���óɹ�
    }

    GitHubUploader() : curlConfigured(false) {} // ���캯����ʼ��

    // ����Ƿ��Ѿ����ù� cURL ��
    bool isCurlConfigured() 
    {
        // ��� curlConfigured ��־�Ѿ�Ϊ true��ֱ�ӷ���
        if (curlConfigured)
            return true;

        // ��� cURL ͷ�ļ��Ƿ���ڣ������������Ϊ cURL ���Ѿ����ù�
        if (std::filesystem::exists("/path/to/your/project/curl/include/curl/curl.h")) 
        {
            curlConfigured = true; // ���ñ�־Ϊ true����ʾ cURL ���Ѿ����ù�
            return true;
        }

        return false; // ����ļ������ڣ����ʾ cURL ����δ���ù�
    }

    
};

#endif // GITHUB_UPLOADER_H
