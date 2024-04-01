#ifndef PROJECT_SORTING_H
#define PROJECT_SORTING_H

#include <vector>
#include <algorithm>
#include <unordered_map>
#include<string>

/**
 * Project 类代表一个项目，包含项目的名称、类型和成绩。
 */
class Project {
public:
    std::string name; // 项目名称
    std::string type; // 项目类型
    float grade; // 项目成绩

    // 构造函数
    Project(const std::string& name, const std::string& type, float grade)
        : name(name), type(type), grade(grade) {}
};

/**
 * ProjectSorting 类提供了项目排序相关的功能。
 */
class ProjectSorting {
public:
    /**
     * 根据成绩对项目进行排序，并返回每个类型的最高成绩项目。
     *
     * @param projects 项目列表。
     * @return 排序并去重后的项目列表。
     *
     * 开发注意事项：这个方法将首先根据成绩对项目进行降序排序，然后从每个类型中选取成绩最高的项目。
     */
    std::vector<Project> sortAndPickTopByType(const std::vector<Project>& projects);

    /**
     * 根据项目类型和成绩去重。
     *
     * @param projects 项目列表。
     * @return 去重后的项目列表，每个类型的项目只保留成绩最高的一个。
     *
     * 开发注意事项：对于每个项目类型，只保留成绩最高的项目。
     */
    std::vector<Project> deduplicateByTypeAndGrade(const std::vector<Project>& projects);

    /**
     * 根据项目类型权重对每个类型的项目成绩进行修改。
     *
     * @param projects 项目列表。
     * @param typeWeights 项目类型到权重的映射。
     *
     * 开发注意事项：每个项目的成绩将乘以其类型对应的权重系数。
     */
    void adjustGradesByTypeWeight(std::vector<Project>& projects, const std::unordered_map<std::string, float>& typeWeights);
};

std::vector<Project> ProjectSorting::sortAndPickTopByType(const std::vector<Project>& projects) {
    // 实现排序和选择逻辑
}

std::vector<Project> ProjectSorting::deduplicateByTypeAndGrade(const std::vector<Project>& projects) {
    // 实现去重逻辑
}

void ProjectSorting::adjustGradesByTypeWeight(std::vector<Project>& projects, const std::unordered_map<std::string, float>& typeWeights) {
    // 实现成绩调整逻辑
}

#endif // PROJECT_SORTING_H
