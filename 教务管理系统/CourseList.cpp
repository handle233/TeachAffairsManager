/*
* project: 教务管理系统
* file   : CourseList.cpp
* Copyright <c> ciallo all right reserved.
*/
#include "CourseList.h"

TimeTable SemesterArrange;
const char HeadString[HEAD_LENGTH] = "[Ciallo~ Teaching Affairs Manager][Coursesfile]";
DataCluster AllCourse;
DataCluster AllClass;

int InitialSemester(const char* path, TimeTable* Table)
{
	memcpy(&SemesterArrange, Table, sizeof(TimeTable));
	FILE* fp = SaveFile(path, HeadString);
	CHECK_NULLPTR(fp);

	fwrite(&SemesterArrange.NumOfCourse, sizeof(int), 1, fp);
	fwrite(SemesterArrange.ClassTime, sizeof(Period), SemesterArrange.NumOfCourse, fp);
	fclose(fp);
	return 1;
}

void InitialCourses(const char* path)
{
	InitialDataCluster(&AllCourse, path, sizeof(Course), HeadString);
	FormattedDataCluster(&AllCourse);
}

void InitialClasses(const char* path)
{
	InitialDataCluster(&AllClass, path, sizeof(Class), HeadString);
	FormattedDataCluster(&AllClass);
}

int LoadSemesterFromFile(const char* path)
{
	char Recv[HEAD_LENGTH] = "";
	FILE* fp = OpenFile(path, "r", Recv);
	CHECK_NULLANY(memcmp(Recv, HeadString, HEAD_LENGTH))else {
		return -1;
	}
	fread(&SemesterArrange.NumOfCourse, sizeof(int), 1, fp);
	SemesterArrange.ClassTime = (Time*)malloc(SemesterArrange.NumOfCourse * sizeof(Time));
	fread(SemesterArrange.ClassTime, sizeof(Period), SemesterArrange.NumOfCourse, fp);
	return 0;
}

int ReadCourseProc(FILE* fC, void* pData) {
	Course* pCou = (Course*)pData;
	int NumTea = 0;
	fread(&pCou->ID, sizeof(int), 1, fC);
	fread(&pCou->MaxAttendee, sizeof(int), 1, fC);
	fread(&pCou->Teacher, sizeof(int), 1, fC);

	ReadString(fC, &pCou->CourseName);
	ReadString(fC, &pCou->Inf.CourseCode);
	ReadString(fC, &pCou->Inf.Intro);
	return 1;
}
int LoadCoursesFromFile(const char* path)
{
	return LoadDataFromFile(&AllCourse,ReadCourseProc , path, sizeof(Course), HeadString);
}
int ReadClassProc(FILE* fC, void* pData) {
	Class* pCl = (Class*)pData;
	fread(&pCl->ID, sizeof(ClassID), 1, fC);
	fread(&pCl->Duration, sizeof(Period), 1, fC);
	fread(&pCl->Day, sizeof(Weekday), 1, fC);
	fread(&pCl->course, sizeof(CourseID), 1, fC);
	ReadString(fC, &pCl->ClassInfPath);
	return 1;
}
int LoadClassesFromFile(const char* path)
{
	return LoadDataFromFile(&AllClass , ReadClassProc, path, sizeof(Class), HeadString);
}
int WriteCourseProc(FILE* fC, void* pData) {
	Course* pCou = (Course*)pData;
	fwrite(&pCou->ID, sizeof(int), 1, fC);
	fwrite(&pCou->MaxAttendee, sizeof(int), 1, fC);
	fwrite(&pCou->Teacher, sizeof(int), 1, fC);

	WriteString(fC, pCou->CourseName);
	WriteString(fC, pCou->Inf.CourseCode);
	WriteString(fC, pCou->Inf.Intro);
	return 1;
}
int SaveCoursesToFile()
{
	return SaveDataToFile(&AllCourse, WriteCourseProc);
}
int WriteClassProc(FILE* fC, void* pData) {
	Class* pCl = (Class*)pData;
	fwrite(&pCl->ID, sizeof(ClassID), 1, fC);
	fwrite(&pCl->Duration, sizeof(Period), 1, fC);
	fwrite(&pCl->Day, sizeof(Weekday), 1, fC);
	fwrite(&pCl->course, sizeof(CourseID), 1, fC);
	WriteString(fC, pCl->ClassInfPath);
	return 1;
}
int SaveClassesToFile()
{
	return SaveDataToFile(&AllClass, WriteClassProc);
}
TimeTable MakeTimeTable(int NumOfTable, Time Class, ...)
{
	Time* ps = &Class;
	TimeTable table;
	table.NumOfCourse = NumOfTable;
	table.ClassTime = (Time*)calloc(NumOfTable, sizeof(Period));
	memcpy(table.ClassTime, ps, sizeof(Period) * NumOfTable);

	return table;
}
Time makeTime(int H, int M)
{
	Time t = { H,M };
	return t;
}
void DeConstructTimeTable(TimeTable* table) {
	free(table->ClassTime);
}
Course MakeCourse(CourseInformation Inf, const char* CourseName, int MaxAttend, CourseID Id, int NumofTea, int Tea, ...)
{
	Course course;
	
	course.ID = Id;
	course.MaxAttendee = MaxAttend;
	course.Teacher = NumofTea;
	course.CourseName = NewString(CourseName);

	course.Inf = Inf;

	return course;
}
CourseInformation MakeCourseInformation(const char* ClassCode, const char* Intro)
{
	CourseInformation inf;
	inf.CourseCode = NewString(ClassCode);
	inf.Intro = NewString(Intro);
	return inf;
}
void DeConstructCourse(Course* course)
{
	free(course->CourseName);
	DeConstructCourseInformation(course->Inf);
}
void DeConstructCourseInformation(CourseInformation Inf)
{
	free(Inf.CourseCode);
	free(Inf.Intro);
}
Class MakeClass(ClassID Id, Period Duration, Weekday Day, CourseID Course)
{
	Class cl = { 0 };
	cl.ID = Id;
	cl.Day = Day;
	cl.Duration = Duration;
	cl.course = Course;
	cl.ClassInfPath = (char*)calloc(_MAX_PATH, sizeof(char));
	sprintf(cl.ClassInfPath, "%X.class", cl.ID);

	return cl;
}
ClassInformation MakeClassInformation(int AttendStuNum, Period DuringWeek)
{
	ClassInformation Inf;
	Inf.NumOfStus = AttendStuNum;
	Inf.StuSitus = (StuSituation*)calloc(AttendStuNum,sizeof(StuSituation));
	for (int a = 0; a < Inf.NumOfStus; a++) {
		Inf.StuSitus[a].StuID = 0;
		Inf.StuSitus[a].Grade = 0;
	}
	Inf.WeekDuration = DuringWeek;
	return Inf;
}

