#include "ProjectSorting.h"
#include<iostream>
using namespace std;


//����Ȩ�ؽ��гɼ��޸�;
void ProjectSorting::adjustGradesByTypeWeight(std::vector<Project>& projects, std::unordered_map<std::string, float> typeWeights) {
	auto project = projects.begin();
	while (project != projects.end()) {
		auto it = typeWeights.find(project->type);
		if (it != typeWeights.end())//�鵽��Ӧ����;
		{
			project->grade *= it->second;//�޸ĸ�ֵ;
			++project;
		}

		else {//δ�ҵ���Ӧ����;
			cerr << "δ�ҵ�typeΪ" << project->type << "��Ȩ��" << endl;
			cout << "��ȷ������Ŀ������" << project->type << "��ȷ" << endl;
			cout << "��0ʹ��Ĭ��Ȩ��1.0;" << endl << "��1��Ӹ�����Ȩ��;" << endl << "��2�޸���Ŀ������;" << endl;
			cout << "���������ѡ��: ";
			int a;//�����û�ѡ��;
			cin >> a;

			int s = cin.rdstate();//�ж��û�������Ƿ�Ϊ������Ϊ0��1��2;
			while (s || (a != 0 && a != 1 && a != 2)) {
				cin.clear();
				cerr << "����Ƿ�!����������: ";
				while (cin.get() != '\n');
				cin >> a;
				s = s = cin.rdstate();//���������;
			}

			if (a == 1) {
				pair<string, float> NewtypeWeight;
				NewtypeWeight.first = project->type;
				cout << "������Ȩ��: ";
				cin >> NewtypeWeight.second;
				typeWeights.insert(NewtypeWeight);
				cout << "��ӳɹ�!" << endl;
				project->grade *= NewtypeWeight.second;
				cout << "�ѳɹ��޸ĸ����͵ĳɼ�,����ִ�г���!" << endl << endl;
				++project;
			}//������Ϊ1,����������͵�Ȩ��;

			else if (a == 0)
				++project;//������Ϊ0,��Ȩ��Ϊ1.0,�ɼ�����;

			else {
				cout << "�������޸ĺ�ĸ���Ŀ����: ";
				cin >> project->type;
				cout << endl << "�޸ĳɹ�" << endl;
				project = project;//������Ϊ2,�����¶Ը���Ŀ�����޸�;
			}
		}
	}
	cout << "�ѳɹ��޸����гɼ�!" << endl;
}


//�Զ�����;
void  ProjectSorting::sortAndPickTopByType(std::vector<Project>& projects) {
	sort(projects.begin(), projects.end(), [](const Project& a, const Project& b)->bool {
		return a.grade > b.grade;
		});//lambda������sort����ѡ��;
}

//ȥ����Ŀ;����ÿ�����͵���߳ɼ�;
void ProjectSorting::deduplicateByTypeAndGrade(std::vector<Project>& projects) {

	unordered_map<std::string, float> TopTypes;//���ڴ洢�ѱ�����Ŀ�͸���Ŀ����߳ɼ�;

	auto it = projects.begin();
	while (it != projects.end()) {
		auto project = *it;
		auto typeIt = TopTypes.find(project.type);
		if (typeIt != TopTypes.end()) { // ��������Ѿ��� TopTypes ��
			if (typeIt->second > project.grade) {//���ѱ����ĳɼ����ڸóɼ�
				it = projects.erase(it); // ɾ����ǰ��Ŀ
			}
			else {//���ѱ����ĳɼ����ڸóɼ�
				++it; // ������һ����Ŀ
			}
		}
		else { // ������Ͳ��� TopTypes ��
			TopTypes[project.type] = project.grade; // ��ӵ� TopTypes ��
			++it; // ������һ����Ŀ
		}
	}
	cout << "ȥ�����!" << endl;
}

//�޸���Ŀ�ɼ�;
void ProjectSorting::updateProjectGrade(Project& project, float newGrade, float min, float max) {
	if (newGrade >= min && newGrade <= max) {
		cout << "����ɼ��Ϸ�!" << endl;
		project.grade = newGrade;
		cout << "�ѳɹ��޸ĳɼ�!" << endl;
		return;
	}//����ɼ��ڸ÷�Χ��;

	else {//����ɼ����ڷ�Χ��;
		cerr << "����ɼ��Ƿ�!";
		while (newGrade<min || newGrade>max) {
			cout << "����������ɼ�����-1�˳�: ";
			cin >> newGrade;
			int s = cin.rdstate();
			while (s) {//�ж�������Ƿ�Ϊ����;
				cin.clear();
				cerr << "����Ĳ�������!" << endl;
				cout << "�����������-1�˳�: ";
				while (cin.get() != '\n');
				cin >> newGrade;
				s = cin.rdstate();//�ж�������Ƿ�Ϊ����;
			}
			if (newGrade == -1) {
				cout << "�ѳɹ��˳�!" << endl;//�˳�����;
				return;
			}
			else {
				updateProjectGrade(project, newGrade);
				return;
			}//�����˳�,�������޸ĳɼ�;
		}
	}
}


