#ifndef PROJECT_SORTING_H
#define PROJECT_SORTING_H

#include <vector>
#include <algorithm>
#include <unordered_map>
#include<string>

/**
 * Project �����һ����Ŀ��������Ŀ�����ơ����ͺͳɼ���
 */
class Project {
public:
    std::string name; // ��Ŀ����
    std::string type; // ��Ŀ����
    float grade; // ��Ŀ�ɼ�

    // ���캯��
    Project(const std::string& name, const std::string& type, float grade)
        : name(name), type(type), grade(grade) {}
};

/**
 * ProjectSorting ���ṩ����Ŀ������صĹ��ܡ�
 */
class ProjectSorting {
public:
    /**
     * ���ݳɼ�����Ŀ�������򣬲�����ÿ�����͵���߳ɼ���Ŀ��
     *
     * @param projects ��Ŀ�б�
     * @return ����ȥ�غ����Ŀ�б�
     *
     * ����ע�����������������ȸ��ݳɼ�����Ŀ���н�������Ȼ���ÿ��������ѡȡ�ɼ���ߵ���Ŀ��
     */
    std::vector<Project> sortAndPickTopByType(const std::vector<Project>& projects);

    /**
     * ������Ŀ���ͺͳɼ�ȥ�ء�
     *
     * @param projects ��Ŀ�б�
     * @return ȥ�غ����Ŀ�б�ÿ�����͵���Ŀֻ�����ɼ���ߵ�һ����
     *
     * ����ע���������ÿ����Ŀ���ͣ�ֻ�����ɼ���ߵ���Ŀ��
     */
    std::vector<Project> deduplicateByTypeAndGrade(const std::vector<Project>& projects);

    /**
     * ������Ŀ����Ȩ�ض�ÿ�����͵���Ŀ�ɼ������޸ġ�
     *
     * @param projects ��Ŀ�б�
     * @param typeWeights ��Ŀ���͵�Ȩ�ص�ӳ�䡣
     *
     * ����ע�����ÿ����Ŀ�ĳɼ������������Ͷ�Ӧ��Ȩ��ϵ����
     */
    void adjustGradesByTypeWeight(std::vector<Project>& projects, const std::unordered_map<std::string, float>& typeWeights);
};

std::vector<Project> ProjectSorting::sortAndPickTopByType(const std::vector<Project>& projects) {
    // ʵ�������ѡ���߼�
}

std::vector<Project> ProjectSorting::deduplicateByTypeAndGrade(const std::vector<Project>& projects) {
    // ʵ��ȥ���߼�
}

void ProjectSorting::adjustGradesByTypeWeight(std::vector<Project>& projects, const std::unordered_map<std::string, float>& typeWeights) {
    // ʵ�ֳɼ������߼�
}

#endif // PROJECT_SORTING_H
