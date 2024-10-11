/*
* project: 教务管理系统
* file   : Teacher.cpp
* Copyright <c> ciallo all right reserved.
*/
#include "Teacher.h"

int NumOfTeachers;
int AllocNumOfTea;            
Teacher* AllTeas = NULL;
/**最小分配粒度**/
const int MinAllocSize = 64;

const char HeadString[HEAD_LENGTH] = "[Ciallo~ Teaching Affairs Manager][Teachersfile]";

void InitialTeachers(const char* FilePath)
{
	if (NumOfTeachers) {
		return;
	}
	NumOfTeachers = 0;
	AllocNumOfTea = 0;
	AllTeas = NULL;
	FILE* fTea = SaveFile(FilePath, HeadString);
	fwrite(&NumOfTeachers, sizeof(int), 1, fTea);
	fclose(fTea);
}

int LoadTeacherFromFile(const char* FilePath)
{
	CHECK_NULLANY(AllTeas,)else { return -1; }
	char ReadHead[HEAD_LENGTH] = "";
	FILE* fTea = OpenFile(FilePath, "r", ReadHead);
	CHECK_NULL(fTea)

	if (memcmp(ReadHead, HeadString, HEAD_LENGTH) != 0) {
		printf("Error: File head is Uncompatible! Suspect invation.\n");
		return -1;
	}
	fread(&NumOfTeachers, sizeof(int), 1, fTea);
	AllocNumOfTea = NumOfTeachers;

	AllTeas = (Teacher*)malloc(sizeof(Teacher) * NumOfTeachers);
	if (!AllTeas) {
		printf("Error: Failed to alloc memory.Allocing:%d * %d\n",NumOfTeachers,(int)sizeof(Teacher));
		return -1;
	}
	
	for (int a = 0; a < NumOfTeachers; a++) {     
		fread(&AllTeas[a].Id, sizeof(TeaID), 1, fTea);

		ReadString(fTea, &AllTeas[a].name);

		fread(&AllTeas[a].Inf.Sex, sizeof(sex), 1, fTea);
		fread(&AllTeas[a].Inf.Birthday, sizeof(Date), 1, fTea);
		ReadString(fTea, &AllTeas[a].Inf.Hometown);
		ReadString(fTea, &AllTeas[a].Inf.IDCard);
		ReadString(fTea, &AllTeas[a].Inf.Mail);
		ReadString(fTea, &AllTeas[a].Inf.PhoneNumber);
		ReadString(fTea, &AllTeas[a].Inf.Photo);
		ReadString(fTea, &AllTeas[a].Inf.Title);
		ReadString(fTea, &AllTeas[a].Inf.PersonalInf);
	}

	fclose(fTea);
	return 0;
}

int SaveTeacherToFile(const char* FilePath)
{
	FILE* fTea = SaveFile(FilePath, HeadString);
	CHECK_NULL(fTea)

	int RealNum = GetNumOfStudents();
	fwrite(&RealNum, sizeof(int), 1, fTea);

	for (int a = 0; a < NumOfTeachers; a++) {
		if (!AllTeas->remove) {
			int l = 0;
			fwrite(&AllTeas[a].Id, sizeof(TeaID), 1, fTea);

			WriteString(fTea, AllTeas[a].name);

			fwrite(&AllTeas[a].Inf.Sex, sizeof(sex), 1, fTea);
			fwrite(&AllTeas[a].Inf.Birthday, sizeof(Date), 1, fTea);
			WriteString(fTea, AllTeas[a].Inf.Hometown);
			WriteString(fTea, AllTeas[a].Inf.IDCard);
			WriteString(fTea, AllTeas[a].Inf.Mail);
			WriteString(fTea, AllTeas[a].Inf.PhoneNumber);
			WriteString(fTea, AllTeas[a].Inf.Photo);
			WriteString(fTea, AllTeas[a].Inf.Title);
			WriteString(fTea, AllTeas[a].Inf.PersonalInf);
		}
	}

	fclose(fTea);

	free(AllTeas);
	AllTeas = NULL;
	NumOfTeachers = 0;
	AllocNumOfTea = 0;

	return 0;
}

int GetNumOfTeachers()
{
	int removed = 0;
	for (int a = 0; a < NumOfTeachers; a++) {
		if (AllTeas[a].remove == true) {
			removed++;
		}
	}
	return NumOfTeachers - removed;
}

