#ifndef PROJECT_MANAGEMENT_H
#define PROJECT_MANAGEMENT_H

#include <string>
#include <filesystem> // C++17 文件系统库
#include <iostream>
#include <fstream>

/**
 * Project 类表示一个项目，包含项目的基本信息。
 */
class Project {
public:
    std::string name; // 项目名称
    std::string projectName; // 对应的项目名
    float grade; // 项目成绩

    Project(const std::string& name, const std::string& projectName, float grade)
        : name(name), projectName(projectName), grade(grade) {}
};

/**
 * ProjectManagement 类提供了项目管理功能，包括项目文件的浅拷贝、删除项目文件夹以及成绩管理。
 */
class ProjectManagement {
public:
    /**
     * 浅拷贝项目文件夹。
     *
     * @param sourcePath 源文件夹路径。
     * @param destinationPath 目标项目文件夹路径。
     * @return 成功返回true，否则返回false。
     *
     * 注意：这里的“浅拷贝”指的是仅拷贝文件夹结构和文件的引用，而非文件内容。
     * 开发时需确保源路径和目标路径有效，且目标路径不存在同名文件夹。
     */
    bool shallowCopyProjectFolder(const std::filesystem::path& sourcePath, const std::filesystem::path& destinationPath);

    /**
     * 删除项目文件夹。
     *
     * @param path 项目文件夹路径。
     * @return 成功返回true，否则返回false。
     *
     * 开发时需注意异常处理，确保路径有效且指向一个文件夹。
     */
    bool deleteProjectFolder(const std::filesystem::path& path);
};

#endif // PROJECT_MANAGEMENT_H
