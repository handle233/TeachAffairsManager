/*
* project: �������ϵͳ
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
		ShowTitle("�������ϵͳ");
		pwd = InputBox("��������", 1);
		if (strcmp(pwd, "114514") != 0) {
			char tip[] = "�������";
			NewBorder(WIDTH / 2 - strlen(tip), HEIGHT / 2 - 3, strlen(tip) + 10, 5);
			ShowText("�������");
			getch();
		}
	} while (strcmp(pwd, "114514") != 0);
	free(pwd);
	PrintBorder();
	ShowTitle("�������ϵͳ - ������");
	ShowText("1. ѧ������");
	ShowText("2. ��ʦ����");
	ShowText("3. �α����");
	pwd = InputBox("��ѡ��һ��ѡ��", 0);
	PrintBorder();
loop:
	puts("generate");
	LoadStudentFromFile("Students.txt");
	int Time = timeGetTime();
	for (int a = 0; a < 40000; a++) {
		Student Stu = MakeStudent(MakeStudentInformation(male, { 01,1,1 }, "����", "00001", "mail.com", "1145141919810",
			"yeshouxianbei.jpg"), "Ұ���ȱ�", a);
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