TeacherInformation MakeTeacherInformation(sex _sex, const Date birthday, const char* hometown,
	const char* idcard, const char* mail, const char* PhoneNumber, const char* Photo, const char* Title, const char* PersonalInf)
{
	TeacherInformation TeaInf = { other };
	int len = 0;
	TeaInf.Sex = _sex;
	TeaInf.Birthday = birthday;

	len = strlen(hometown) + 1;
	TeaInf.Hometown = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.Hometown,return TeaInf)
	memcpy(TeaInf.Hometown, hometown, len);

	len = strlen(idcard) + 1;
	TeaInf.IDCard = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.IDCard, return TeaInf)
	memcpy(TeaInf.IDCard, idcard, len);

	len = strlen(mail) + 1;
	TeaInf.Mail = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.Mail, return TeaInf)
	memcpy(TeaInf.Mail, mail, len);

	len = strlen(PhoneNumber) + 1;
	TeaInf.PhoneNumber = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.PhoneNumber, return TeaInf)
	memcpy(TeaInf.PhoneNumber, PhoneNumber, len);

	len = strlen(Photo) + 1;
	TeaInf.Photo = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.Photo, return TeaInf)
	memcpy(TeaInf.Photo, Photo, len);
	
	len = strlen(Title) + 1;
	TeaInf.Title = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.Title, return TeaInf)
	memcpy(TeaInf.Title, Title, len);
	
	len = strlen(PersonalInf) + 1;
	TeaInf.PersonalInf = (char*)calloc(len, sizeof(char));
	CHECK_NULLANY(TeaInf.PersonalInf, return TeaInf)
	memcpy(TeaInf.PersonalInf, PersonalInf, len);
	
	return TeaInf;
}

void DeConstructTeacherInformation(TeacherInformation *TeaInf)
{
	free(TeaInf->Hometown);
	free(TeaInf->IDCard);
	free(TeaInf->Mail);
	free(TeaInf->PhoneNumber);
	free(TeaInf->Photo);
	free(TeaInf->Title);
	free(TeaInf->PersonalInf);
}

Teacher MakeTeacher(TeacherInformation TeaInf,const char* name, const TeaID NewID)
{
	Teacher Tea = { 0 };
	if (NewID < 1) {
		return Tea;
	}
	Tea.Id = NewID;
	Tea.name = (char*)calloc(strlen(name) + 1, sizeof(char));
	CHECK_NULLANY(Tea.name,return Tea)
	strcpy(Tea.name, name);
	Tea.Inf = TeaInf;
	return Tea;
}

void DeConstructTeacher(Teacher* teacher)
{
	DeConstructTeacherInformation(&teacher->Inf);
	free(teacher->name);
}


TeaID NewTeacher(Teacher* NewTea)
{
	if (NewTea->Id < 1) {
		return -1;
	}
	
	CHECK_NULLANY(AllTeas, AllTeas = (Teacher*)calloc(MinAllocSize, sizeof(Teacher)); AllocNumOfTea += MinAllocSize;)
	else {
		if (NumOfTeachers >= AllocNumOfTea) {
			Teacher* NewTeas = (Teacher*)realloc(AllTeas, sizeof(Teacher) * ((long)AllocNumOfTea + MinAllocSize));
			if (!NewTeas) {
				printf("Error: Failed to alloc memory.Allocing:%d * %d\n", AllocNumOfTea+MinAllocSize, (int)sizeof(Teacher));
				return -1;
			}

			AllTeas = NewTeas;
			AllocNumOfTea += MinAllocSize;
		}
	}

	Teacher* atea = NULL;
	for (int i = -1; (atea = IterateTeacher(&i)) != NULL;) {
		if (atea->Id == NewTea->Id) {
			return -2;
		}
	}

	memcpy(AllTeas + (NumOfTeachers), NewTea, sizeof(Teacher));
	NumOfTeachers++;

	return AllTeas->Id;
}

TeaID DeleteTeacher(TeaID Id)
{
	Teacher* TheTea = SeekTeacher(Id);
	CHECK_NULL(TheTea)
	TheTea->remove = true;
	return TheTea->Id;
}

Teacher* SeekTeacher(TeaID SeekTea)
{
	CHECK_NULLPTR(AllTeas)
	if (SeekTea < 1) {
		return NULL;
	}
	for (int a = 0; a < NumOfTeachers; a++) {
		if (AllTeas[a].Id == SeekTea && AllTeas[a].remove==false) {
			return AllTeas+a;
		}
	}
	return NULL;
}

inline Teacher* IterateTeacher(int* Index)
{
	for ((*Index)++; AllTeas[*Index].remove == true && *Index<NumOfTeachers; (*Index)++);
	if (*Index >= NumOfTeachers) {
		return NULL;
	}
	return AllTeas + *Index;
}

