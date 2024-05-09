#pragma once
#ifndef PROJECT_SORTING_H
#define PROJECT_SORTING_H

#include <vector>
#include <algorithm>
#include <unordered_map>
#include  <string>
/**
 * Project 类代表一个项目，包含项目的名称、类型和成绩。
 */
class Project {
public:
    std::string name; // 项目名称
    std::string type; // 项目类型
    float grade; // 项目成绩

    Project(std::string& name,std:: string& type, float& grade) :name(name), type(type), grade(grade) {} // 构造函数;

    //运算符重载,用于排序;
    bool operator>(const Project& a) const { // 应使用引用传递，并且加上 const
        if (grade == a.grade) // 使用比较操作符 '=='
            return type < a.type;
        else
            return grade > a.grade;
    }

};
/**
 * ProjectSorting 类提供了项目排序相关的功能。
 */
class ProjectSorting {
public:

    /**
   * 根据项目类型权重对每个类型的项目成绩进行修改。
   *
   * @param projects 项目列表。
   * @param typeWeights 项目类型到权重的映射。
   *
   * 开发注意事项：每个项目的成绩将乘以其类型对应的权重系数。
   */
   //根据权重对成绩进行修改,并默认了权重.若未传入权重,则按默认权重进行修改;
    void adjustGradesByTypeWeight(std::vector<Project>& projects, std::unordered_map<std::string, float> typeWeights = { {"题库建设",1.0},
        {"课堂表情管理",1.5},{"回答问题自动加分",1.5},{"项目管理系统",1.2},{"课堂评价与统计",1.0},{"源码自动编译与运行",1.2},{"源码相似性比较",1.2},
        {"自动判卷系统",1.2} });


    /**
     * 根据成绩对项目进行排序，并返回每个类型的最高成绩项目。
     *
     * @param projects 项目列表。
     * @return 排序并去重后的项目列表。
     *
     * 开发注意事项：这个方法将首先根据成绩对项目进行降序排序，然后从每个类型中选取成绩最高的项目。
     */
     //排序函数,根据成绩对项目进行排序;
    void sortAndPickTopByType(std::vector<Project>& projects);


    /**
     * 根据项目类型和成绩去重。
     *
     * @param projects 项目列表。
     * @return 去重后的项目列表，每个类型的项目只保留成绩最高的一个。
     *
     * 开发注意事项：对于每个项目类型，只保留成绩最高的项目。
     */
     //注意使用该函数前请执行排序函数;去重函数,保留每个项目成绩最好的;若成绩相等,则保留全部;
    void deduplicateByTypeAndGrade(std::vector<Project>& projects);


    /**
    * 修改项目成绩。
    *
    * @param project 项目对象引用。
    * @param newGrade 新的成绩。
    *
    * 开发时需确保成绩在合理范围内（例如0到100）。
    */
    //修改项目成绩,若成绩范围未传入,则默认最小值为0,最大值为100
    void updateProjectGrade(Project& project, float newGrade, float min = 0, float max = 100);

};
#endif // PROJECT_SORTING_H

