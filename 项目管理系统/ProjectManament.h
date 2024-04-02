#ifndef PROJECT_MANAGEMENT_H
#define PROJECT_MANAGEMENT_H

#include <string>
#include <filesystem> // C++17 �ļ�ϵͳ��
#include <iostream>
#include <fstream>

/**
 * Project ���ʾһ����Ŀ��������Ŀ�Ļ�����Ϣ��
 */
class Project {
public:
    std::string name; // ��Ŀ����
    std::string projectName; // ��Ӧ����Ŀ��
    float grade; // ��Ŀ�ɼ�

    Project(const std::string& name, const std::string& projectName, float grade)
        : name(name), projectName(projectName), grade(grade) {}
};

/**
 * ProjectManagement ���ṩ����Ŀ�����ܣ�������Ŀ�ļ���ǳ������ɾ����Ŀ�ļ����Լ��ɼ�����
 */
class ProjectManagement {
public:
    /**
     * ǳ������Ŀ�ļ��С�
     *
     * @param sourcePath Դ�ļ���·����
     * @param destinationPath Ŀ����Ŀ�ļ���·����
     * @return �ɹ�����true�����򷵻�false��
     *
     * ע�⣺����ġ�ǳ������ָ���ǽ������ļ��нṹ���ļ������ã������ļ����ݡ�
     * ����ʱ��ȷ��Դ·����Ŀ��·����Ч����Ŀ��·��������ͬ���ļ��С�
     */
    bool shallowCopyProjectFolder(const std::filesystem::path& sourcePath, const std::filesystem::path& destinationPath);

    /**
     * ɾ����Ŀ�ļ��С�
     *
     * @param path ��Ŀ�ļ���·����
     * @return �ɹ�����true�����򷵻�false��
     *
     * ����ʱ��ע���쳣����ȷ��·����Ч��ָ��һ���ļ��С�
     */
    bool deleteProjectFolder(const std::filesystem::path& path);
};

#endif // PROJECT_MANAGEMENT_H
