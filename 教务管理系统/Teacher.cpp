/*
* project: 教务管理系统
* file   : Teacher.cpp
* Copyright <c> ciallo all right reserved.
*/
#include "Teacher.h"

/**关键全局变量**/
DataCluster AllTea;
/**最小分配粒度**/
/**文件头标识**/
const char HeadString[HEAD_LENGTH] = "[Ciallo~ Teaching Affairs Manager][Teachersfile]";
/**文件操作相关**/
void InitialTeachers(const char* FilePath)
{
	InitialDataCluster(&AllTea, FilePath, sizeof(Teacher), HeadString);
	FormattedDataCluster(&AllTea);
}

int WriteTeaProc(FILE* fTea, void* pData) {
	Teacher* pTea = (Teacher*)pData;
	fwrite(&pTea->Id, sizeof(TeaID), 1, fTea);

	WriteString(fTea, pTea->name);

	fwrite(&pTea->Inf.Sex, sizeof(sex), 1, fTea);
	fwrite(&pTea->Inf.Birthday, sizeof(Date), 1, fTea);
	WriteString(fTea, pTea->Inf.Hometown);
	WriteString(fTea, pTea->Inf.IDCard);
	WriteString(fTea, pTea->Inf.Mail);
	WriteString(fTea, pTea->Inf.PhoneNumber);
	WriteString(fTea, pTea->Inf.Photo);
	WriteString(fTea, pTea->Inf.Title);
	WriteString(fTea, pTea->Inf.PersonalInf);
	fwrite(&pTea->Inf.pwdHash, sizeof(Date), 1, fTea);
	fwrite(pTea->ClassList, 128, sizeof(int), fTea);
	return 1;
}
int SaveTeacherToFile(const char* FilePath)
{
	return SaveDataToFile(&AllTea,WriteTeaProc);
}

int ReadTeaProc(FILE* fTea, void* pData) {
	Teacher* pTea = (Teacher*)pData;
	fread(&pTea->Id, sizeof(TeaID), 1, fTea);
	//读入教师姓名
	ReadString(fTea, &pTea->name);
	//读入教师信息
	fread(&pTea->Inf.Sex, sizeof(sex), 1, fTea);
	fread(&pTea->Inf.Birthday, sizeof(Date), 1, fTea);
	ReadString(fTea, &pTea->Inf.Hometown);
	ReadString(fTea, &pTea->Inf.IDCard);
	ReadString(fTea, &pTea->Inf.Mail);
	ReadString(fTea, &pTea->Inf.PhoneNumber);
	ReadString(fTea, &pTea->Inf.Photo);
	ReadString(fTea, &pTea->Inf.Title);
	ReadString(fTea, &pTea->Inf.PersonalInf);
	fread(&pTea->Inf.pwdHash, sizeof(Date), 1, fTea);
	pTea->ClassList = (int*)calloc(128, 4);
	fread(pTea->ClassList, 128, sizeof(int), fTea);
	return 0;
}
int LoadTeacherFromFile(const char* FilePath)
{
	return LoadDataFromFile(&AllTea,ReadTeaProc , FilePath, sizeof(Teacher), HeadString);
}

