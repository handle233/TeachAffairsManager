#pragma once
/*
* project: �������ϵͳ
* file   : PublicHead.h
* Copyright <c> ciallo all right reserved.
*/
//��������ͷ�ļ�
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
//ȡ��C4996���棬�Ա�scanf�Ⱥ���������
#pragma warning(disable:4996)
#pragma warning(disable:4267)
//��ʾ�ò������ڽ��ܺ��������
#define _IN_
//��ʾ�ò������ڽ��ܺ���������
#define _OUT_
//����NULL
#ifndef NULL
#define NULL (void*)0
#endif // !NULL
//����true �� false
#ifndef true
#define true 1
#endif // !true
#ifndef false
#define false 0
#endif // !false
//С�ͺ�ű�
#ifndef CHECK_NULL
/**CHECKNULLϵ����һ�ַ�����жϱ����Ƿ�Ϊ�յķ���**/
/**
����Դ��в�����ʹ�õ�ʱ���ڲ���������Ķ������ᱻֱ��ԭ�ⲻ�����Ƶ���Ӧλ��
**/
//��һ�����Ǽ��q����Ķ����ǲ���null���ǵĻ�����-1��ע�����������ֱ���ں����ﷵ�أ��걾��û�з���ֵ
#define CHECK_NULL(q) if(q==NULL){return -1;}
//��һ�����Ǽ��q����Ķ����ǲ���null���ǵĻ�����NULL��ע�����������ֱ���ں����ﷵ�أ��걾��û�з���ֵ
#define CHECK_NULLPTR(q) if(q==NULL){return NULL;}
//��һ�����Ǽ��q����Ķ����ǲ���null���ǵĻ���ִ��r����Ĵ��롣
//r���Բ����ʾִ�пմ��룬�������ں������else�������ͻ��else��ǰ������if��������ʾ���q����NULLִ�е������
#define CHECK_NULLANY(q,r) if(q==NULL){r;}
#endif // !CHECK_NULL
/**�汾���Ʊ���**/
#ifndef VERSION_CONTROL
#define VERSION_CONTROL

#define VERSION 1

#define WEEKDAY 7
/**�����Ͷ���**/
//�Ա𣬿�������̧��
enum sex { male, female, other };
enum Weekday { Sun, Mon, Tue, Wed, Thu, Fri, Sat };
void gotoxy(int x, int y);
//����
struct Date
{
	int Year; int Month; int Day;
};
#endif // !�汾���Ʊ���