/*
* project: 教务管理系统
* file   : Teacher.cpp
* Copyright <c> ciallo all right reserved.
*/
#include "Teacher.h"
#include "File.h"

/**关键全局变量**/
int NumOfTeachers;//AllTeas中保存教师数目
int AllocNumOfTea;//AllTeas内存分配数量            
Teacher* AllTeas = NULL;//AllTeas本体
/**最小分配粒度**/
const int MinAllocSize = 64;//一次预分配AllTeas大小
/**文件头标识**/
const char HeadString[HEAD_LENGTH] = "[Ciallo~ Teaching Affairs Manager][Teachersfile]";
/**文件操作相关**/
void InitialTeachers(const char* FilePath)
{
	//如果发现AllTeas已经分配，就不要重置，避免内存泄漏
	if (NumOfTeachers) {
		return;
	}
	//初始化所有和AllTeas有关的变量
	NumOfTeachers = 0;
	AllocNumOfTea = 0;
	AllTeas = NULL;
	//保存一份空文件，以免缺文件LoadTeacherFromFile报错
	FILE* fTea = SaveFile(FilePath, HeadString);
	//写入一个0表示文件保存Teachers数目为零
	fwrite(&NumOfTeachers, sizeof(int), 1, fTea);
	//关闭文件
	fclose(fTea);
}

int SaveTeacherToFile(const char* FilePath)
{
	//先打开保存文件的句柄(文件标识) 
	FILE* fTea = SaveFile(FilePath, HeadString);
	CHECK_NULL(fTea)//检查打开成功没

	//获取有效教师数量
	int RealNum = GetNumOfTeachers();
	//写入一个教师数目
	fwrite(&RealNum, sizeof(int), 1, fTea);
	
	//用大循环写入教师信息
	for (int a = 0; a < NumOfTeachers; a++) {//判断是否是删除教师，删除的就别写入了
		if (!AllTeas->remove) {
			int l = 0;
			fwrite(&AllTeas[a].Id, sizeof(TeaID), 1, fTea);

			WriteString(fTea, AllTeas[a].name);

			fwrite(&AllTeas[a].Inf.Sex, sizeof(sex), 1, fTea);
			fwrite(&AllTeas[a].Inf.Birthday, sizeof(Date), 1, fTea);
			WriteString(fTea, AllTeas[a].Inf.Hometown);
			WriteString(fTea, AllTeas[a].Inf.IDCard);
			WriteString(fTea, AllTeas[a].Inf.Mail);
			WriteString(fTea, AllTeas[a].Inf.PhoneNumber);
			WriteString(fTea, AllTeas[a].Inf.Photo);
			WriteString(fTea, AllTeas[a].Inf.Title);
			WriteString(fTea, AllTeas[a].Inf.PersonalInf);
		}
	}
	
	//关闭文件
	fclose(fTea);
	//重置AllTeas以便下次LoadTeachersFromFile不要褒姒
	free(AllTeas);
	AllTeas = NULL;
	NumOfTeachers = 0;
	AllocNumOfTea = 0;

	return 0;
}

int LoadTeacherFromFile(const char* FilePath)
{
	//先判断AllTeas，如果已经分配就拒绝读取，避免内存泄漏(上一次保存后，没有任何操作，重复执行此函数，就会造成内存泄露)
	CHECK_NULLANY(AllTeas,)else { return -1; }
	//读入文件头缓冲区
	char ReadHead[HEAD_LENGTH] = "";
	//打开文件 
	FILE* fTea = OpenFile(FilePath, "r", ReadHead);
	//检查打开是否失败
	CHECK_NULL(fTea)

	//memcmp(比较内存区域的前n个字节是否相同)检查文件头标识是否匹配
	if (memcmp(ReadHead, HeadString, HEAD_LENGTH) != 0) {
		//不匹配输出报错信息退出
		printf("Error: File head is Uncompatible! Suspect invation.\n");
		return -1;
	}
	//读入NumOfTeacher方便申请内存
	fread(&NumOfTeachers, sizeof(int), 1, fTea);
	//这里预申请直接和数目相等
	AllocNumOfTea = NumOfTeachers;
	//分配内存
	AllTeas = (Teacher*)malloc(sizeof(Teacher) * NumOfTeachers);
	if (!AllTeas) {//分配出错,报错退出
		printf("Error: Failed to alloc memory.Allocing:%d * %d\n",NumOfTeachers,(int)sizeof(Teacher));
		return -1;
	}
	//用一个循环反复读入Teachers信息拼凑成Teacher
	for (int a = 0; a < NumOfTeachers; a++) {     
		//读入教师工号 
		fread(&AllTeas[a].Id, sizeof(TeaID), 1, fTea);
		//读入教师姓名
		ReadString(fTea, &AllTeas[a].name);
		//读入教师信息
		fread(&AllTeas[a].Inf.Sex, sizeof(sex), 1, fTea);
		fread(&AllTeas[a].Inf.Birthday, sizeof(Date), 1, fTea);
		ReadString(fTea, &AllTeas[a].Inf.Hometown);
		ReadString(fTea, &AllTeas[a].Inf.IDCard);
		ReadString(fTea, &AllTeas[a].Inf.Mail);
		ReadString(fTea, &AllTeas[a].Inf.PhoneNumber);
		ReadString(fTea, &AllTeas[a].Inf.Photo);
		ReadString(fTea, &AllTeas[a].Inf.Title);
		ReadString(fTea, &AllTeas[a].Inf.PersonalInf);
	}

	fclose(fTea);
	return 0;
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
	//排除所有removed的教师
	int removed = 0;
	for (int a = 0; a < NumOfTeachers; a++) {//遍历AllTeas
		if (AllTeas[a].remove == true) {//只要发现remove标识自动把remove加一
			removed++;
		}
	}
	return NumOfTeachers - removed;//总长-删除的数目=有效长度
}

