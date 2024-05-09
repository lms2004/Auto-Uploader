#pragma once
#ifndef PROJECT_SORTING_H
#define PROJECT_SORTING_H

#include <vector>
#include <algorithm>
#include <unordered_map>
#include  <string>
/**
 * Project �����һ����Ŀ��������Ŀ�����ơ����ͺͳɼ���
 */
class Project {
public:
    std::string name; // ��Ŀ����
    std::string type; // ��Ŀ����
    float grade; // ��Ŀ�ɼ�

    Project(std::string& name,std:: string& type, float& grade) :name(name), type(type), grade(grade) {} // ���캯��;

    //���������,��������;
    bool operator>(const Project& a) const { // Ӧʹ�����ô��ݣ����Ҽ��� const
        if (grade == a.grade) // ʹ�ñȽϲ����� '=='
            return type < a.type;
        else
            return grade > a.grade;
    }

};
/**
 * ProjectSorting ���ṩ����Ŀ������صĹ��ܡ�
 */
class ProjectSorting {
public:

    /**
   * ������Ŀ����Ȩ�ض�ÿ�����͵���Ŀ�ɼ������޸ġ�
   *
   * @param projects ��Ŀ�б�
   * @param typeWeights ��Ŀ���͵�Ȩ�ص�ӳ�䡣
   *
   * ����ע�����ÿ����Ŀ�ĳɼ������������Ͷ�Ӧ��Ȩ��ϵ����
   */
   //����Ȩ�ضԳɼ������޸�,��Ĭ����Ȩ��.��δ����Ȩ��,��Ĭ��Ȩ�ؽ����޸�;
    void adjustGradesByTypeWeight(std::vector<Project>& projects, std::unordered_map<std::string, float> typeWeights = { {"��⽨��",1.0},
        {"���ñ������",1.5},{"�ش������Զ��ӷ�",1.5},{"��Ŀ����ϵͳ",1.2},{"����������ͳ��",1.0},{"Դ���Զ�����������",1.2},{"Դ�������ԱȽ�",1.2},
        {"�Զ��о�ϵͳ",1.2} });


    /**
     * ���ݳɼ�����Ŀ�������򣬲�����ÿ�����͵���߳ɼ���Ŀ��
     *
     * @param projects ��Ŀ�б�
     * @return ����ȥ�غ����Ŀ�б�
     *
     * ����ע�����������������ȸ��ݳɼ�����Ŀ���н�������Ȼ���ÿ��������ѡȡ�ɼ���ߵ���Ŀ��
     */
     //������,���ݳɼ�����Ŀ��������;
    void sortAndPickTopByType(std::vector<Project>& projects);


    /**
     * ������Ŀ���ͺͳɼ�ȥ�ء�
     *
     * @param projects ��Ŀ�б�
     * @return ȥ�غ����Ŀ�б�ÿ�����͵���Ŀֻ�����ɼ���ߵ�һ����
     *
     * ����ע���������ÿ����Ŀ���ͣ�ֻ�����ɼ���ߵ���Ŀ��
     */
     //ע��ʹ�øú���ǰ��ִ��������;ȥ�غ���,����ÿ����Ŀ�ɼ���õ�;���ɼ����,����ȫ��;
    void deduplicateByTypeAndGrade(std::vector<Project>& projects);


    /**
    * �޸���Ŀ�ɼ���
    *
    * @param project ��Ŀ�������á�
    * @param newGrade �µĳɼ���
    *
    * ����ʱ��ȷ���ɼ��ں���Χ�ڣ�����0��100����
    */
    //�޸���Ŀ�ɼ�,���ɼ���Χδ����,��Ĭ����СֵΪ0,���ֵΪ100
    void updateProjectGrade(Project& project, float newGrade, float min = 0, float max = 100);

};
#endif // PROJECT_SORTING_H

