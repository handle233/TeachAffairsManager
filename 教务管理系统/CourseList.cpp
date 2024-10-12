/*
* project: 教务管理系统
* file   : CourseList.h
* Copyright <c> ciallo all right reserved.
*/
#define COURSELIST
#include "CourseList.h"


Period NewPeriod(Time t1, Time t2) {
	Time Bef = { 0 }, Aft = { 0 };
	Period Per = { 0 };
	if (t1.Hour > t2.Hour) {
		Bef = t2; Aft = t1;
	}
	else if (t1.Hour < t2.Hour) {
		Bef = t1; Aft = t2;
	}
	else {
		if (t1.Min > t2.Min) {
			Bef = t2; Aft = t1;
		}
		else if (t1.Min < t2.Min) {
			Bef = t1; Aft = t2;
		}
		else {
			if (t1.Sec > t2.Sec) {
				Bef = t2; Aft = t1;
			}
			else if (t1.Sec < t2.Sec) {
				Bef = t1; Aft = t2;
			}
			else {
				Bef = t1; Aft = t2;
			}
		}
	}
	Per.Start = Bef, Per.End = Aft;
	return Per;
}
