#pragma once
/*
* project: 教务管理系统
* file   : Teacher.h
* Copyright <c> ciallo all right reserved.
*/
#define TEACHER
#include "PublicHead.h"

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
//保存读取部分教师全体信息
extern Teacher* AllTeas;

/**公开函数**/
/*
初始化教师文件，创建一个空教师文件，重置AllStus
参数1 文件路径
*/
void InitialTeachers(const char* FilePath);
/*
从文件中加载教师，分配AllStus
参数1 文件路径
返回值 -1:文件打开失败或者文件头出错，-2:内存分配失败
*/
int LoadTeacherFromFile(const char *FilePath);
/*
将读取教师信息保存到文件中，并且释放AllStus
参数1 文件路径
返回值 -1:文件打开失败
*/
int SaveTeacherToFile(const char* FilePath);

/**结构体操作简化函数**/
/*
快速创建TeacherInformation结构体，免去手动Alloc
参数N 结构体信息，参见TeacherInformation
返回值 创建好的TeacherInformation 错误返回性别为others的结构体
*/
TeacherInformation MakeTeacherInformation(sex _sex, const Date birthday, const char* hometown,
	const char* idcard, const char* mail, const char* PhoneNumber, const char* Photo,const char* Title,const char* PersonalInf);
/*
快速析构TeacherInformation结构体，免去手动Free  折构函数(收回对象申请的空间) 
参数1 TeachertInformation指针
*/
void DeConstructTeacherInformation(TeacherInformation *TeaInf);
/*
快速创建Teacher结构体，免去手动Alloc
参数1 TeacherInformation
参数2 教师姓名
参数3 分配的教师ID(必须大于0)
返回值 创建好的Teacher 错误返回名字为nullptr的结构体
*/
Teacher MakeTeacher(TeacherInformation TeaInf, const char* name, const TeaID NewID);
/*
快速析构Teacher结构体，免去手动Free
参数1 Teacher指针
*/
void DeConstructTeacher(Teacher* teacher);

/**AllStus数据库操作函数**/
/*
获取AllStus中有效Teachers数量
返回值 Teachers数量
*/
int GetNumOfTeachers();
/*
往数据库添加一名教师
参数1 Teacher结构体(TeaID必须大于0)
返回值 Teacher ID -1:分配内存失败或参数不合法 -2:分配教师D重复 
*/
TeaID NewTeacher(Teacher* NewTea);
/*
往数据库删除一名教师
参数1 TeacherID(TeaID必须大于0)
返回值 Teacher ID -1:分配内存失败或参数不合法
*/
TeaID DeleteTeacher(TeaID Id);
/*
向数据库查找一名教师
参数1 Teacher结构体(TeaID必须大于0)
返回值 Teacher ID NULL:没找到教师或参数不合法，也可能是AllStus没有初始化
*/
Teacher* SeekTeacher(TeaID SeekTea);
/*
枚举数据库中教师
参数1 Index指针，用于标识枚举进度，每次Iterate函数会加一，初始应该设定index为-1
返回值 Teacher ID -1:分配内存失败或参数不合法 -2:分配教师ID重复
*/
inline Teacher* IterateTeacher(int *Index);
