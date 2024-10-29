#pragma once
/*
* project: 教务管理系统
* file   : CourseList.h
* Copyright <c> ciallo all right reserved.
*/
#ifndef COURSELIST
#define COURSELIST
#include "PublicHead.h"
#include "DataManage.h"

/**简单类型定义**/
typedef int CourseID;
typedef int ClassID;

/**简单结构体定义**/
struct Time {
	int Hour, Min;
};
struct Period {
	int Start, End;
};
/**结构体类型定义**/
//课程时间表结构体

struct TimeTable {
	int NumOfCourse;
	Time* ClassTime;
};//学期时间表安排

extern TimeTable SemesterArrange;
//课程信息结构体
struct CourseInformation {
	char* CourseCode;
	char* Intro;
};
struct Course {
	CourseID ID;
	char* CourseName;
	int Teacher;
	int MaxAttendee;
	CourseInformation Inf;
};
//课程班级结构体
struct StuSituation {
	int StuID;
	int Grade;
	int Attendence;
};
struct ClassInformation {
	int NumOfStus;
	StuSituation *StuSitus;
	Period WeekDuration;
};
struct Class {
	ClassID ID;
	CourseID course;
	Period Duration;
	Weekday Day;
	char *ClassInfPath;
};
/****关键全局变量****/
extern DataCluster AllCourse;
extern DataCluster AllClass;
/****函数****/

int InitialSemester(const char* path,TimeTable *Table);
void InitialCourses(const char* path);
void InitialClasses(const char* path);

int LoadSemesterFromFile(const char* path);
int LoadCoursesFromFile(const char* path);
int LoadClassesFromFile(const char* path);

int SaveCoursesToFile();
int SaveClassesToFile();

TimeTable MakeTimeTable(int NumOfTable,Time Class,...);
Time makeTime(int H, int M);
void DeConstructTimeTable(TimeTable* table);

Course MakeCourse(CourseInformation Inf,const char* CourseName,int MaxAttend,CourseID Id,int NumofTea,int Tea,...);
CourseInformation MakeCourseInformation(const char* ClassCode,const char*Intro);
void DeConstructCourse(Course* course);
void DeConstructCourseInformation(CourseInformation Inf);

Class MakeClass(ClassID Id,Period Duration,Weekday Day,CourseID Course);
ClassInformation MakeClassInformation(int AttendStuNum,Period DuringWeek);
void DeConstructClass(Class* Class);
void DeConstructClassInformation(ClassInformation inf);

int NewCourse(Course* course);
int NewClass(Class* Cl);

Course* SeekCourseById(CourseID Id);
Class* SeekClassById(ClassID Id);

Course* IterateCourse(int* Index);
Class* IterateClass(int* Index);

int LoadClassInfFromFile(DataCluster* InfClu, Class* cl);
int SaveClassInfFromFile(DataCluster* InfClu, Class* cl, ClassInformation* Inf);

int CheckArrival(ClassID Cl, int StuID);
int ReadArrival(ClassID Cl, int Stus);

int ChooseClass(ClassID Cl, int StuID);
ClassInformation ReturnClassStudentName(ClassID Cl);

int LoginGrade(ClassID Cl, int StuID,int NewGrade);
int SeekGrade(ClassID Cl, int Stus);

#endif