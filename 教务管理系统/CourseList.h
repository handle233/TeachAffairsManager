#pragma once
/*
* project: 教务管理系统
* file   : CourseList.h
* Copyright <c> ciallo all right reserved.
*/
#ifndef COURSELIST
#define COURSELIST
#include "PublicHead.h"
#include "File.h"

/**简单类型定义**/
typedef int CourseID;
typedef int ClassID;
extern int CourseCodeLength;

/**简单结构体定义**/
struct Time {
	int Hour, Min, Sec;
};
struct Period {
	Time Start, End;
};
/**结构体类型定义**/
//课程时间表结构体
struct TimeList {
	int DailyCourseCount;
	Period* ClassTime;
}extern SemesterArrange;//学期时间表安排
//课程信息结构体
struct Course {
	CourseID ID;
	char* Classname,*Intro,*CourseCode;
	int Teacher;
	int MaxAttendee;
};
//课程班级结构体
struct Class {
	ClassID ID;
	CourseID course;
	int StartTime, EndTime;
	Weekday Day;
	int NumOfStus;
	int* StusID;
};
//课程表主结构体定义
struct CourseList {
	Class** Matrix;
};

/**Period操作函数**/
Period NewPeriod(Time t1, Time t2);
void InitialSemester(const char* path, int CourseNum,int LengthOfCourseCode,Period Periods,...);
int LoadSemesterFromFile(const char* path);
int SaveSemesterToFile(const char* path);

int SetupCourse(const char Classname[], const char Introduction[], const char* CourseCode,int TeacherID,int MaxAttend );
int OpenupClass(CourseID course,int StartTime, int EndTime, Weekday WhichDay);

int ChooseClass(ClassID Class, int StuID);
CourseList* GenerateStuCourseList(int StuID);
CourseList* GenerateTeaCourseList(int StuID);
void DeConstructCourseList(CourseList* p);

Course* IterateCourse(int *Index);
Class* IterateClass(int* Index);

#endif