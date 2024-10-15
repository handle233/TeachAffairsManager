/*
* project: 教务管理系统
* file   : CourseList.h
* Copyright <c> ciallo all right reserved.
*/
#include "CourseList.h"
#include "File.h"


int CourseCodeLength;
int NumOfCourses;
Course* AllCourses;
int NumOfClasses;
Class* AllClasses;
TimeList SemesterArrange;

const char HeadString[HEAD_LENGTH] = "[Ciallo~ Teaching Affairs Manager][Courselistsfile]";

Period NewPeriod(Time t1, Time t2) {
	Time Bef = { 0 }, Aft = { 0 };
	Period Per = { 0 };
	if (t1.Hour > t2.Hour) {
		Bef = t2; Aft = t1;
	}
	else if (t1.Hour < t2.Hour) {
		Bef = t1; Aft = t2;
	}
	else {
		if (t1.Min > t2.Min) {
			Bef = t2; Aft = t1;
		}
		else if (t1.Min < t2.Min) {
			Bef = t1; Aft = t2;
		}
		else {
			if (t1.Sec > t2.Sec) {
				Bef = t2; Aft = t1;
			}
			else if (t1.Sec < t2.Sec) {
				Bef = t1; Aft = t2;
			}
			else {
				Bef = t1; Aft = t2;
			}
		}
	}
	Per.Start = Bef, Per.End = Aft;
	return Per;
}
inline int PeriodConflict(int S1Start,int S1Stop, int S2Start,int S2Stop) {
	if (S1Start < S2Start) {
		if (S1Stop >= S2Start) {
			return 1;
		}
	}
	else if (S1Start >= S2Start) {
		if (S1Start <= S2Stop) {
			return 1;
		}
	}
	return 0;
}

void InitialSemester(const char *path,int CourseNum, int LengthOfCourseCode, Period Periods, ...)
{
	SemesterArrange.DailyCourseCount = CourseNum;
	CourseCodeLength = LengthOfCourseCode;
	NumOfClasses = NumOfCourses = 0;

	SemesterArrange.ClassTime = (Period*)calloc(CourseNum, sizeof(Period));
	CHECK_NULLANY(SemesterArrange.ClassTime, return);
	memset(SemesterArrange.ClassTime, 0, sizeof(Period) * CourseNum);

	Period* periods = &Periods;
	for (int a = 0; a < CourseNum; a++) {
		memcpy(SemesterArrange.ClassTime, periods + a, sizeof(Period));
	}

	FILE* fp = SaveFile(path, HeadString);

	fwrite(&SemesterArrange, 1, sizeof(SemesterArrange), fp);
	fwrite(&NumOfCourses, 1, sizeof(int), fp);
	fwrite(&NumOfClasses, 1, sizeof(int), fp);
	fclose(fp);
}

int LoadSemesterFromFile(const char* path)
{
	return 0;
}

int SaveSemesterToFile(const char* path)
{
	return 0;
}

int SetupCourse(const char Classname[], const  char Introduction[], const char* CourseCode, int TeacherID, int MaxAttend)
{
	CHECK_NULLANY(AllCourses,AllCourses = (Course*)malloc(sizeof(Course));)
	else {
		Course* NewCourses = (Course*)realloc(AllCourses, sizeof(Course) * (NumOfCourses + 1));
		CHECK_NULLPTR(NewCourses);
	}
	NumOfCourses++;
	Course Cour = { 0 };
	int l = 0;

	l = strlen(Classname);
	Cour.Classname = (char*)calloc(l+1, sizeof(char));
	CHECK_NULLPTR(Cour.Classname);
	strcpy(Cour.Classname, Classname);

	l = strlen(Introduction);
	Cour.Intro = (char*)calloc(l + 1, sizeof(char));
	CHECK_NULLPTR(Cour.Intro);
	strcpy(Cour.Intro, Introduction);

	Cour.CourseCode = (char*)calloc(CourseCodeLength, sizeof(char));
	CHECK_NULLPTR(Cour.CourseCode);
	memcpy(Cour.CourseCode, CourseCode, CourseCodeLength);
	Cour.Teacher = TeacherID;
	Cour.MaxAttendee = MaxAttend;

	Cour.ID = NumOfCourses;

	memcpy(AllCourses + NumOfCourses - 1, &Cour, sizeof(Course));

	return Cour.ID;
}

