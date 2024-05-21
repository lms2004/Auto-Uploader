#ifndef DOCUMENT_SERVICE_H
#define DOCUMENT_SERVICE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <map>
#include <mysql.h>

/**
 * DocumentService 类用于管理项目文档。
 * 包括获取项目文件夹路径和名字，选择项目类型并记录。
 */
class DocumentService {
public:
    /**
     * 构造函数。
     * 初始化项目类型。
     */
    DocumentService();

    /**
     * 获取所有项目文件夹路径和名字。
     *
     * @param rootPath 项目根目录。
     */
    void loadProjects(const std::string& rootPath);

    /**
     * 选择项目类型并记录。
     */
    void selectProjectTypes();

    /**
     * 显示所有项目及其类型。
     */
    void displayProjects();

    /**
     * 将项目文件夹信息存储在数据库中。
     */
    void saveProjectsToDB();

private:
    std::vector<std::string> projectPaths; // 存储项目文件夹路径
    std::unordered_map<std::string, std::string> projectTypes; // 存储项目文件夹名称及其类型

    std::map<std::string, double> projectTypeOptions = {
        {"题库建设", 1.0},
        {"课堂表情管理", 1.5},
        {"回答问题自动加分", 1.5},
        {"项目管理系统", 1.2},
        {"课堂评价与统计", 1.0},
        {"源码自动编译与运行", 1.2},
        {"源码相似性比较", 1.2},
        {"自动判卷系统", 1.2}
    };

    /**
     * 数据库连接信息
     */
    MYSQL* connectDB();

    /**
     * 检查并创建表
     */
    void checkAndCreateTable();

    /**
     * 检查数据库字符集
     */
    void checkDatabaseCharset(MYSQL* mysql);

    /**
     * 检查表字符集
     */
    void checkTableCharset(MYSQL* mysql);

    /**
     * 修改数据库字符集
     */
    void alterDatabaseCharset(MYSQL* mysql);

    /**
     * 修改表字符集
     */
    void alterTableCharset(MYSQL* mysql);
};

#endif // DOCUMENT_SERVICE_H
