#ifndef GITHUB_UPLOADER_H
#define GITHUB_UPLOADER_H

#include <vector>
#include <string>
#include "ProjectManament.h"

/**
 * GitHubUploader �ฺ������GitHub��ص��ϴ�������
 */
class GitHubUploader {
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
};

#endif // GITHUB_UPLOADER_H
