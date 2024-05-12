#include "ProjectSorting.h"
#include<iostream>
#include <algorithm>
using namespace std;
//����Ȩ�ؽ��гɼ��޸�;
void ProjectSorting::adjustGradesByTypeWeight(std::vector<Project>& projects, std::unordered_map<std::string, float> typeWeights) {
	vector <Project>::iterator project = projects.begin();
	while (project != projects.end()) {
		auto it = typeWeights.find(project->type);
		if (it != typeWeights.end())//�鵽��Ӧ����;
		{
			project->grade *= it->second;//�޸ĸ�ֵ;
			++project;
		}

		else {//δ�ҵ���Ӧ����;
			auto it = NoFindTypeWeight(project, typeWeights);
			project = it;
		}
	}
	cout << "�ѳɹ��޸����гɼ�!" << endl;
}

//����δ�ҵ�Ȩ�صĴ�����;
vector <Project>::iterator ProjectSorting::NoFindTypeWeight(vector <Project>::iterator& project, unordered_map<std::string, float>& typeWeights) {
	cerr << "δ�ҵ�typeΪ" << project->type << "��Ȩ��" << endl;
	cout << "��ȷ������Ŀ������" << project->type << "��ȷ" << endl;
	cout << "��0ʹ��Ĭ��Ȩ��1.0;" << endl << "��1��Ӹ�����Ȩ��;" << endl << "��2�޸���Ŀ������;" << endl;
	cout << "���������ѡ��: ";

	int a = SelectNoFindTyWe();//�����û�ѡ��;

	if (a == 1) {
		SelectNoFindTyWe1(project, typeWeights);
		return ++project;
	}//������Ϊ1,����������͵�Ȩ��;

	else if (a == 0) {
		SelectNoFindTyWe0(project, typeWeights);
		return ++project;
	}//������Ϊ0,��Ȩ��Ϊ1.0,�ɼ�����;

	else {
		SelectNoFindTyWe2(project);//������Ϊ2,�����¶Ը���Ŀ�����޸�;
		return project;
	}

}

//���ڵõ��û�����δ����typeweight������ش����ѡ��;
int ProjectSorting::SelectNoFindTyWe() {
	int a;//�����û�ѡ��;
	cin >> a;
	int s = cin.rdstate();//�ж��û�������Ƿ�Ϊ������Ϊ0��1��2;
	while (s || (a != 0 && a != 1 && a != 2)) {
		cin.clear();
		cerr << "����Ƿ�!����������: ";
		while (cin.get() != '\n');
		cin >> a;
		s = cin.rdstate();//���������;
	}
	return a;
}

//�����û�ѡ��0��������(ʹ��Ĭ��Ȩ��1.0)
void ProjectSorting::SelectNoFindTyWe0(vector <Project>::iterator& project, unordered_map<std::string, float>& typeWeights) {
	cout << "�Ƿ񱣴��Ȩ��,(���������Ժ�ʹ�ø�Ȩ��1.0)" << endl;
	cout << "����1��ʾ��,����2��ʾ��:";
	int a;//�����û�ѡ��;
	cin >> a;
	int s = cin.rdstate();//�ж��û�������Ƿ�Ϊ������Ϊ1��2;
	while (s || (a != 1 && a != 2)) {
		cin.clear();
		cerr << "����Ƿ�!����������: ";
		while (cin.get() != '\n');
		cin >> a;
		s = cin.rdstate();//���������;
	}
	if (a == 1) {//�û�����1,��������͵�Ȩ��;
		pair<string, float> NewtypeWeight;
		NewtypeWeight.first = project->type;
		NewtypeWeight.second = 1.0;
		typeWeights.insert(NewtypeWeight);
		cout << "��ӳɹ�!" << endl;
		cout << "�ѳɹ��޸ĸ����͵ĳɼ�!" << endl;
		return;
	}
	return;//�û�����2,��������;
}

//�����û�ѡ��1��������(��Ӹ����͵�Ȩ��)
void ProjectSorting::SelectNoFindTyWe1(vector <Project>::iterator& project,
	unordered_map<std::string, float>& typeWeights) {
	pair<string, float> NewtypeWeight;
	NewtypeWeight.first = project->type;
	cout << "������Ȩ��: ";
	cin >> NewtypeWeight.second;
	typeWeights.insert(NewtypeWeight);
	cout << "��ӳɹ�!" << endl;
	project->grade *= NewtypeWeight.second;
	cout << "�ѳɹ��޸ĸ����͵ĳɼ�!" << endl;
	return;
}

//�����û�ѡ��2��������(�޸���Ŀ������)
void ProjectSorting::SelectNoFindTyWe2(vector <Project>::iterator& project) {
	cout << "�������޸ĺ�ĸ���Ŀ����: ";
	cin >> project->type;
	cout << endl << "�޸ĳɹ�" << endl;
	return;
}

//�Զ�����;
void  ProjectSorting::sortAndPickTopByType(vector<Project>& projects) {
	sort(projects.begin(), projects.end(), [](Project& a, Project& b)->bool {
		return a > b;
		});//lambda������sort����ѡ��;
}


//ȥ����Ŀ;����ÿ�����͵���߳ɼ�;
void ProjectSorting::deduplicateByTypeAndGrade(std::vector<Project>& projects) {
	sortAndPickTopByType(projects);//ȥ��ǰ������;
	projects.erase(unique(projects.begin(), projects.end(), [](Project& a, Project& b)->bool {
		return a == b;
		}), projects.end());//lambda������erase����ȥ��;
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
			newGrade = RegetupdateProjectGrade();//���»�ȡnewGrade;
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

//�޸���Ŀ������
int ProjectSorting::RegetupdateProjectGrade() {
	int newGrade;
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
	return newGrade;
}


