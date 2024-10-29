/*
* project: 教务管理系统
* file   : Student.cpp
* Copyright <c> ciallo all right reserved.
*/
#include "Student.h"

/**关键全局变量**/
DataCluster AllStu;
/**最小分配粒度**/
/**文件头标识**/
const char HeadString[HEAD_LENGTH] = "[Ciallo~ Teaching Affairs Manager][Studentsfile]";
/**文件操作相关**/
void InitialStudents(const char* FilePath)
{
	InitialDataCluster(&AllStu, FilePath, sizeof(Student), HeadString);
	FormattedDataCluster(&AllStu);
}
int ReadStuProc(FILE* fp, void* pData) {
	Student* pStu = (Student*)pData;

	fread(&pStu->Id, sizeof(StuID), 1, fp);
	//读入名字
	ReadString(fp, &pStu->name);
	//读入学生信息
	fread(&pStu->Inf.Sex, sizeof(sex), 1, fp);
	fread(&pStu->Inf.Birthday, sizeof(Date), 1, fp);
	ReadString(fp, &pStu->Inf.Hometown);
	ReadString(fp, &pStu->Inf.IDCard);
	ReadString(fp, &pStu->Inf.Mail);
	ReadString(fp, &pStu->Inf.PhoneNumber);
	ReadString(fp, &pStu->Inf.Photo);
	fread(&pStu->Inf.pwdHash, sizeof(int), 1, fp);
	pStu->ClassList = (int*)calloc(128, sizeof(int));
	fread(pStu->ClassList, sizeof(int), 128, fp);
	return 0;
}
int LoadStudentFromFile(const char* FilePath)
{
	return LoadDataFromFile(&AllStu,ReadStuProc , FilePath, sizeof(Student), HeadString);
}


int WriteStuProc(FILE* fStu, void* pData) {
	Student* pStu = (Student*)pData;
	fwrite(&pStu->Id, sizeof(StuID), 1, fStu);

	WriteString(fStu, pStu->name);

	fwrite(&pStu->Inf.Sex, sizeof(sex), 1, fStu);
	fwrite(&pStu->Inf.Birthday, sizeof(Date), 1, fStu);
	WriteString(fStu, pStu->Inf.Hometown);
	WriteString(fStu, pStu->Inf.IDCard);
	WriteString(fStu, pStu->Inf.Mail);
	WriteString(fStu, pStu->Inf.PhoneNumber);
	WriteString(fStu, pStu->Inf.Photo);
	fwrite(&pStu->Inf.pwdHash, sizeof(int), 1, fStu);
	fwrite(pStu->ClassList, sizeof(int), 128, fStu);
	return 0;
}

int SaveStudentToFile(const char* FilePath)
{
	return SaveDataToFile(&AllStu, WriteStuProc);
	
}

/**结构体操作简化函数**/
StudentInformation MakeStudentInformation(sex _sex,Date birthday,const char* hometown,
	const char* idcard, const char* mail, const char* PhoneNumber, const char* Photo)
{
	//先创建函数内临时变量StuInf
	StudentInformation StuInf = { other };
	int len = 0;//len反复表示字符串长度
	StuInf.Sex = _sex;//能直接赋值先赋值
	StuInf.Birthday = birthday;

	/**
	对于文字，如果直接 = 赋值，会把字符串首地址赋值过去，在这里是要创建字符串副本
	所以要先根据字符串长度（strlen）申请内存，然后用memcpy赋值（memcpy在已知长度会略快于strcpy）
	**/
	len = strlen(hometown) + 1;
	StuInf.Hometown = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(StuInf.Hometown,return StuInf)
	memcpy(StuInf.Hometown, hometown, len);
	//下面的同理
	len = strlen(idcard) + 1;
	StuInf.IDCard = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(StuInf.IDCard, return StuInf)
	memcpy(StuInf.IDCard, idcard, len);

	len = strlen(mail) + 1;
	StuInf.Mail = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(StuInf.Mail, return StuInf)
	memcpy(StuInf.Mail, mail, len);

	len = strlen(PhoneNumber) + 1;
	StuInf.PhoneNumber = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(StuInf.PhoneNumber, return StuInf)
	memcpy(StuInf.PhoneNumber, PhoneNumber, len);

	len = strlen(Photo) + 1;
	StuInf.Photo = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(StuInf.Photo, return StuInf)
	memcpy(StuInf.Photo, Photo, len);
	/**
	最后返回的是StuInf，返回的时候会创建StuInf的内存副本，然后函数内的StuInf会被析构，
	但是新的StuInf会被返回，这期间只有字符串指针本身转移了，字符串本身在内存中不受影响
	**/
	return StuInf;
}

void DeConstructStudentInformation(StudentInformation *StuInf)
{
	//全free掉，否则直接丢弃StuInf里面的动态分配字符串不能自动销毁，会内存泄漏
	free(StuInf->Hometown);
	free(StuInf->IDCard);
	free(StuInf->Mail);
	free(StuInf->PhoneNumber);
	free(StuInf->Photo);
}

Student MakeStudent(StudentInformation StuInf,const char* name, const StuID NewID)
{
	//详细代码解读参考MakeStudentInformation
	Student Stu = { 0 };
	if (NewID < 1) {//判断StuID合法否？
		return Stu;
	}
	//直接赋值
	Stu.Id = NewID;
	Stu.name = (char*)calloc(strlen(name) + 1, sizeof(char));
	CHECK_NULLANY(Stu.name,return Stu)
	strcpy(Stu.name, name);
	Stu.Inf = StuInf;//这里可以直接赋值，因为结构体直接传播过去了，相当于是把字符串指针传递过去了
	Stu.ClassList = (int*)calloc(128, 4);
	memset(Stu.ClassList, 0, 128 * 4);
	return Stu;
}

void DeConstructStudent(Student* student)
{
	//参DeConstructStudentInformation
	DeConstructStudentInformation(&student->Inf);
	free(student->name);
}

/**AllStus数据库操作函数**/
int GetNumOfStudents()
{
	return GetLength(&AllStu);
}

StuID NewStudent(Student* NewStu)
{
	if (NewStu->Id == 0) {
		return -1;
	}
	if (SeekStudent(NewStu->Id) != NULL) {
		return -2;
	}
	return AddData(&AllStu, NewStu);
}

StuID DeleteStudent(StuID Id)
{
	Student* pStu = SeekStudent(Id);
	DeConstructStudent(pStu);
	return DeleteData(&AllStu, Id);
}


int SeekStuByIdProc(void* pData,void* args) {
	Student* pStu = (Student*)pData;
	int* Id = (int*)args;

	if (pStu->Id == *Id) {
		return 1;
	}
	return 0;
}
Student* SeekStudent(StuID Id)
{
	return (Student*)SeekData(&AllStu, SeekStuByIdProc,&Id,0);
}
int GetStudentIndex(StuID SeekStu)
{
	int a = 0;
	SeekData(&AllStu, SeekStuByIdProc, &SeekStu, &a);
	return a;
}

Student* IterateStudent(int* Index)
{
	return (Student*)IterateData(&AllStu, Index);
}
