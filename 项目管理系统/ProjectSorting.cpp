#include "ProjectSorting.h"
#include<iostream>
#include <algorithm>
using namespace std;
//根据权重进行成绩修改;
void ProjectSorting::adjustGradesByTypeWeight(std::vector<Project>& projects, std::unordered_map<std::string, float> typeWeights) {
	vector <Project>::iterator project = projects.begin();
	while (project != projects.end()) {
		auto it = typeWeights.find(project->type);
		if (it != typeWeights.end())//查到相应类型;
		{
			project->grade *= it->second;//修改该值;
			++project;
		}

		else {//未找到相应类型;
			auto it = NoFindTypeWeight(project, typeWeights);
			project = it;
		}
	}
	cout << "已成功修改所有成绩!" << endl;
}

//用于未找到权重的错误处理;
vector <Project>::iterator ProjectSorting::NoFindTypeWeight(vector <Project>::iterator& project, unordered_map<std::string, float>& typeWeights) {
	cerr << "未找到type为" << project->type << "的权重" << endl;
	cout << "请确保该项目的类型" << project->type << "正确" << endl;
	cout << "按0使用默认权重1.0;" << endl << "按1添加该类型权重;" << endl << "按2修改项目的类型;" << endl;
	cout << "请输入你的选择: ";

	int a = SelectNoFindTyWe();//定义用户选择;

	if (a == 1) {
		SelectNoFindTyWe1(project, typeWeights);
		return ++project;
	}//若输入为1,则增添该类型的权重;

	else if (a == 0) {
		SelectNoFindTyWe0(project, typeWeights);
		return ++project;
	}//若输入为0,则权重为1.0,成绩不变;

	else {
		SelectNoFindTyWe2(project);//若输入为2,则重新对该项目进行修改;
		return project;
	}

}

//用于得到用户对于未发现typeweight问题相关处理的选择;
int ProjectSorting::SelectNoFindTyWe() {
	int a;//定义用户选择;
	cin >> a;
	int s = cin.rdstate();//判断用户输入的是否为数字且为0或1或2;
	while (s || (a != 0 && a != 1 && a != 2)) {
		cin.clear();
		cerr << "输入非法!请重新输入: ";
		while (cin.get() != '\n');
		cin >> a;
		s = cin.rdstate();//输入错误处理;
	}
	return a;
}

//用于用户选择0处理问题(使用默认权重1.0)
void ProjectSorting::SelectNoFindTyWe0(vector <Project>::iterator& project, unordered_map<std::string, float>& typeWeights) {
	cout << "是否保存该权重,(若保存则以后都使用该权重1.0)" << endl;
	cout << "输入1表示是,输入2表示否:";
	int a;//定义用户选择;
	cin >> a;
	int s = cin.rdstate();//判断用户输入的是否为数字且为1或2;
	while (s || (a != 1 && a != 2)) {
		cin.clear();
		cerr << "输入非法!请重新输入: ";
		while (cin.get() != '\n');
		cin >> a;
		s = cin.rdstate();//输入错误处理;
	}
	if (a == 1) {//用户输入1,保存该类型的权重;
		pair<string, float> NewtypeWeight;
		NewtypeWeight.first = project->type;
		NewtypeWeight.second = 1.0;
		typeWeights.insert(NewtypeWeight);
		cout << "添加成功!" << endl;
		cout << "已成功修改该类型的成绩!" << endl;
		return;
	}
	return;//用户输入2,不做处理;
}

//用于用户选择1处理问题(添加该类型的权重)
void ProjectSorting::SelectNoFindTyWe1(vector <Project>::iterator& project,
	unordered_map<std::string, float>& typeWeights) {
	pair<string, float> NewtypeWeight;
	NewtypeWeight.first = project->type;
	cout << "请输入权重: ";
	cin >> NewtypeWeight.second;
	typeWeights.insert(NewtypeWeight);
	cout << "添加成功!" << endl;
	project->grade *= NewtypeWeight.second;
	cout << "已成功修改该类型的成绩!" << endl;
	return;
}

//用于用户选择2处理问题(修改项目的类型)
void ProjectSorting::SelectNoFindTyWe2(vector <Project>::iterator& project) {
	cout << "请输入修改后的该项目类型: ";
	cin >> project->type;
	cout << endl << "修改成功" << endl;
	return;
}

//自动排序;
void  ProjectSorting::sortAndPickTopByType(vector<Project>& projects) {
	sort(projects.begin(), projects.end(), [](Project& a, Project& b)->bool {
		return a > b;
		});//lambda函数和sort函数选择;
}


//去重项目;保留每个类型的最高成绩;
void ProjectSorting::deduplicateByTypeAndGrade(std::vector<Project>& projects) {
	sortAndPickTopByType(projects);//去重前先排序;
	projects.erase(unique(projects.begin(), projects.end(), [](Project& a, Project& b)->bool {
		return a == b;
		}), projects.end());//lambda函数和erase函数去重;
}


//修改项目成绩;
void ProjectSorting::updateProjectGrade(Project& project, float newGrade, float min, float max) {
	if (newGrade >= min && newGrade <= max) {
		cout << "输入成绩合法!" << endl;
		project.grade = newGrade;
		cout << "已成功修改成绩!" << endl;
		return;
	}//输入成绩在该范围内;
	else {//输入成绩不在范围内;
		cerr << "输入成绩非法!";
		while (newGrade<min || newGrade>max) {
			cout << "请重新输入成绩或则按-1退出: ";
			newGrade = RegetupdateProjectGrade();//重新获取newGrade;
			if (newGrade == -1) {
				cout << "已成功退出!" << endl;//退出程序;
				return;
			}
			else {
				updateProjectGrade(project, newGrade);
				return;
			}//若不退出,则重新修改成绩;
		}
	}
}

//修改项目错误处理
int ProjectSorting::RegetupdateProjectGrade() {
	int newGrade;
	cin >> newGrade;
	int s = cin.rdstate();
	while (s) {//判断输入的是否为数字;
		cin.clear();
		cerr << "输入的不是数字!" << endl;
		cout << "请重新输入或按-1退出: ";
		while (cin.get() != '\n');
		cin >> newGrade;
		s = cin.rdstate();//判断输入的是否为数字;
	}
	return newGrade;
}