/**结构体操作简化函数**/
TeacherInformation MakeTeacherInformation(sex _sex, const Date birthday, const char* hometown,
	const char* idcard, const char* mail, const char* PhoneNumber, const char* Photo, const char* Title, const char* PersonalInf)
{
	//先创建函数内临时变量TeaInf
	TeacherInformation TeaInf = { other };
	int len = 0;//len反复表示字符串长度
	TeaInf.Sex = _sex;//能直接赋值先赋值
	TeaInf.Birthday = birthday;
	
	/**
	对于文字，如果直接 = 赋值，会把字符串首地址赋值过去，在这里是要创建字符串副本
	所以要先根据字符串长度（strlen）申请内存，然后用memcpy赋值（memcpy在已知长度会略快于strcpy）
	**/
	len = strlen(hometown) + 1;
	TeaInf.Hometown = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.Hometown,return TeaInf)
	memcpy(TeaInf.Hometown, hometown, len);

	len = strlen(idcard) + 1;
	TeaInf.IDCard = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.IDCard, return TeaInf)
	memcpy(TeaInf.IDCard, idcard, len);

	len = strlen(mail) + 1;
	TeaInf.Mail = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.Mail, return TeaInf)
	memcpy(TeaInf.Mail, mail, len);

	len = strlen(PhoneNumber) + 1;
	TeaInf.PhoneNumber = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.PhoneNumber, return TeaInf)
	memcpy(TeaInf.PhoneNumber, PhoneNumber, len);

	len = strlen(Photo) + 1;
	TeaInf.Photo = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.Photo, return TeaInf)
	memcpy(TeaInf.Photo, Photo, len);
	
	len = strlen(Title) + 1;
	TeaInf.Title = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.Title, return TeaInf)
	memcpy(TeaInf.Title, Title, len);
	
	len = strlen(PersonalInf) + 1;
	TeaInf.PersonalInf = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.PersonalInf, return TeaInf)
	memcpy(TeaInf.PersonalInf, PersonalInf, len);
	
	return TeaInf;
	/**
	最后返回的是TeaInf，返回的时候会创建TeaInf的内存副本，然后函数内的TeaInf会被析构(结束其生命周期)，
	但是新的TeaInf会被返回，这期间只有字符串指针本身转移了，字符串本身在内存中不受影响
	**/
}

void DeConstructTeacherInformation(TeacherInformation *TeaInf)
{
	//全free掉，否则直接丢弃TeaInf里面的动态分配字符串不能自动销毁，会内存泄漏
	free(TeaInf->Hometown);
	free(TeaInf->IDCard);
	free(TeaInf->Mail);
	free(TeaInf->PhoneNumber);
	free(TeaInf->Photo);
	free(TeaInf->Title);
	free(TeaInf->PersonalInf);
}

Teacher MakeTeacher(TeacherInformation TeaInf,const char* name, const TeaID NewID)
{
	//详细代码解读参考MakeTeacherInformation
	Teacher Tea = { 0 };
	if (NewID < 1) {//判断TeaID合法否？
		return Tea;
	}
	//直接赋值
	Tea.Id = NewID;
	Tea.name = (char*)calloc(strlen(name) + 1, sizeof(char));
	CHECK_NULLANY(Tea.name,return Tea)
	strcpy(Tea.name, name);
	Tea.Inf = TeaInf; //这里可以直接赋值，因为结构体直接传播过去了，相当于是把字符串指针传递过去了
	Tea.ClassList = (int*)calloc(128, sizeof(int));
	memset(Tea.ClassList, 0, 128 * 4);
	return Tea;
}

void DeConstructTeacher(Teacher* teacher)
{
	//参DeConstructTeacherInformation
	DeConstructTeacherInformation(&teacher->Inf);
	free(teacher->name);
}

/**AllTeas数据库操作函数**/

int GetNumOfTeachers()
{
	return GetLength(&AllTea);
}

TeaID NewTeacher(Teacher* NewTea)
{

	if (NewTea->Id == 0) {
		return -1;
	}
	if (SeekTeacher(NewTea->Id) != NULL) {
		return -2;
	}
	return AddData(&AllTea, NewTea);
}

TeaID DeleteTeacher(TeaID Id)
{
	Teacher* pTea = SeekTeacher(Id);
	DeConstructTeacher(pTea);
	return DeleteData(&AllTea, GetTeacherIndex(Id));
}

int SeekTeaByIdProc(void* pData, void* args) {
	Teacher* pTea = (Teacher*)pData;
	int* Id = (int*)args;

	if (pTea->Id == *Id) {
		return 1;
	}
	return 0;
}
Teacher* SeekTeacher(TeaID SeekTea)
{
	return (Teacher*)SeekData(&AllTea, SeekTeaByIdProc, &SeekTea,0);
}
int GetTeacherIndex(TeaID SeekTea)
{
	int a = 0;
	SeekData(&AllTea, SeekTeaByIdProc, &SeekTea,&a);
	return a;
}

Teacher* IterateTeacher(int* Index)   //inline 内联函数(类比#define宏定义，减少对于栈空间的占用，提高程序效率) 
{
	return (Teacher*)IterateData(&AllTea, Index);
}

