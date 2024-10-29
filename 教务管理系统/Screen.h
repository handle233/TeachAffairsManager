#pragma once
/*
* project: 教务管理系统
* file   : Screen.h
* Copyright <c> ciallo all right reserved.
*/
#include "Student.h"
#include "Teacher.h"
#include "UserInterface.h"

void Screen1();
void Screenlogon();
void StudentMain(StuID Stu);
void TeacherMain(TeaID Tea);
void AdminMain();
void AddStu();
void AddTea();
void ListStu();
void ListTea();

void StuDetail(Student* pStu);
void StuChoClass(Student* pStu);
void SeeGrade(Student* pStu);

void ChangePwdStu(Student* pStu);
void OpenCourse(Teacher* pTea);
void OpenClass(Teacher* pTea);

void ScoreStu(Teacher* pTea);