int NewCourse(Course* course)
{
	return AddData(&AllCourse, course);
}
int NewClass(Class* Cl)
{
	return AddData(&AllClass, Cl);
}

int SeekCourseByIdProc(void* pData, void* args) {
	Course* pCou = (Course*)pData;
	int* Id = (int*)args;
	if (pCou->ID == *Id) {
		return 1;
	}
	return 0;
}
Course* SeekCourseById(CourseID Id)
{
	return (Course*)SeekData(&AllCourse,SeekCourseByIdProc , &Id, 0);
}
int SeekClassByIdProc(void* pData, void* args) {
	Class* pCou = (Class*)pData;
	int* Id = (int*)args;
	if (pCou->ID == *Id) {
		return 1;
	}
	return 0;
}
Class* SeekClassById(ClassID Id)
{
	return (Class*)SeekData(&AllClass, SeekClassByIdProc, &Id, 0);
}
Course* IterateCourse(int* Index)
{
	return (Course*)IterateData(&AllCourse, Index);
}
Class* IterateClass(int* Index)
{
	return (Class*)IterateData(&AllClass, Index);
}

int ReadClInfProc(FILE* fC, void* pData) {
	ClassInformation* pInf = (ClassInformation*)pData;
	fread(&pInf->NumOfStus, sizeof(int), 1, fC);
	fread(&pInf->WeekDuration, sizeof(Period), 1, fC);
	pInf->StuSitus = (StuSituation*)calloc(pInf->NumOfStus, sizeof(StuSituation));
	fread(pInf->StuSitus, sizeof(StuSituation), pInf->NumOfStus, fC);
	return 1;
}
int LoadClassInfFromFile(DataCluster *InfClu,Class* cl) {
	return LoadDataFromFile(InfClu, ReadClInfProc, cl->ClassInfPath, sizeof(ClassInformation), HeadString);
}
int WriteClInfProc(FILE* fC, void* pData) {
	ClassInformation* pInf = (ClassInformation*)pData;
	fwrite(&pInf->NumOfStus, sizeof(int), 1, fC);
	fwrite(&pInf->WeekDuration, sizeof(Period), 1, fC);
	fwrite(pInf->StuSitus, sizeof(StuSituation), pInf->NumOfStus, fC);
	return 1;
}
int SaveClassInfFromFile(DataCluster* InfClu,Class* cl, ClassInformation* Inf) {
	return SaveDataToFile(InfClu, WriteClInfProc);
}
int CheckArrival(ClassID Cl, int StuID)
{
	Class *cl = SeekClassById(Cl);
	char str[HEAD_LENGTH] = "";
	FILE* fp = OpenFile(cl->ClassInfPath, "r", str);

	ClassInformation Inf;
	ReadClInfProc(fp, &Inf);
	for (int a = 0; a < Inf.NumOfStus; a++) {
		if (Inf.StuSitus[a].StuID == StuID) {
			Inf.StuSitus[a].Attendence++;
			fclose(fp);
			fp = SaveFile(cl->ClassInfPath, HeadString);
			WriteClInfProc(fp, &Inf);
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return 0;
}

int ReadArrival(ClassID Cl, int StuID)
{
	Class* cl = SeekClassById(Cl);
	char str[HEAD_LENGTH] = "";
	FILE* fp = OpenFile(cl->ClassInfPath, "r", str);

	ClassInformation Inf;
	ReadClInfProc(fp, &Inf);
	for (int a = 0; a < Inf.NumOfStus; a++) {
		if (Inf.StuSitus[a].StuID == StuID) {
			fclose(fp);
			return Inf.StuSitus[a].Attendence;
		}
	}
	fclose(fp);
	return -1;
}

int ChooseClass(ClassID Cl, int StuID)
{
	Class* cl = SeekClassById(Cl);
	ClassInformation Inf;
	char str[HEAD_LENGTH] = "";
	FILE* fp = OpenFile(cl->ClassInfPath, "r", str);

	CHECK_NULLANY(fp,
		fp = SaveFile(cl->ClassInfPath, HeadString);
	Inf = MakeClassInformation(1, { 1,10 });
	Inf.NumOfStus--;
	WriteClInfProc(fp, &Inf);
	fclose(fp);
		)else {
		ReadClInfProc(fp, &Inf);
		Inf.StuSitus = (StuSituation*)realloc(Inf.StuSitus, sizeof(StuSituation) * (Inf.NumOfStus + 1));
	}


	Inf.StuSitus[Inf.NumOfStus].Attendence = 0;
	Inf.StuSitus[Inf.NumOfStus].Grade = 0;
	Inf.StuSitus[Inf.NumOfStus].StuID = StuID;
	Inf.NumOfStus++;
	fp = SaveFile(cl->ClassInfPath, HeadString);
	WriteClInfProc(fp, &Inf);
	fclose(fp);
	return 1;
}

int LoginGrade(ClassID Cl, int StuID,int NewGrade)
{
	Class* cl = SeekClassById(Cl);
	char str[HEAD_LENGTH] = "";
	FILE* fp = OpenFile(cl->ClassInfPath, "r", str);

	ClassInformation Inf;
	ReadClInfProc(fp, &Inf);
	for (int a = 0; a < Inf.NumOfStus; a++) {
		if (Inf.StuSitus[a].StuID == StuID) {
			Inf.StuSitus[a].Grade = NewGrade;
			fclose(fp);
			fp = SaveFile(cl->ClassInfPath, HeadString);
			WriteClInfProc(fp, &Inf);
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return 0;
}

ClassInformation ReturnClassStudentName(ClassID Cl) {
	Class* cl = SeekClassById(Cl);
	char str[HEAD_LENGTH] = "";
	FILE* fp = OpenFile(cl->ClassInfPath, "r", str);

	ClassInformation Inf;
	ReadClInfProc(fp, &Inf);
	fclose(fp);
	return Inf;
}

int SeekGrade(ClassID Cl, int StuID)
{
	Class* cl = SeekClassById(Cl);
	char str[HEAD_LENGTH] = "";
	FILE* fp = OpenFile(cl->ClassInfPath, "r", str);

	ClassInformation Inf;
	ReadClInfProc(fp, &Inf);
	for (int a = 0; a < Inf.NumOfStus; a++) {
		if (Inf.StuSitus[a].StuID == StuID) {
			fclose(fp);
			return Inf.StuSitus[a].Grade;
		}
	}
	fclose(fp);
	return -1;
}
