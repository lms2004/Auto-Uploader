#pragma once
#ifndef PROJECT_SORTING_H
#define PROJECT_SORTING_H
#include<iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include<string>
class Project {
public:
    std::string name; // ��Ŀ����
    std::string type; // ��Ŀ����
    float grade; // ��Ŀ�ɼ�

    Project(std::string& name, std::string& type, float& grade) :name(name), type(type), grade(grade) {} // ���캯��;

    //���������,��������;
    bool operator>(const Project a) {

        if (type == a.type) {
            if (grade == a.grade)
                return name < a.name;
            else
                return grade > a.grade;
        }//�Ȱ���������ĸ��С��������,�ٰ��ɼ��ɴ�С����;
        else
            return type < a.type; //�����Ͳ�������������;
    }
    //���������,����ȥ��;
    bool operator==(const Project a) {
        if (type == a.type)
            return grade > a.grade;
        else
            return 0;
    }
};


class ProjectSorting {
public:
    //����Ȩ�ضԳɼ������޸�,��Ĭ����Ȩ��.��δ����Ȩ��,��Ĭ��Ȩ�ؽ����޸�;
    void adjustGradesByTypeWeight(std::vector<Project>& projects, std::unordered_map<std::string, float> typeWeights = { {"��⽨��",1.0},
        {"���ñ������",1.5},{"�ش������Զ��ӷ�",1.5},{"��Ŀ����ϵͳ",1.2},{"����������ͳ��",1.0},{"Դ���Զ�����������",1.2},{"Դ�������ԱȽ�",1.2},
        {"�Զ��о�ϵͳ",1.2} });
    //����adjustGradesByTypeWeight��δ����typeweight�Ĵ�����
    std::vector <Project>::iterator NoFindTypeWeight(std::vector <Project>::iterator&, std::unordered_map<std::string, float>&);
    //���ڵõ��û�����δ����typeweight������ش����ѡ��;
    //ѡ��0��������(ʹ��Ĭ��Ȩ��1.0)
    //ѡ��1��������(��Ӹ����͵�Ȩ��)
    //ѡ��2��������(�޸���Ŀ������)
    int SelectNoFindTyWe();
    //�����û�ѡ��0��������(ʹ��Ĭ��Ȩ��1.0)
    void SelectNoFindTyWe0(std::vector <Project>::iterator& project, std::unordered_map<std::string, float>& typeWeights);
    //�����û�ѡ��1��������(��Ӹ����͵�Ȩ��)
    void  SelectNoFindTyWe1(std::vector <Project>::iterator&, std::unordered_map<std::string, float>&);
    //�����û�ѡ��2��������(�޸���Ŀ������)
    void  SelectNoFindTyWe2(std::vector <Project>::iterator&);

    //������,���ݳɼ�����Ŀ��������;(�Ȱ�����������,�ٰ��ճɼ�����)
    void sortAndPickTopByType(std::vector<Project>& projects);

    //ȥ�غ���,����ÿ����Ŀ�ɼ���õ�;���ɼ����,�������е��ڸóɼ�����Ŀ;
    void deduplicateByTypeAndGrade(std::vector<Project>& projects);


    //�޸���Ŀ�ɼ�,���ɼ���Χδ����,��Ĭ����СֵΪ0,���ֵΪ100
    void updateProjectGrade(Project& project, float newGrade, float min = 0, float max = 100);

    //���ڵ��޸���Ŀ�ɼ�ʱ,�ɼ����볬�����,��Сֵ���趨��Χ;
    int RegetupdateProjectGrade();

};
#endif // PROJECT_SORTING_H

