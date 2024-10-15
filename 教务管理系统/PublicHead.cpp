/*
* project: 教务管理系统
* file   : PublicHead.h
* Copyright <c> ciallo all right reserved.
*/
#include "PublicHead.h"
#include "Windows.h"
void gotoxy(int x, int y) {
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置
}