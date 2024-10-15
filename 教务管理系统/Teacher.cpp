/*
* project: �������ϵͳ
* file   : Teacher.cpp
* Copyright <c> ciallo all right reserved.
*/
#include "Teacher.h"
#include "File.h"

/**�ؼ�ȫ�ֱ���**/
int NumOfTeachers;//AllTeas�б����ʦ��Ŀ
int AllocNumOfTea;//AllTeas�ڴ��������            
Teacher* AllTeas = NULL;//AllTeas����
/**��С��������**/
const int MinAllocSize = 64;//һ��Ԥ����AllTeas��С
/**�ļ�ͷ��ʶ**/
const char HeadString[HEAD_LENGTH] = "[Ciallo~ Teaching Affairs Manager][Teachersfile]";
/**�ļ��������**/
void InitialTeachers(const char* FilePath)
{
	//�������AllTeas�Ѿ����䣬�Ͳ�Ҫ���ã������ڴ�й©
	if (NumOfTeachers) {
		return;
	}
	//��ʼ�����к�AllTeas�йصı���
	NumOfTeachers = 0;
	AllocNumOfTea = 0;
	AllTeas = NULL;
	//����һ�ݿ��ļ�������ȱ�ļ�LoadTeacherFromFile����
	FILE* fTea = SaveFile(FilePath, HeadString);
	//д��һ��0��ʾ�ļ�����Teachers��ĿΪ��
	fwrite(&NumOfTeachers, sizeof(int), 1, fTea);
	//�ر��ļ�
	fclose(fTea);
}

