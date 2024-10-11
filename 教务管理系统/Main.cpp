/*
* project: 教务管理系统
* file   : Main.cpp
* Copyright <c> ciallo all right reserved.
*/
#include "PublicHead.h"
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

int main() {

	printf("hello world \0 genshin");
	getch();

	puts("generate");
	LoadStudentFromFile("Students.txt");
	int Time = timeGetTime();
	for (int a = 0; a < 100000; a++) {
		Student Stu = MakeStudent(MakeStudentInformation(male, { 01,1,1 }, "地球", "00001", "mail.com", "1145141919810",
			"yeshouxianbei.jpg"), "野兽先辈", a);
		NewStudent(&Stu);
		DeConstructStudent(&Stu);
	}
	printf("Cost %d ms\n", timeGetTime() - Time);
	puts("De gen");
	Time = timeGetTime();
	for (int a = 0; a < 100000; a++) {
		DeleteStudent(a);
	}
	printf("Cost %d ms\n", timeGetTime() - Time);
	SaveStudentToFile("Students.txt");
	
	return 0;
}