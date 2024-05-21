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
            std::cout << "ѡ����Ŀ���ͣ�" << projectName << "\n";
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
                std::cout << "��Ч������ѡ�������ԡ�\n";
            }
        }
    }
}

void DocumentService::displayProjects()
{
    for (const auto& [projectName, projectType] : projectTypes) {
        std::cout << "��Ŀ���ƣ�" << projectName << "����Ŀ���ͣ�" << projectType << "\n";
    }
}

void DocumentService::saveProjectsToDB()
{
    MYSQL* mysql = connectDB();
    if (!mysql) return;

    checkAndCreateTable();

    mysql_query(mysql, "DELETE FROM projects;");

    for (const auto& [projectName, projectType] : projectTypes) {
        // ȷ���ַ�����UTF-8����
        std::string utf8ProjectName = projectName;
        std::string utf8ProjectType = projectType;

        // ת���ַ���
        char escapedProjectName[1024];
        char escapedProjectType[1024];
        mysql_real_escape_string(mysql, escapedProjectName, utf8ProjectName.c_str(), utf8ProjectName.length());
        mysql_real_escape_string(mysql, escapedProjectType, utf8ProjectType.c_str(), utf8ProjectType.length());

        std::string query = "INSERT INTO projects (project_name, project_type) VALUES ('" + std::string(escapedProjectName) + "', '" + std::string(escapedProjectType) + "');";
        if (mysql_query(mysql, query.c_str()) != 0) {
            printf("��������ʧ��: %s\n", mysql_error(mysql));
        }
    }

    mysql_close(mysql);
}

MYSQL* DocumentService::connectDB()
{
    MYSQL* mysql = mysql_init(nullptr);
    if (mysql == nullptr) {
        printf("mysql_init() ʧ��\n");
        return nullptr;
    }

    if (mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "data", 3306, nullptr, 0) == nullptr) {
        printf("�������ݿ�ʧ��: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return nullptr;
    }

    // �����ַ���Ϊutf8mb4
    if (mysql_set_character_set(mysql, "utf8mb4")) {
        printf("�����ַ���ʧ��: %s\n", mysql_error(mysql));
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
        printf("������ݿ��ַ���ʧ��: %s\n", mysql_error(mysql));
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
        printf("������ݿ��������ʧ��: %s\n", mysql_error(mysql));
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
        printf("�����ַ���ʧ��: %s\n", mysql_error(mysql));
    }
}

void DocumentService::alterDatabaseCharset(MYSQL* mysql)
{
    if (mysql_query(mysql, "ALTER DATABASE data CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;") != 0) {
        printf("�޸����ݿ��ַ���ʧ��: %s\n", mysql_error(mysql));
    }
}

void DocumentService::alterTableCharset(MYSQL* mysql)
{
    if (mysql_query(mysql, "ALTER TABLE projects CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;") != 0) {
        printf("�޸ı��ַ���ʧ��: %s\n", mysql_error(mysql));
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
        printf("������ʧ��: %s\n", mysql_error(mysql));
    }

    mysql_close(mysql);
}
