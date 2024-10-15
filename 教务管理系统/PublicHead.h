#pragma once
/*
* project: 教务管理系统
* file   : PublicHead.h
* Copyright <c> ciallo all right reserved.
*/
//包含公共头文件
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
//取消C4996警告，以便scanf等函数不报错
#pragma warning(disable:4996)
#pragma warning(disable:4267)
//表示该参数用于接受函数的输出
#define _IN_
//表示该参数用于接受函数的输入
#define _OUT_
//定义NULL
#ifndef NULL
#define NULL (void*)0
#endif // !NULL
//定义true 和 false
#ifndef true
#define true 1
#endif // !true
#ifndef false
#define false 0
#endif // !false
//小型宏脚本
#ifndef CHECK_NULL
/**CHECKNULL系宏是一种方便的判断变量是否为空的方法**/
/**
宏可以带有参数，使用的时候在参数处填入的东西都会被直接原封不动复制到对应位置
**/
//这一个就是检查q代表的东西是不是null，是的话返回-1，注意这个返回是直接在函数里返回，宏本身没有返回值
#define CHECK_NULL(q) if(q==NULL){return -1;}
//这一个就是检查q代表的东西是不是null，是的话返回NULL，注意这个返回是直接在函数里返回，宏本身没有返回值
#define CHECK_NULLPTR(q) if(q==NULL){return NULL;}
//这一个就是检查q代表的东西是不是null，是的话就执行r代表的代码。
//r可以不填，表示执行空代码，还可以在后面加入else，这样就会把else和前面的这个if关联，表示如果q不是NULL执行的情况。
#define CHECK_NULLANY(q,r) if(q==NULL){r;}
#endif // !CHECK_NULL
/**版本控制变量**/
#ifndef VERSION_CONTROL
#define VERSION_CONTROL

#define VERSION 1

#define WEEKDAY 7
/**简单类型定义**/
//性别，看不懂的抬走
enum sex { male, female, other };
enum Weekday { Sun, Mon, Tue, Wed, Thu, Fri, Sat };
void gotoxy(int x, int y);
//日期
struct Date
{
	int Year; int Month; int Day;
};
#endif // !版本控制变量