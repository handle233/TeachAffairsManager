#include "DataManage.h"

int InitialDataCluster(DataCluster* Datas,const char* path, int BlockSize, const char* FileHeader)
{
	Datas->PreAllocSize = 64;
	Datas->RealLength = 0;
	Datas->pData = NULL;
	Datas->AllocLength = 0;
	Datas->Length = 0;

	Datas->FileHeader = (char*)malloc(HEAD_LENGTH);
	CHECK_NULLPTR(Datas->FileHeader);
	memcpy(Datas->FileHeader, FileHeader, HEAD_LENGTH);

	Datas->FilePath = (char*)malloc(strlen(path) + 1);
	CHECK_NULLPTR(Datas->FilePath);
	strcpy(Datas->FilePath, FileHeader);

	Datas->Size = BlockSize;

	FILE*fp = SaveFile(Datas->FilePath, Datas->FileHeader);
	int a = 1;
	fwrite(&a, sizeof(int), 1, fp);
	fclose(fp);
	return 1;
}

int LoadDataFromFile(DataCluster* Datas, ReadDataProc RDP, const char* path, int BlockSize, const char* FileHeader)
{
	InitialDataCluster(Datas, path, BlockSize, FileHeader);


	char ReadHead[HEAD_LENGTH] = "";
	Datas->fp = OpenFile(Datas->FilePath, "r", ReadHead);
	CHECK_NULLANY(memcmp(ReadHead, Datas->FileHeader, HEAD_LENGTH))else {
		return 1;
	}
	fread(&Datas->Length, sizeof(int), 1, Datas->fp);
	Datas->RealLength = Datas->AllocLength = Datas->Length;

	Datas->pData = (DataBlock*)calloc(Datas->Length, sizeof(DataBlock));
	CHECK_NULLPTR(Datas->pData);

	for (int a = 0; a < Datas->Length; a++) {
		RDP(Datas->fp, Datas->pData[a].pData);
		Datas->pData[a].Delete = false;
	}

	return 1;
}

void SetAllocSize(DataCluster* Datas, int NewSize)
{
	Datas->PreAllocSize = NewSize;
}

int SaveDataToFile(DataCluster* Datas, WriteDataProc WDP)
{

	fclose(Datas->fp);
	Datas->fp = SaveFile(Datas->FilePath, Datas->FileHeader);
	CHECK_NULLPTR(Datas->fp);
	int l = GetLength(Datas);

	for (int a = 0; a < l;a++) {
		WDP(Datas->fp, Datas->pData[a].pData);
	}
	return 0;
}

ID AddData(DataCluster* Datas,void* pData)
{
	CHECK_NULLPTR(Datas); CHECK_NULLPTR(pData);

	CHECK_NULLANY(Datas->pData,
		Datas->pData = (DataBlock*)calloc(Datas->PreAllocSize, sizeof(DataBlock));
		CHECK_NULLPTR(Datas->pData);
		Datas->AllocLength += Datas->PreAllocSize;
	)else {
		if (Datas->RealLength >= Datas->AllocLength) {
			DataBlock* pNew = (DataBlock*)realloc(Datas->pData, (Datas->AllocLength + Datas->PreAllocSize) * sizeof(DataBlock));
			CHECK_NULLPTR(pNew);
			Datas->pData = pNew;
			Datas->AllocLength += Datas->PreAllocSize;
		}
	}

	(Datas->pData + Datas->RealLength)->pData = malloc(Datas->Size);
	CHECK_NULLPTR((Datas->pData + Datas->RealLength)->pData);
	(Datas->pData + Datas->RealLength)->Delete = false;
	memcpy((Datas->pData+Datas->RealLength)->pData, pData, Datas->Size);
	Datas->RealLength++;
	Datas->Length++;

	return Datas->RealLength - 1;
}

int DeleteData(DataCluster* Datas, ID id)
{
	Datas->Length--;
	Datas->pData[id].Delete = true;
	return id;
}

void* SeekData(DataCluster* Datas,SeekDataProc SeekPro)
{
	for (int a = 0; a < Datas->RealLength; a++) {
		if (Datas->pData[a].Delete == false && SeekPro(Datas->pData[a].pData) == true) {
			return Datas->pData[a].pData;
		}
	}
}

void* IterateData(DataCluster* Datas, int* Index)
{
	for ((*Index)++;//Index进来先自增，表示从上一个跳到下一个
		Datas->pData[*Index].Delete == true && *Index < Datas->RealLength; //然后判断下一名有没有被删除，Index有没有超出AllStus数量
		(*Index)++);//如果被删除那么for循环条件成立，Index再跳一个

	//如果for循环结束要么找到了，要么找完了
	if (*Index >= Datas->RealLength) {//判断是不是找完了
		return NULL;
	}
	//直接返回在数组中地址
	return (Datas->pData + *Index)->pData;
}

int GetLength(DataCluster* Datas)
{
	if (Datas->Length > 0) {
		return Datas->Length;
	}
	int l = Datas->RealLength;

	for (int a = 0; a < Datas->RealLength; a++) {
		if (Datas->pData[a].Delete) {
			l--;
		}
	}

	return l;
}
