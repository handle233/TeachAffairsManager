#pragma once
/*
* project: �������ϵͳ
* file   : Teacher.h
* Copyright <c> ciallo all right reserved.
*/
#ifndef TEACHER
#define TEACHER
#include "PublicHead.h"
#include "CourseList.h"

/**�����Ͷ���**/
typedef int TeaID;

/**�ṹ�����Ͷ���**/
//��ʦ������Ϣ�ṹ��
struct TeacherInformation {
	sex Sex;//�Ա�
	char* Hometown;//����
	Date Birthday;//����
	char* IDCard;//���֤
	char *Photo;//��Ƭ·��
	char* Mail;//����
	char* PhoneNumber;//�绰
	char* Title;//ְ�� 
	char* PersonalInf;//���˼�� 
};

//��ʦ�ṹ��
struct Teacher
{
	int Id;//���� 
	char* name;//����
	TeacherInformation Inf;//��ʦ������Ϣ
	bool remove;
};

/**ȫ�ֱ�������**/
//�����ȡ���ֽ�ʦȫ����Ϣ
extern Teacher* AllTeas;

/**��������**/
/*
��ʼ����ʦ�ļ�������һ���ս�ʦ�ļ�������AllTeas
����1 �ļ�·��
*/
void InitialTeachers(const char* FilePath);
/*
���ļ��м��ؽ�ʦ������AllTeas
����1 �ļ�·��
����ֵ -1:�ļ���ʧ�ܻ����ļ�ͷ����-2:�ڴ����ʧ��
*/
int LoadTeacherFromFile(const char *FilePath);
/*
����ȡ��ʦ��Ϣ���浽�ļ��У������ͷ�AllTeas
����1 �ļ�·��
����ֵ -1:�ļ���ʧ��
*/
int SaveTeacherToFile(const char* FilePath);

/**�ṹ������򻯺���**/
/*
���ٴ���TeacherInformation�ṹ�壬��ȥ�ֶ�Alloc
����N �ṹ����Ϣ���μ�TeacherInformation
����ֵ �����õ�TeacherInformation ���󷵻��Ա�Ϊothers�Ľṹ��
*/
TeacherInformation MakeTeacherInformation(sex _sex, const Date birthday, const char* hometown,
	const char* idcard, const char* mail, const char* PhoneNumber, const char* Photo,const char* Title,const char* PersonalInf);
/*
��������TeacherInformation�ṹ�壬��ȥ�ֶ�Free  �۹�����(�ջض�������Ŀռ�) 
����1 TeachertInformationָ��
*/
void DeConstructTeacherInformation(TeacherInformation *TeaInf);
/*
���ٴ���Teacher�ṹ�壬��ȥ�ֶ�Alloc
����1 TeacherInformation
����2 ��ʦ����
����3 ����Ľ�ʦID(�������0)
����ֵ �����õ�Teacher ���󷵻�����Ϊnullptr�Ľṹ��
*/
Teacher MakeTeacher(TeacherInformation TeaInf, const char* name, const TeaID NewID);
/*
��������Teacher�ṹ�壬��ȥ�ֶ�Free
����1 Teacherָ��
*/
void DeConstructTeacher(Teacher* teacher);

/**AllTeas���ݿ��������**/
/*
��ȡAllTeas����ЧTeachers����
����ֵ Teachers����
*/
int GetNumOfTeachers();
/*
�����ݿ����һ����ʦ
����1 Teacher�ṹ��(TeaID�������0)
����ֵ Teacher ID -1:�����ڴ�ʧ�ܻ�������Ϸ� -2:�����ʦD�ظ� 
*/
TeaID NewTeacher(Teacher* NewTea);
/*
�����ݿ�ɾ��һ����ʦ
����1 TeacherID(TeaID�������0)
����ֵ Teacher ID -1:�����ڴ�ʧ�ܻ�������Ϸ�
*/
TeaID DeleteTeacher(TeaID Id);
/*
�����ݿ����һ����ʦ
����1 Teacher�ṹ��(TeaID�������0)
����ֵ Teacher ID NULL:û�ҵ���ʦ��������Ϸ���Ҳ������AllTeasû�г�ʼ��
*/
Teacher* SeekTeacher(TeaID SeekTea);
/*
ö�����ݿ��н�ʦ
����1 Indexָ�룬���ڱ�ʶö�ٽ��ȣ�ÿ��Iterate�������һ����ʼӦ���趨indexΪ-1
����ֵ Teacher ID -1:�����ڴ�ʧ�ܻ�������Ϸ� -2:�����ʦID�ظ�
*/
inline Teacher* IterateTeacher(int *Index);

#endif