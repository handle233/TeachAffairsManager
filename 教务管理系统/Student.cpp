/*
* project: 教务管理系统
* file   : Student.cpp
* Copyright <c> ciallo all right reserved.
*/
#include "Student.h"

/**关键全局变量**/
int NumOfStudents;//AllStus中保存学生数目
int AllocNumOfStu;//AllStus内存分配数量
Student* AllStus = NULL;//AllStus本体
/**最小分配粒度**/
const int MinAllocSize = 64;//一次预分配AllStus大小
/**文件头标识**/
const char HeadString[HEAD_LENGTH] = "[Ciallo~ Teaching Affairs Manager][Studentsfile]";
/**文件操作相关**/
void InitialStudents(const char* FilePath)
{
	//如果发现AllStus已经分配，就不要重置，避免内存泄漏
	if (NumOfStudents) {
		return;
	}
	//初始化所有和AllStus有关的变量
	NumOfStudents = 0;
	AllocNumOfStu = 0;
	AllStus = NULL;
	//保存一份空文件，以免缺文件LoadStudentFromFile报错
	FILE* fStu = SaveFile(FilePath, HeadString);
	//写入一个0表示文件保存Students数目为零
	fwrite(&NumOfStudents, sizeof(int), 1, fStu);
	//关闭文件
	fclose(fStu);
}

int LoadStudentFromFile(const char* FilePath)
{
	//先判断AllStus，如果已经分配就拒绝读取，避免内存泄漏
	CHECK_NULLANY(AllStus,)else { return -1; }
	//读入文件头缓冲区
	char ReadHead[HEAD_LENGTH] = "";
	//打开文件
	FILE* fStu = OpenFile(FilePath, "r", ReadHead);
	//检查打开是否失败
	CHECK_NULL(fStu)

	//memcmp检查文件头是否匹配
	if (memcmp(ReadHead, HeadString, HEAD_LENGTH) != 0) {
		//不匹配输出报错信息退出
		printf("Error: File head is Uncompatible! Suspect invation.\n");
		return -1;
	}
	//读入NumOfStudent方便申请内存
	fread(&NumOfStudents, sizeof(int), 1, fStu);
	//这里预申请直接和数目相等
	AllocNumOfStu = NumOfStudents;

	//分配内存
	AllStus = (Student*)malloc(sizeof(Student) * NumOfStudents);
	CHECK_NULLANY(AllStus, //分配出错报错退出
		printf("Error: Failed to alloc memory.Allocing:%d * %d\n", NumOfStudents, (int)sizeof(Student));return -2;)
	
	//用一个循环反复读入Students信息拼凑成Student
	for (int a = 0; a < NumOfStudents; a++) {
		//读入ID
		fread(&AllStus[a].Id, sizeof(StuID), 1, fStu);
		//读入名字
		ReadString(fStu, &AllStus[a].name);
		//读入学生信息
		fread(&AllStus[a].Inf.Sex, sizeof(sex), 1, fStu);
		fread(&AllStus[a].Inf.Birthday, sizeof(Date), 1, fStu);
		ReadString(fStu, &AllStus[a].Inf.Hometown);
		ReadString(fStu, &AllStus[a].Inf.IDCard);
		ReadString(fStu, &AllStus[a].Inf.Mail);
		ReadString(fStu, &AllStus[a].Inf.PhoneNumber);
		ReadString(fStu, &AllStus[a].Inf.Photo);
	}
	//关闭文件
	fclose(fStu);
	return 0;
}

