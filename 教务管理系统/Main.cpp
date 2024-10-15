/*
* project: 教务管理系统
* file   : Main.cpp
* Copyright <c> ciallo all right reserved.
*/
#include "PublicHead.h"
#include "Student.h"
#include "Teacher.h"
#include "CourseList.h"
#include "UserInterface.h"

#include <Windows.h>
#pragma comment(lib,"winmm.lib")

int main() {
	
	char* pwd = NULL;
	do{
		if (pwd) {
			free(pwd);
		}
		PrintBorder();
		ShowTitle("教务管理系统");
		pwd = InputBox("输入密码", 1);
		if (strcmp(pwd, "114514") != 0) {
			char tip[] = "密码错误";
			NewBorder(WIDTH / 2 - strlen(tip), HEIGHT / 2 - 3, strlen(tip) + 10, 5);
			ShowText("密码错误");
			getch();
		}
	} while (strcmp(pwd, "114514") != 0);
	free(pwd);
	PrintBorder();
	ShowTitle("教务管理系统 - 主界面");
	ShowText("1. 学生管理");
	ShowText("2. 老师管理");
	ShowText("3. 课表管理");
	pwd = InputBox("请选择一个选项", 0);
	PrintBorder();
loop:
	puts("generate");
	LoadStudentFromFile("Students.txt");
	int Time = timeGetTime();
	for (int a = 0; a < 40000; a++) {
		Student Stu = MakeStudent(MakeStudentInformation(male, { 01,1,1 }, "地球", "00001", "mail.com", "1145141919810",
			"yeshouxianbei.jpg"), "野兽先辈", a);
		NewStudent(&Stu);
		DeConstructStudent(&Stu);
	}
	printf("Cost %d ms\n", timeGetTime() - Time);
	puts("De gen");
	Time = timeGetTime();
	for (int a = 0; a < 40000; a++) {
		DeleteStudent(a);
	}
	printf("Cost %d ms\n", timeGetTime() - Time);
	SaveStudentToFile("Students.txt");
	getch();
	goto loop;
	
	
	return 0;
}

/*

*/