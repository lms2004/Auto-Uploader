#include "ProjectSorting.h"
#include<iostream>
using namespace std;


//根据权重进行成绩修改;
void ProjectSorting::adjustGradesByTypeWeight(std::vector<Project>& projects, std::unordered_map<std::string, float> typeWeights) {
	auto project = projects.begin();
	while (project != projects.end()) {
		auto it = typeWeights.find(project->type);
		if (it != typeWeights.end())//查到相应类型;
		{
			project->grade *= it->second;//修改该值;
			++project;
		}

		else {//未找到相应类型;
			cerr << "未找到type为" << project->type << "的权重" << endl;
			cout << "请确保该项目的类型" << project->type << "正确" << endl;
			cout << "按0使用默认权重1.0;" << endl << "按1添加该类型权重;" << endl << "按2修该项目的类型;" << endl;
			cout << "请输入你的选择: ";
			int a;//定义用户选择;
			cin >> a;

			int s = cin.rdstate();//判断用户输入的是否为数字且为0或1或2;
			while (s || (a != 0 && a != 1 && a != 2)) {
				cin.clear();
				cerr << "输入非法!请重新输入: ";
				while (cin.get() != '\n');
				cin >> a;
				s = s = cin.rdstate();//输入错误处理;
			}

			if (a == 1) {
				pair<string, float> NewtypeWeight;
				NewtypeWeight.first = project->type;
				cout << "请输入权重: ";
				cin >> NewtypeWeight.second;
				typeWeights.insert(NewtypeWeight);
				cout << "添加成功!" << endl;
				project->grade *= NewtypeWeight.second;
				cout << "已成功修改该类型的成绩,继续执行程序!" << endl << endl;
				++project;
			}//若输入为1,则增添该类型的权重;

			else if (a == 0)
				++project;//若输入为0,则权重为1.0,成绩不变;

			else {
				cout << "请输入修改后的该项目类型: ";
				cin >> project->type;
				cout << endl << "修改成功" << endl;
				project = project;//若输入为2,则重新对该项目进行修改;
			}
		}
	}
	cout << "已成功修改所有成绩!" << endl;
}


//自动排序;
void  ProjectSorting::sortAndPickTopByType(std::vector<Project>& projects) {
	sort(projects.begin(), projects.end(), [](const Project& a, const Project& b)->bool {
		return a.grade > b.grade;
		});//lambda函数和sort函数选择;
}

//去重项目;保留每个类型的最高成绩;
void ProjectSorting::deduplicateByTypeAndGrade(std::vector<Project>& projects) {

	unordered_map<std::string, float> TopTypes;//用于存储已保留项目和该项目的最高成绩;

	auto it = projects.begin();
	while (it != projects.end()) {
		auto project = *it;
		auto typeIt = TopTypes.find(project.type);
		if (typeIt != TopTypes.end()) { // 如果类型已经在 TopTypes 中
			if (typeIt->second > project.grade) {//若已保留的成绩大于该成绩
				it = projects.erase(it); // 删除当前项目
			}
			else {//若已保留的成绩等于该成绩
				++it; // 继续下一个项目
			}
		}
		else { // 如果类型不在 TopTypes 中
			TopTypes[project.type] = project.grade; // 添加到 TopTypes 中
			++it; // 继续下一个项目
		}
	}
	cout << "去重完毕!" << endl;
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