TeaID NewTeacher(Teacher* NewTea)
{
	//先检查TeaID合法性
	if (NewTea->Id < 1) {
		return -1;
	}
	//这一行比较难理解，请参考CHECK_NULLANY宏一起看，整体的意思是如果发现AllTeas还没分配就分配内存
	CHECK_NULLANY(AllTeas, //这一行是判断NULL条件，也就是判断AllTeas是否为空
	AllTeas = (Teacher*)calloc(MinAllocSize, sizeof(Teacher)); AllocNumOfTea += MinAllocSize;) //对应宏中的r，如果为空，一次分配最小粒度数量的空间 
	else {//这里利用宏展开直接接上，表示AllTeas不为零的情况
	
		//如果预分配存储已经耗尽，就继续扩分配
		if (NumOfTeachers >= AllocNumOfTea) {
			Teacher* NewTeas = (Teacher*)realloc(AllTeas, sizeof(Teacher) * ((long)AllocNumOfTea + MinAllocSize));
			if (!NewTeas) {
				printf("Error: Failed to alloc memory.Allocing:%d * %d\n", AllocNumOfTea+MinAllocSize, (int)sizeof(Teacher));
				return -1;//判断内存分配失败否？
			}
			//realloc如果分配成功，就把AllTeas换为新分配的NewStus，旧的AllTeas会被realloc自动销毁
			AllTeas = NewTeas;
			//已分配大小记得加上
			AllocNumOfTea += MinAllocSize;
		}
	}
	//这一部分是负责教师添加
	Teacher* atea = NULL;
	//首先判断工号有无重号
	for (int i = -1; (atea = IterateTeacher(&i)) != NULL;) {//IterateTeacher遍历教师
		if (atea->Id == NewTea->Id) {//一旦发现和新教师重号，直接return
			return -2;
		}
	}
	//没问题的话在数组末尾把新的Teachers复制过去
	memcpy(AllTeas + (NumOfTeachers), NewTea, sizeof(Teacher));
	NumOfTeachers++;//教师数量加一
	//返回教师工号 
	return AllTeas->Id;
}

TeaID DeleteTeacher(TeaID Id)
{
	Teacher* TheTea = SeekTeacher(Id);////先寻找教师，返回数组中Teacher结构体地址
	CHECK_NULL(TheTea)//判断找到否？
	TheTea->remove = true;//把删除位置一
	return TheTea->Id;//返回ID
}

Teacher* SeekTeacher(TeaID SeekTea)
{
	//先判断AllTeas有没有分配，避免访问空指针
	CHECK_NULLPTR(AllTeas)
	if (SeekTea < 1) {//检查TeaID合法性
		return NULL;
	}
	for (int a = 0; a < NumOfTeachers; a++) { //遍历AllTeas
		if (AllTeas[a].Id == SeekTea && AllTeas[a].remove==false) { //如果找到该教师并且不是已删除教师 
			return AllTeas+a;//直接返回在数组中地址
		}
	}
	return NULL;//找不到返回空
}

inline Teacher* IterateTeacher(int* Index)   //inline 内联函数(类比#define宏定义，减少对于栈空间的占用，提高程序效率) 
{
	for ((*Index)++; //Index进来先自增，表示从上一个教师跳到下一个
	AllTeas[*Index].remove == true && *Index<NumOfTeachers; //然后判断下一名有没有被删除，Index有没有超出AllTeas数量
	(*Index)++);  //如果被删除那么for循环条件成立，Index再跳一个
	//如果for循环结束要么找到了，要么找完了
	if (*Index >= NumOfTeachers) {//判断是不是找完了
		return NULL;
	}
	//直接返回在数组中地址
	return AllTeas + *Index;
}