int SaveStudentToFile(const char* FilePath)
{
	//先打开保存文件的句柄
	FILE* fStu = SaveFile(FilePath, HeadString);
	CHECK_NULL(fStu)//检查打开成功没

	//获取有效学生数量，别溢出了
	int RealNum = GetNumOfStudents();
	//先写入一个学生数目
	fwrite(&RealNum, sizeof(int), 1, fStu);

	//用大循环写入学生信息
	for (int a = 0; a < NumOfStudents; a++) {
		if (!AllStus->remove) {//判断是否是删除学生，删除的就别写入了
			//里面的不用注释也能看明白
			int l = 0;
			fwrite(&AllStus[a].Id, sizeof(StuID), 1, fStu);

			WriteString(fStu, AllStus[a].name);

			fwrite(&AllStus[a].Inf.Sex, sizeof(sex), 1, fStu);
			fwrite(&AllStus[a].Inf.Birthday, sizeof(Date), 1, fStu);
			WriteString(fStu, AllStus[a].Inf.Hometown);
			WriteString(fStu, AllStus[a].Inf.IDCard);
			WriteString(fStu, AllStus[a].Inf.Mail);
			WriteString(fStu, AllStus[a].Inf.PhoneNumber);
			WriteString(fStu, AllStus[a].Inf.Photo);
		}
	}
	//关闭文件
	fclose(fStu);
	//重置AllStus以便下次LoadStudentsFrom File不要褒姒
	free(AllStus);
	AllStus = NULL;
	NumOfStudents = 0;
	AllocNumOfStu = 0;

	return 0;
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
	//排除所有removed的学生
	int removed = 0;
	for (int a = 0; a < NumOfStudents; a++) {//便利AllStus
		if (AllStus[a].remove == true) {//只要发现remove标识自动把remove加一
			removed++;
		}
	}
	return NumOfStudents - removed;//总长-删除的数目=有效长度
}

StuID NewStudent(Student* NewStu)
{
	//先检查StuID合法性
	if (NewStu->Id < 1) {
		return -1;
	}
	//这一行比较难理解，请参考CHECK_NULLANY宏一起看，整体的意思是如果发现AllStus还没分配就分配内存
	CHECK_NULLANY(AllStus, //这一行是判断NULL条件，也就是判断AllStus是否为空
		AllStus = (Student*)calloc(MinAllocSize, sizeof(Student)); //这两行是判断结果执行，也就是如果为空那么做什么
	AllocNumOfStu += MinAllocSize;)//一次分配一个MinAllocSize大小
	else {//这里利用宏展开直接接上，表示AllStus不为零的情况

		//如果预分配存储已经耗尽，就继续扩分配
		if (NumOfStudents >= AllocNumOfStu) {
			Student* NewStus = (Student*)realloc(AllStus, sizeof(Student) * ((long)AllocNumOfStu + MinAllocSize));
			CHECK_NULLANY(NewStus, 
				printf("Error: Failed to alloc memory.Allocing:%d * %d\n", AllocNumOfStu + MinAllocSize, (int)sizeof(Student));
			return -1;)//判断内存分配失败否？

			//realloc如果分配成功，就把AllStus换为新分配的NewStus，旧的AllStus会被realloc自动销毁
			AllStus = NewStus;
			//已分配大小记得加上
			AllocNumOfStu += MinAllocSize;
		}
	}

	//这一部分是负责学生添加
	Student* astu = NULL;
	//首先判断学号有无重号
	for (int i = -1; (astu = IterateStudent(&i)) != NULL;) {//IterateStudent遍历学生
		if (astu->Id == NewStu->Id) {//一旦发现和新生重号，直接return
			return -2;
		}
	}
	//没问题的话在数组末尾把新的Student复制过去
	memcpy(AllStus + (NumOfStudents), NewStu, sizeof(Student));
	NumOfStudents++;//学生数量加一
	//返回ID
	return AllStus->Id;
}

StuID DeleteStudent(StuID Id)
{
	Student* TheStu = SeekStudent(Id);//先寻找这个学生，返回数组中Student结构体地址
	CHECK_NULL(TheStu)//判断找到否？
	TheStu->remove = true;//把删除位置一
	return TheStu->Id;//返回ID
}

Student* SeekStudent(StuID SeekStu)
{
	//先判断AllStus有没有分配，避免访问空指针
	CHECK_NULLPTR(AllStus)
	if (SeekStu < 1) {//检查StuID合法性
		return NULL;
	}
	for (int a = 0; a < NumOfStudents; a++) {//遍历AllStus
		if (AllStus[a].Id == SeekStu && AllStus[a].remove==false) {//如果找到该学生并且不是已删除学生
			return AllStus+a;//直接返回在数组中地址
		}
	}
	return NULL;//找不到返回空
}

inline Student* IterateStudent(int* Index)
{
	for ((*Index)++;//Index进来先自增，表示从上一个学生跳到下一个
		AllStus[*Index].remove == true && *Index<NumOfStudents; //然后判断下一名有没有被删除，Index有没有超出AllStus数量
		(*Index)++);//如果被删除那么for循环条件成立，Index再跳一个

	//如果for循环结束要么找到了，要么找完了
	if (*Index >= NumOfStudents) {//判断是不是找完了
		return NULL;
	}
	//直接返回在数组中地址
	return AllStus + *Index;
}