int OpenupClass(CourseID course, int StartTime, int EndTime, Weekday WhichDay)
{
	CHECK_NULLANY(AllClasses, AllClasses = (Class*)malloc(sizeof(Class));)
	else {
		Class* NewClasses = (Class*)realloc(AllClasses, sizeof(Class) * (NumOfClasses + 1));
		CHECK_NULLPTR(NewClasses);
	}
	NumOfClasses++;
	Class Cour = { 0 };

	Course* pCou = NULL;
	int found = 0;
	for (int Index = -1; (pCou = IterateCourse(&Index)) != NULL;) {
		if (pCou->ID == course) {
			found = 1;
			break;
		}
	}
	if (!found) {
		return -1;
	}
	Cour.course = course;
	Cour.Day = WhichDay;
	Cour.StartTime = StartTime;
	Cour.EndTime = EndTime;
	Cour.NumOfStus = 0;
	Cour.StusID = NULL;
	Cour.ID = NumOfClasses;

	memcpy(AllClasses + NumOfClasses - 1, &Cour, sizeof(Class));
	return 0;
}

int ChooseClass(ClassID ChooseClass, int StuID)
{
	Class* pCla = NULL;
	CourseList* pCL = GenerateStuCourseList(StuID);
	for (int Index = -1; (pCla = IterateClass(&Index)) != NULL;) {
		if (pCla->ID == ChooseClass) {
			break;
		}
	}
	CHECK_NULLPTR(pCla);
	if (pCL->Matrix[pCla->Day-1][pCla->StartTime-1].course != 0
		|| pCL->Matrix[pCla->Day-1][pCla->EndTime-1].course != 0) {
		return -1;
	}
	int* NewL = (int*)realloc(pCla->StusID, (pCla->NumOfStus+1) * sizeof(int));
	CHECK_NULLPTR(NewL);
	pCla->NumOfStus++;
	pCla->StusID = NewL;
	NewL[pCla->NumOfStus - 1] = StuID;

	return pCla->ID;
}

CourseList* GenerateStuCourseList(int StuID)
{
	CourseList* CL = (CourseList*)malloc(sizeof(CourseList));
	CL->Matrix = (Class**)calloc(WEEKDAY * SemesterArrange.DailyCourseCount, sizeof(Class));
	memset(CL->Matrix, 0, WEEKDAY * SemesterArrange.DailyCourseCount * sizeof(Class));

	Class* pCla = NULL;
	for (int Index = -1; (pCla = IterateClass(&Index)) != NULL;) {
		int found = 0;
		for (int a = 0; a < pCla->NumOfStus;a++) {
			if (pCla->StusID[a] == StuID) {
				memcpy(&CL->Matrix[pCla->Day][pCla->StartTime], pCla, sizeof(Class));
				
			}
		}
	}

	return CL;
}

CourseList* GenerateTeaCourseList(int StuID)
{
	return nullptr;
}

void DeConstructCourseList(CourseList* p)
{
}

Course* IterateCourse(int* Index)
{
	(*Index)++;
	if (*Index >= NumOfCourses) {//判断是不是找完了
		return NULL;
	}
	//直接返回在数组中地址
	return AllCourses + *Index;
}

Class* IterateClass(int* Index)
{
	(*Index)++;
	if (*Index >= NumOfClasses) {//判断是不是找完了
		return NULL;
	}
	//直接返回在数组中地址
	return AllClasses + *Index;
}
