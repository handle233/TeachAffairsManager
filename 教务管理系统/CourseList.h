#pragma once
/*
* project: 教务管理系统
* file   : CourseList.h
* Copyright <c> ciallo all right reserved.
*/
#define COURSELIST
#include "PublicHead.h"
#include "File.h"

/**简单类型定义**/
typedef int CourseID;

/**简单结构体定义**/
struct Time {
	int Hour, Min, Sec;
};
struct Period {
	Time Start, End;
};
//课程表主结构体定义
struct Courses {
};

/**Period操作函数**/
Period NewPeriod(Time t1, Time t2);
