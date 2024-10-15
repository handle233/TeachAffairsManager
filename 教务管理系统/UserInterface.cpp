#pragma once
/*
* project: 教务管理系统
* file   : UserInterface.cpp
* Copyright <c> ciallo all right reserved.
*/
#include "UserInterface.h"

void PrintBorder() {
	char cmd[255] = "";
	sprintf(cmd, "mode con cols=%d lines=%d", WIDTH, HEIGHT + 1);
	system(cmd);
	NewBorder(0, 0, WIDTH, HEIGHT);
}

void NewBorder(int x, int y, int Width, int Height)
{
	gotoxy(x, y);
	char* s_Head = (char*)calloc(Width + 1, sizeof(char));
	memset(s_Head, BLOCK, Width * sizeof(char));
	s_Head[Width] = '\0';
	printf(s_Head);
	for (int a = 0; a < HEIGHT - 2; a++) {
		gotoxy(x, y + a + 1);
		putchar(BLOCK);
		gotoxy(x + Width - 1, y + a + 1);
		putchar(BLOCK);
	}
	gotoxy(x, y + Height - 1);
	printf(s_Head);
	gotoxy(x + 1, y + 1);
}

void ShowTitle(const char* title)
{
	int x = (WIDTH - 2) / 2 - strlen(title) / 2;
	for (int a = 0; a < x; a++)putchar(' ');
	printf("%s\n#", title);

}

char* InputBox(const char* Tip, int pwd) {
	printf("%s:", Tip);
	int AllocCount = 1,index=0;
	char* str = (char*)calloc(AllocCount * 64, sizeof(char));
	memset(str, 0, AllocCount * 64);
	int ch = getch();
	while (ch != 13) {
		if (ch >= '0' && ch <= '9' ||
			ch >= 'a' && ch <= 'z' ||
			ch >= 'A' && ch <= 'Z') {
			str[index] = ch;
			index++;
			printf("%c", ch);
		}
		else {
			switch (ch) {
			case 8:
				printf("\b \b");
				str[index] = 0;
				index--;
				break;
			default:
				printf("%d ", ch);
			}
		}
		ch = getch();
	}
	return str;
}