#pragma once
/*
* project: �������ϵͳ
* file   : CourseList.h
* Copyright <c> ciallo all right reserved.
*/
#ifndef COURSELIST
#define COURSELIST
#include "PublicHead.h"
#include "File.h"

/**�����Ͷ���**/
typedef int CourseID;
typedef int ClassID;
extern int CourseCodeLength;

/**�򵥽ṹ�嶨��**/
struct Time {
	int Hour, Min, Sec;
};
struct Period {
	Time Start, End;
};
/**�ṹ�����Ͷ���**/
//�γ�ʱ���ṹ��
struct TimeList {
	int DailyCourseCount;
	Period* ClassTime;
}extern SemesterArrange;//ѧ��ʱ�����
//�γ���Ϣ�ṹ��
struct Course {
	CourseID ID;
	char* Classname,*Intro,*CourseCode;
	int Teacher;
	int MaxAttendee;
};
//�γ̰༶�ṹ��
struct Class {
	ClassID ID;
	CourseID course;
	int StartTime, EndTime;
	Weekday Day;
	int NumOfStus;
	int* StusID;
};
//�γ̱����ṹ�嶨��
struct CourseList {
	Class** Matrix;
};

/**Period��������**/
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