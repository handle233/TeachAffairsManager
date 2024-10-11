#pragma once
/*
* project: 教务管理系统
* file   : CourseList.h
* Copyright <c> ciallo all right reserved.
*/
#define COURSELIST
#include "PublicHead.h"

/**简单结构体定义**/
struct Time {
	int Hour, Min, Sec;
};
struct Period {
	Time Start, End;
};
