#pragma once
/*
* project: 教务管理系统
* file   : DataManage.h
* Copyright <c> ciallo all right reserved.
*/
#ifndef DATAMANAGE
#define DATAMANAGE
#include "PublicHead.h"
#include "File.h"

typedef int Ident;

struct DataBlock {
	void* pData;//数据块基地址
	bool Delete;//删除标识
};

struct DataCluster{
	char* FilePath;//文件路径
	int Size;//数据块大小
	DataBlock* pData;//数据数组基地址
	char* FileHeader;//文件头
	int RealLength;//数组真实使用大小
	int AllocLength;//数组已分配内存大小
	int Length;//数组有效数据大小
	int PreAllocSize;//预分配粒度
	FILE* fp;//关联文件
};

typedef int (*SeekDataProc)(void* pData, void* args);
typedef int (*ReadDataProc)(FILE* fp, void *pData);
typedef int (*WriteDataProc)(FILE* fp, void* pData);

int InitialDataCluster(DataCluster* Datas, const char* path, int BlockSize, const char* FileHeader);
int FormattedDataCluster(DataCluster* Datas);
int LoadDataFromFile(DataCluster* Datas,ReadDataProc RDP, const char* path, int BlockSize, const char* FileHeader);
void SetAllocSize(DataCluster* Datas, int NewSize);
int SaveDataToFile(DataCluster* Datas,WriteDataProc WDP);

Ident AddData(DataCluster* Datas, void* pData);
int DeleteData(DataCluster* Datas, Ident id);
void* SeekData(DataCluster* Datas, SeekDataProc SeekPro,void* args, int* index);
void* IterateData(DataCluster* Datas, int* Index);
int GetLength(DataCluster* Datas);


#endif // !DATAMANAGE