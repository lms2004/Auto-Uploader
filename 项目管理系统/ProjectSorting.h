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
    std::string name; // 项目名称
    std::string type; // 项目类型
    float grade; // 项目成绩

    Project(std::string& name, std::string& type, float& grade) :name(name), type(type), grade(grade) {} // 构造函数;

    //运算符重载,用于排序;
    bool operator>(const Project a) {

        if (type == a.type) {
            if (grade == a.grade)
                return name < a.name;
            else
                return grade > a.grade;
        }//先按类型首字母由小到大排序,再按成绩由大到小排序;
        else
            return type < a.type; //若类型不等则按类型排序;
    }
    //运算符重载,用于去重;
    bool operator==(const Project a) {
        if (type == a.type)
            return grade > a.grade;
        else
            return 0;
    }
};


class ProjectSorting {
public:
    //根据权重对成绩进行修改,并默认了权重.若未传入权重,则按默认权重进行修改;
    void adjustGradesByTypeWeight(std::vector<Project>& projects, std::unordered_map<std::string, float> typeWeights = { {"题库建设",1.0},
        {"课堂表情管理",1.5},{"回答问题自动加分",1.5},{"项目管理系统",1.2},{"课堂评价与统计",1.0},{"源码自动编译与运行",1.2},{"源码相似性比较",1.2},
        {"自动判卷系统",1.2} });
    //用于adjustGradesByTypeWeight中未发现typeweight的错误处理
    std::vector <Project>::iterator NoFindTypeWeight(std::vector <Project>::iterator&, std::unordered_map<std::string, float>&);
    //用于得到用户对于未发现typeweight问题相关处理的选择;
    //选择0处理问题(使用默认权重1.0)
    //选择1处理问题(添加该类型的权重)
    //选择2处理问题(修改项目的类型)
    int SelectNoFindTyWe();
    //用于用户选择0处理问题(使用默认权重1.0)
    void SelectNoFindTyWe0(std::vector <Project>::iterator& project, std::unordered_map<std::string, float>& typeWeights);
    //用于用户选择1处理问题(添加该类型的权重)
    void  SelectNoFindTyWe1(std::vector <Project>::iterator&, std::unordered_map<std::string, float>&);
    //用于用户选择2处理问题(修改项目的类型)
    void  SelectNoFindTyWe2(std::vector <Project>::iterator&);

    //排序函数,根据成绩对项目进行排序;(先按照类型排序,再按照成绩排序)
    void sortAndPickTopByType(std::vector<Project>& projects);

    //去重函数,保留每个项目成绩最好的;若成绩相等,则保留所有等于该成绩的项目;
    void deduplicateByTypeAndGrade(std::vector<Project>& projects);


    //修改项目成绩,若成绩范围未传入,则默认最小值为0,最大值为100
    void updateProjectGrade(Project& project, float newGrade, float min = 0, float max = 100);

    //用于当修改项目成绩时,成绩传入超出最大,最小值的设定范围;
    int RegetupdateProjectGrade();

};
#endif // PROJECT_SORTING_H

