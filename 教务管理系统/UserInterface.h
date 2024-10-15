#pragma once
/*
* project: 教务管理系统
* file   : UserInterface.h
* Copyright <c> ciallo all right reserved.
*/
#ifndef USERINTERFACE
#define USERINTERFACE
#include "PublicHead.h"

#define WIDTH 120
#define HEIGHT 30
#define BLOCK '#'

void PrintBorder();
void NewBorder(int x, int y, int Width, int Height);
void ShowTitle(const char* title);
char* InputBox(const char* Tip, int pwd);
void ShowText(const char* Label);
#endif