/*
* project: �������ϵͳ
* file   : PublicHead.h
* Copyright <c> ciallo all right reserved.
*/
#include "PublicHead.h"
#include "Windows.h"
void gotoxy(int x, int y) {
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// ��ȡ��׼����豸���
	SetConsoleCursorPosition(hOut, pos);//���������ֱ���ָ���ĸ����壬����λ��
}