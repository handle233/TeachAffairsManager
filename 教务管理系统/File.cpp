/*
* project: 教务管理系统
* file   : File.cpp
* Copyright <c> ciallo all right reserved.
*/
#include "File.h"

FILE* OpenFile(const char* Path, const  char *Mode, _IN_ char* Header)
{
	FILE* fp = fopen(Path, Mode);
	if (!fp) {
		printf("Error: FileOpenFailed\nPATH:%s\n\n", Path);
		return NULL;
	}
	fread(Header, HEAD_LENGTH, 1, fp);

	return fp;
}
FILE* SaveFile(const char* Path, _OUT_ const char* Header)
{
	FILE* fp = fopen(Path, "wt+");
	if (!fp) {
		printf("Error: FileOpenFailed\nPATH:%s\n\n", Path);
		return NULL;
	}
	fwrite(Header, HEAD_LENGTH, 1, fp);
	return fp;
}

int WriteString(FILE* fp, _OUT_ const char* str) {
	int l = strlen(str);
	fwrite(&l, sizeof(int), 1, fp);
	fwrite(str, l + 1, 1, fp);
	return l;
}

int ReadString(FILE* fp, _IN_ char** str) {
	int l = 0;
	fread(&l, sizeof(int), 1, fp);
	*str = (char*)calloc(l + 1, sizeof(char));
	fread(*str, l + 1, 1, fp);
	return l;
}

