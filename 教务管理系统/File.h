#pragma once
/*
* project: 教务管理系统
* file   : File.h
* Copyright <c> ciallo all right reserved.
*/
#define FILEMANAGE
#include "PublicHead.h"

#define HEAD_LENGTH 64

FILE* OpenFile(const char* Path, const char* Mode, _IN_ char* Header);
FILE* SaveFile(const char* Path, _OUT_ const char* Header);
int WriteString(FILE* fp,_OUT_ const char* str);
int ReadString(FILE* fp, _IN_ char** str);