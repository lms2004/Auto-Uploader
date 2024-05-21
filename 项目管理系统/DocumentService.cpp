#include "DocumentService.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

DocumentService::DocumentService() {}

void DocumentService::loadProjects(const std::string& rootPath)
{
    try {
        for (const auto& entry : fs::directory_iterator(rootPath)) {
            if (entry.is_directory()) {
                projectPaths.push_back(entry.path().string());
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
    }
}

void DocumentService::selectProjectTypes()
{
    for (const auto& projectPath : projectPaths) {
        std::string projectName = fs::path(projectPath).filename().string();
        bool validSelection = false;

        while (!validSelection) {
            std::cout << "选择项目类型：" << projectName << "\n";
            for (const auto& [type, weight] : projectTypeOptions) {
                std::cout << type << " (" << weight << ")\n";
            }

            std::string selectedType;
            std::cin >> selectedType;

            if (projectTypeOptions.find(selectedType) != projectTypeOptions.end()) {
                projectTypes[projectName] = selectedType;
                validSelection = true;
            }
            else {
                std::cout << "无效的类型选择，请重试。\n";
            }
        }
    }
}

void DocumentService::displayProjects()
{
    for (const auto& [projectName, projectType] : projectTypes) {
        std::cout << "项目名称：" << projectName << "，项目类型：" << projectType << "\n";
    }
}

void DocumentService::saveProjectsToDB()
{
    MYSQL* mysql = connectDB();
    if (!mysql) return;

    checkAndCreateTable();

    mysql_query(mysql, "DELETE FROM projects;");

    for (const auto& [projectName, projectType] : projectTypes) {
        // 确保字符串是UTF-8编码
        std::string utf8ProjectName = projectName;
        std::string utf8ProjectType = projectType;

        // 转义字符串
        char escapedProjectName[1024];
        char escapedProjectType[1024];
        mysql_real_escape_string(mysql, escapedProjectName, utf8ProjectName.c_str(), utf8ProjectName.length());
        mysql_real_escape_string(mysql, escapedProjectType, utf8ProjectType.c_str(), utf8ProjectType.length());

        std::string query = "INSERT INTO projects (project_name, project_type) VALUES ('" + std::string(escapedProjectName) + "', '" + std::string(escapedProjectType) + "');";
        if (mysql_query(mysql, query.c_str()) != 0) {
            printf("插入数据失败: %s\n", mysql_error(mysql));
        }
    }

    mysql_close(mysql);
}

MYSQL* DocumentService::connectDB()
{
    MYSQL* mysql = mysql_init(nullptr);
    if (mysql == nullptr) {
        printf("mysql_init() 失败\n");
        return nullptr;
    }

    if (mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "data", 3306, nullptr, 0) == nullptr) {
        printf("连接数据库失败: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return nullptr;
    }

    // 设置字符集为utf8mb4
    if (mysql_set_character_set(mysql, "utf8mb4")) {
        printf("设置字符集失败: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return nullptr;
    }

    return mysql;
}

void DocumentService::checkDatabaseCharset(MYSQL* mysql)
{
    if (mysql_query(mysql, "SHOW VARIABLES LIKE 'character_set_database';") == 0) {
        MYSQL_RES* res = mysql_store_result(mysql);
        if (res) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr) {
                printf("%s: %s\n", row[0], row[1]);
            }
            mysql_free_result(res);
        }
    }
    else {
        printf("检查数据库字符集失败: %s\n", mysql_error(mysql));
    }

    if (mysql_query(mysql, "SHOW VARIABLES LIKE 'collation_database';") == 0) {
        MYSQL_RES* res = mysql_store_result(mysql);
        if (res) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr) {
                printf("%s: %s\n", row[0], row[1]);
            }
            mysql_free_result(res);
        }
    }
    else {
        printf("检查数据库排序规则失败: %s\n", mysql_error(mysql));
    }
}

void DocumentService::checkTableCharset(MYSQL* mysql)
{
    if (mysql_query(mysql, "SHOW CREATE TABLE projects;") == 0) {
        MYSQL_RES* res = mysql_store_result(mysql);
        if (res) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr) {
                printf("%s\n", row[1]);
            }
            mysql_free_result(res);
        }
    }
    else {
        printf("检查表字符集失败: %s\n", mysql_error(mysql));
    }
}

void DocumentService::alterDatabaseCharset(MYSQL* mysql)
{
    if (mysql_query(mysql, "ALTER DATABASE data CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;") != 0) {
        printf("修改数据库字符集失败: %s\n", mysql_error(mysql));
    }
}

void DocumentService::alterTableCharset(MYSQL* mysql)
{
    if (mysql_query(mysql, "ALTER TABLE projects CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;") != 0) {
        printf("修改表字符集失败: %s\n", mysql_error(mysql));
    }
}

void DocumentService::checkAndCreateTable()
{
    MYSQL* mysql = connectDB();
    if (!mysql) return;

    checkDatabaseCharset(mysql);
    checkTableCharset(mysql);

    alterDatabaseCharset(mysql);
    alterTableCharset(mysql);

    const char* createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS projects (
            project_name VARCHAR(255) COLLATE utf8mb4_general_ci NOT NULL,
            project_type VARCHAR(255) COLLATE utf8mb4_general_ci NOT NULL
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
    )";
    if (mysql_query(mysql, createTableQuery) != 0) {
        printf("创建表失败: %s\n", mysql_error(mysql));
    }

    mysql_close(mysql);
}
