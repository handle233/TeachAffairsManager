#pragma once
/*
* project: 教务管理系统
* file   : Student.h
* Copyright <c> ciallo all right reserved.
*/
#define STUDENT
#include "PublicHead.h"
#include "CourseList.h"
#include "File.h"

/**简单类型定义**/
typedef int StuID;

/**结构体类型定义**/

//学生个人信息结构体
struct StudentInformation {
	sex Sex;//性别
	char* Hometown;//家乡
	Date Birthday;//生日
	char* IDCard;//身份证
	char *Photo;//照片路径
	char* Mail;//邮箱
	char* PhoneNumber;//电话
};

// 学生结构体
struct Student
{
	int Id;//学号
	char* name;//名字
	StudentInformation Inf;//学生个人信息
	bool remove;//删除标识
	Courses CS;
};

/**全局变量定义**/
//保存读取部分学生全体信息
extern Student* AllStus;

/**公开函数**/

/**文件操作相关**/
/*
初始化学生文件，创建一个空学生文件，重置AllStus
参数1 文件路径
*/
void InitialStudents(const char* FilePath);
/*
从文件中加载学生，分配AllStus
参数1 文件路径
返回值 -1:文件打开失败或者文件头出错，-2:内存分配失败
*/
int LoadStudentFromFile(const char *FilePath);
/*
将读取学生信息保存到文件中，并且释放AllStus
参数1 文件路径
返回值 -1:文件打开失败
*/
int SaveStudentToFile(const char* FilePath);

/**结构体操作简化函数**/
/*
快速创建StudentInformation结构体，免去手动Alloc
参数N 结构体信息，参见StudentInformation
返回值 创建好的StudentInformation 错误返回性别为others的结构体
*/
StudentInformation MakeStudentInformation(sex _sex,const Date birthday, const char* hometown,
	const char* idcard, const char* mail, const char* PhoneNumber, const char* Photo);
/*
快速析构StudentInformation结构体，免去手动Free
参数1 StudentInformation指针
*/
void DeConstructStudentInformation(StudentInformation *StuInf);
/*
快速创建Student结构体，免去手动Alloc
参数1 StudentInformation
参数2 学生姓名
参数3 分配的学生ID(必须大于0)
返回值 创建好的Student 错误返回名字为nullptr的结构体
*/
Student MakeStudent(StudentInformation StuInf, const char* name, const StuID NewID);
/*
快速析构Student结构体，免去手动Free
参数1 Student指针
*/
void DeConstructStudent(Student* student);

/**AllStus数据库操作函数**/
/*
获取AllStus中有效Students数量
返回值 Students数量
*/
int GetNumOfStudents();
/*
往数据库添加一名学生
参数1 Student结构体(StuID必须大于0)
返回值 Student ID -1:分配内存失败或参数不合法 -2:分配学生ID重复 
*/
StuID NewStudent(Student* NewStu);
/*
往数据库删除一名学生
参数1 StudentID(StuID必须大于0)
返回值 Student ID -1:分配内存失败或参数不合法
*/
StuID DeleteStudent(StuID Id);
/*
向数据库查找一名学生
参数1 Student结构体(StuID必须大于0)
返回值 Student ID NULL:没找到学生或参数不合法，也可能是AllStus没有初始化
*/
Student* SeekStudent(StuID SeekStu);
/*
枚举数据库中学生
参数1 Index指针，用于标识枚举进度，每次Iterate函数会加一，初始应该设定index为-1
返回值 Student ID -1:分配内存失败或参数不合法 -2:分配学生ID重复
*/
inline Student* IterateStudent(int *Index);
