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
 * DocumentService �����ڹ�����Ŀ�ĵ���
 * ������ȡ��Ŀ�ļ���·�������֣�ѡ����Ŀ���Ͳ���¼��
 */
class DocumentService {
public:
    /**
     * ���캯����
     * ��ʼ����Ŀ���͡�
     */
    DocumentService();

    /**
     * ��ȡ������Ŀ�ļ���·�������֡�
     *
     * @param rootPath ��Ŀ��Ŀ¼��
     */
    void loadProjects(const std::string& rootPath);

    /**
     * ѡ����Ŀ���Ͳ���¼��
     */
    void selectProjectTypes();

    /**
     * ��ʾ������Ŀ�������͡�
     */
    void displayProjects();

    /**
     * ����Ŀ�ļ�����Ϣ�洢�����ݿ��С�
     */
    void saveProjectsToDB();

private:
    std::vector<std::string> projectPaths; // �洢��Ŀ�ļ���·��
    std::unordered_map<std::string, std::string> projectTypes; // �洢��Ŀ�ļ������Ƽ�������

    std::map<std::string, double> projectTypeOptions = {
        {"��⽨��", 1.0},
        {"���ñ������", 1.5},
        {"�ش������Զ��ӷ�", 1.5},
        {"��Ŀ����ϵͳ", 1.2},
        {"����������ͳ��", 1.0},
        {"Դ���Զ�����������", 1.2},
        {"Դ�������ԱȽ�", 1.2},
        {"�Զ��о�ϵͳ", 1.2}
    };

    /**
     * ���ݿ�������Ϣ
     */
    MYSQL* connectDB();

    /**
     * ��鲢������
     */
    void checkAndCreateTable();

    /**
     * ������ݿ��ַ���
     */
    void checkDatabaseCharset(MYSQL* mysql);

    /**
     * �����ַ���
     */
    void checkTableCharset(MYSQL* mysql);

    /**
     * �޸����ݿ��ַ���
     */
    void alterDatabaseCharset(MYSQL* mysql);

    /**
     * �޸ı��ַ���
     */
    void alterTableCharset(MYSQL* mysql);
};

#endif // DOCUMENT_SERVICE_H