int SaveTeacherToFile(const char* FilePath)
{
	//�ȴ򿪱����ļ��ľ��(�ļ���ʶ) 
	FILE* fTea = SaveFile(FilePath, HeadString);
	CHECK_NULL(fTea)//���򿪳ɹ�û

	//��ȡ��Ч��ʦ����
	int RealNum = GetNumOfTeachers();
	//д��һ����ʦ��Ŀ
	fwrite(&RealNum, sizeof(int), 1, fTea);
	
	//�ô�ѭ��д���ʦ��Ϣ
	for (int a = 0; a < NumOfTeachers; a++) {//�ж��Ƿ���ɾ����ʦ��ɾ���ľͱ�д����
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
	
	//�ر��ļ�
	fclose(fTea);
	//����AllTeas�Ա��´�LoadTeachersFromFile��Ҫ���
	free(AllTeas);
	AllTeas = NULL;
	NumOfTeachers = 0;
	AllocNumOfTea = 0;

	return 0;
}

int LoadTeacherFromFile(const char* FilePath)
{
	//���ж�AllTeas������Ѿ�����;ܾ���ȡ�������ڴ�й©(��һ�α����û���κβ������ظ�ִ�д˺������ͻ�����ڴ�й¶)
	CHECK_NULLANY(AllTeas,)else { return -1; }
	//�����ļ�ͷ������
	char ReadHead[HEAD_LENGTH] = "";
	//���ļ� 
	FILE* fTea = OpenFile(FilePath, "r", ReadHead);
	//�����Ƿ�ʧ��
	CHECK_NULL(fTea)

	//memcmp(�Ƚ��ڴ������ǰn���ֽ��Ƿ���ͬ)����ļ�ͷ��ʶ�Ƿ�ƥ��
	if (memcmp(ReadHead, HeadString, HEAD_LENGTH) != 0) {
		//��ƥ�����������Ϣ�˳�
		printf("Error: File head is Uncompatible! Suspect invation.\n");
		return -1;
	}
	//����NumOfTeacher���������ڴ�
	fread(&NumOfTeachers, sizeof(int), 1, fTea);
	//����Ԥ����ֱ�Ӻ���Ŀ���
	AllocNumOfTea = NumOfTeachers;
	//�����ڴ�
	AllTeas = (Teacher*)malloc(sizeof(Teacher) * NumOfTeachers);
	if (!AllTeas) {//�������,�����˳�
		printf("Error: Failed to alloc memory.Allocing:%d * %d\n",NumOfTeachers,(int)sizeof(Teacher));
		return -1;
	}
	//��һ��ѭ����������Teachers��Ϣƴ�ճ�Teacher
	for (int a = 0; a < NumOfTeachers; a++) {     
		//�����ʦ���� 
		fread(&AllTeas[a].Id, sizeof(TeaID), 1, fTea);
		//�����ʦ����
		ReadString(fTea, &AllTeas[a].name);
		//�����ʦ��Ϣ
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

/**�ṹ������򻯺���**/
TeacherInformation MakeTeacherInformation(sex _sex, const Date birthday, const char* hometown,
	const char* idcard, const char* mail, const char* PhoneNumber, const char* Photo, const char* Title, const char* PersonalInf)
{
	//�ȴ�����������ʱ����TeaInf
	TeacherInformation TeaInf = { other };
	int len = 0;//len������ʾ�ַ�������
	TeaInf.Sex = _sex;//��ֱ�Ӹ�ֵ�ȸ�ֵ
	TeaInf.Birthday = birthday;
	
	/**
	�������֣����ֱ�� = ��ֵ������ַ����׵�ַ��ֵ��ȥ����������Ҫ�����ַ�������
	����Ҫ�ȸ����ַ������ȣ�strlen�������ڴ棬Ȼ����memcpy��ֵ��memcpy����֪���Ȼ��Կ���strcpy��
	**/
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
	/**
	��󷵻ص���TeaInf�����ص�ʱ��ᴴ��TeaInf���ڴ渱����Ȼ�����ڵ�TeaInf�ᱻ����(��������������)��
	�����µ�TeaInf�ᱻ���أ����ڼ�ֻ���ַ���ָ�뱾��ת���ˣ��ַ����������ڴ��в���Ӱ��
	**/
}

void DeConstructTeacherInformation(TeacherInformation *TeaInf)
{
	//ȫfree��������ֱ�Ӷ���TeaInf����Ķ�̬�����ַ��������Զ����٣����ڴ�й©
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
	//��ϸ�������ο�MakeTeacherInformation
	Teacher Tea = { 0 };
	if (NewID < 1) {//�ж�TeaID�Ϸ���
		return Tea;
	}
	//ֱ�Ӹ�ֵ
	Tea.Id = NewID;
	Tea.name = (char*)calloc(strlen(name) + 1, sizeof(char));
	CHECK_NULLANY(Tea.name,return Tea)
	strcpy(Tea.name, name);
	Tea.Inf = TeaInf; //�������ֱ�Ӹ�ֵ����Ϊ�ṹ��ֱ�Ӵ�����ȥ�ˣ��൱���ǰ��ַ���ָ�봫�ݹ�ȥ��
	return Tea;
}

void DeConstructTeacher(Teacher* teacher)
{
	//��DeConstructTeacherInformation
	DeConstructTeacherInformation(&teacher->Inf);
	free(teacher->name);
}

/**AllTeas���ݿ��������**/

int GetNumOfTeachers()
{
	//�ų�����removed�Ľ�ʦ
	int removed = 0;
	for (int a = 0; a < NumOfTeachers; a++) {//����AllTeas
		if (AllTeas[a].remove == true) {//ֻҪ����remove��ʶ�Զ���remove��һ
			removed++;
		}
	}
	return NumOfTeachers - removed;//�ܳ�-ɾ������Ŀ=��Ч����
}

TeaID NewTeacher(Teacher* NewTea)
{
	//�ȼ��TeaID�Ϸ���
	if (NewTea->Id < 1) {
		return -1;
	}
	//��һ�бȽ�����⣬��ο�CHECK_NULLANY��һ�𿴣��������˼���������AllTeas��û����ͷ����ڴ�
	CHECK_NULLANY(AllTeas, //��һ�����ж�NULL������Ҳ�����ж�AllTeas�Ƿ�Ϊ��
	AllTeas = (Teacher*)calloc(MinAllocSize, sizeof(Teacher)); AllocNumOfTea += MinAllocSize;) //��Ӧ���е�r�����Ϊ�գ�һ�η�����С���������Ŀռ� 
	else {//�������ú�չ��ֱ�ӽ��ϣ���ʾAllTeas��Ϊ������
	
		//���Ԥ����洢�Ѿ��ľ����ͼ���������
		if (NumOfTeachers >= AllocNumOfTea) {
			Teacher* NewTeas = (Teacher*)realloc(AllTeas, sizeof(Teacher) * ((long)AllocNumOfTea + MinAllocSize));
			if (!NewTeas) {
				printf("Error: Failed to alloc memory.Allocing:%d * %d\n", AllocNumOfTea+MinAllocSize, (int)sizeof(Teacher));
				return -1;//�ж��ڴ����ʧ�ܷ�
			}
			//realloc�������ɹ����Ͱ�AllTeas��Ϊ�·����NewStus���ɵ�AllTeas�ᱻrealloc�Զ�����
			AllTeas = NewTeas;
			//�ѷ����С�ǵü���
			AllocNumOfTea += MinAllocSize;
		}
	}
	//��һ�����Ǹ����ʦ���
	Teacher* atea = NULL;
	//�����жϹ��������غ�
	for (int i = -1; (atea = IterateTeacher(&i)) != NULL;) {//IterateTeacher������ʦ
		if (atea->Id == NewTea->Id) {//һ�����ֺ��½�ʦ�غţ�ֱ��return
			return -2;
		}
	}
	//û����Ļ�������ĩβ���µ�Teachers���ƹ�ȥ
	memcpy(AllTeas + (NumOfTeachers), NewTea, sizeof(Teacher));
	NumOfTeachers++;//��ʦ������һ
	//���ؽ�ʦ���� 
	return AllTeas->Id;
}

TeaID DeleteTeacher(TeaID Id)
{
	Teacher* TheTea = SeekTeacher(Id);////��Ѱ�ҽ�ʦ������������Teacher�ṹ���ַ
	CHECK_NULL(TheTea)//�ж��ҵ���
	TheTea->remove = true;//��ɾ��λ��һ
	return TheTea->Id;//����ID
}

Teacher* SeekTeacher(TeaID SeekTea)
{
	//���ж�AllTeas��û�з��䣬������ʿ�ָ��
	CHECK_NULLPTR(AllTeas)
	if (SeekTea < 1) {//���TeaID�Ϸ���
		return NULL;
	}
	for (int a = 0; a < NumOfTeachers; a++) { //����AllTeas
		if (AllTeas[a].Id == SeekTea && AllTeas[a].remove==false) { //����ҵ��ý�ʦ���Ҳ�����ɾ����ʦ 
			return AllTeas+a;//ֱ�ӷ����������е�ַ
		}
	}
	return NULL;//�Ҳ������ؿ�
}

inline Teacher* IterateTeacher(int* Index)   //inline ��������(���#define�궨�壬���ٶ���ջ�ռ��ռ�ã���߳���Ч��) 
{
	for ((*Index)++; //Index��������������ʾ����һ����ʦ������һ��
	AllTeas[*Index].remove == true && *Index<NumOfTeachers; //Ȼ���ж���һ����û�б�ɾ����Index��û�г���AllTeas����
	(*Index)++);  //�����ɾ����ôforѭ������������Index����һ��
	//���forѭ������Ҫô�ҵ��ˣ�Ҫô������
	if (*Index >= NumOfTeachers) {//�ж��ǲ���������
		return NULL;
	}
	//ֱ�ӷ����������е�ַ
	return AllTeas + *Index;
}

