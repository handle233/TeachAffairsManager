#pragma once
/*
* project: 教务管理系统
* file   : Teacher.h
* Copyright <c> ciallo all right reserved.
*/
#define TEACHER
#include "PublicHead.h"
#include "CourseList.h"
#include "File.h"

/**简单类型定义**/
typedef int TeaID;

/**结构体类型定义**/
//教师个人信息结构体
struct TeacherInformation {
	sex Sex;//性别
	char* Hometown;//家乡
	Date Birthday;//生日
	char* IDCard;//身份证
	char *Photo;//照片路径
	char* Mail;//邮箱
	char* PhoneNumber;//电话
	char* Title;//职称 
	char* PersonalInf;//个人简介 
};

//教师结构体
struct Teacher
{
	int Id;//工号 
	char* name;//名字
	TeacherInformation Inf;//教师个人信息
	bool remove;
};

/**全局变量定义**/
extern Teacher* AllTeas;

/**公开函数**/
void InitialTeachers(const char* FilePath);
int LoadTeacherFromFile(const char *FilePath);
int SaveTeacherToFile(const char* FilePath);

TeacherInformation MakeTeacherInformation(sex _sex, const Date birthday, const char* hometown,
	const char* idcard, const char* mail, const char* PhoneNumber, const char* Photo,const char* Title,const char* PersonalInf);
void DeConstructTeacherInformation(TeacherInformation *TeaInf);
Teacher MakeTeacher(TeacherInformation TeaInf, const char* name, const TeaID NewID);
void DeConstructTeacher(Teacher* teacher);

int GetNumOfTeachers();
TeaID NewTeacher(Teacher* NewTea);
TeaID DeleteTeacher(TeaID Id);
Teacher* SeekTeacher(TeaID SeekTea);
inline Teacher* IterateTeacher(int *Index);
