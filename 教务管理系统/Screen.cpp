#include "Screen.h"
/*
* project: 教务管理系统
* file   : Screen.cpp
* Copyright <c> ciallo all right reserved.
*/

const char
s_Semester[] = "Semester.ini",
s_Course[] = "Courses.txt",
s_Class[] = "Class.txt",
s_Student[] = "Students.txt",
s_Teacher[] = "Teachers.txt";

void Screen1()
{
	system("color F0");
	PrintBorder();
	CenterBorder(30, 5);
	ShowText("正在初始化中");
	char fst[HEAD_LENGTH] = "FirstRun";
	FILE* fr = OpenFile("FirstRun", "r", fst);
	if (fr == NULL) {
		TimeTable tb = MakeTimeTable(5, makeTime(8, 0)
			, makeTime(9, 15), makeTime(10, 30), makeTime(11, 45), makeTime(13, 0));
		InitialSemester(s_Semester, &tb);
		InitialCourses(s_Course);
		InitialClasses(s_Class);
		InitialStudents(s_Student);
		InitialTeachers(s_Teacher);
		fclose(SaveFile("FirstRun", "FirstRun"));
	}
	LoadSemesterFromFile(s_Semester);
	LoadCoursesFromFile(s_Course);
	LoadClassesFromFile(s_Class);
	LoadStudentFromFile(s_Student);
	LoadTeacherFromFile(s_Teacher);
	Screenlogon();
}

void Screenlogon()
{
	PrintBorder();
	ShowTitle("欢迎登录教务管理系统");
	ShowText("请输入用户名(学号/工号):");
	int UserName;
	scanf("%d", &UserName);
	char* pwd = InputBox("请输入密码:", 1);
	int hash = 0;

	if (UserName == 114514 && strcmp(pwd, "1919810") == 0) {
		AdminMain();
	}
	else {
		for (int a = 0; a < strlen(pwd); a++) {
			hash += pwd[a];
			hash = hash % 13;
		}
		Student* stu = NULL; Teacher* tea = NULL;
		if ((stu = SeekStudent(UserName)) != NULL) {
			if (stu->Inf.pwdHash == hash) {
				StudentMain(stu->Id);
			}
			else {
				CenterBorder(50, 5);
				ShowText("用户名或密码错误");
				getch();
				Screenlogon();
				return;
			}
		}else if ((tea = SeekTeacher(UserName)) != NULL) {
			if (tea->Inf.pwdHash == hash) {
				TeacherMain(tea->Id);
			}
			else {
				CenterBorder(50, 5);
				ShowText("用户名或密码错误");
				getch();
				Screenlogon();
				return;
			}
		}
		else {
			CenterBorder(50, 5);
			ShowText("用户名或密码错误");
			getch();
			Screenlogon();
			return;
		}
	}

	SaveCoursesToFile();
	SaveClassesToFile();
	SaveStudentToFile(s_Student);
	SaveTeacherToFile(s_Teacher);
	
}

void StudentMain(StuID Stu)
{
	int Choose = -1;
	while (Choose != 0) {
		PrintBorder();
		Student* pStu = SeekStudent(Stu);
		CHECK_NULLANY(pStu, ShowText("查找学生数据失败！"); return;);
		printf(" 你好，%s，请问你要做什么?\n", pStu->name);
		ShowText("1. 补充/修正/查看个人信息");
		ShowText("2. 自主选课");
		ShowText("3. 查看课程考核");
		ShowText("4. 修改密码");
		ShowText("0. 退出");

		Choose = getch() - 48;

		switch (Choose) {
		case 1:
			StuDetail(pStu);
			break;
		case 2:
			StuChoClass(pStu);
			break;
		case 3:
			SeeGrade(pStu);
			break;
		case 4:
			ChangePwdStu(pStu);
			break;
		}
	}
}

void TeacherMain(TeaID Tea)
{
	int Choose = -1;
	while (Choose != 0) {
		PrintBorder();
		Teacher* pStu = SeekTeacher(Tea);
		CHECK_NULLANY(pStu, ShowText("查找老师数据失败！"); return;);
		printf(" 你好，%s，请问你要做什么?\n", pStu->name);
		ShowText("1. 补充/修正/查看个人信息");
		ShowText("2. 开课");
		ShowText("3. 开班");
		ShowText("4. 修改密码");
		ShowText("5. 打分");
		ShowText("0. 退出");

		Choose = getch() - 48;

		switch (Choose) {
		case 2:
			OpenCourse(pStu);
			break;
		case 3:
			OpenClass(pStu);
			break;
		case 5:
			ScoreStu(pStu);
		}
	}
}

void AdminMain()
{
	int Choose = -1;
	while (Choose != 0) {
		PrintBorder();
		ShowText("你好，Admin，接下来要做什么：");
		ShowText("1. 添加学生");
		ShowText("2. 添加老师");
		ShowText("3. 列出所有老师");
		ShowText("4. 列出所有学生");
		ShowText("0. 退出");

		Choose = getch() - 48;

		switch (Choose) {
		case 1:
			AddStu();
			break;
		case 2:
			AddTea();
			break;
		case 3:
			ListTea();
			break;
		case 4:
			ListStu();
			break;
		}
	}
}

void AddStu()
{
	ShowTitle("添加学生操作");
	ShowLine();
	Student New;
	StudentInformation Inf;
	ShowText("请输入姓名:");
	char name[_MAX_PATH] = "";
	scanf("%s", name);
	ShowText("请输入性别(男:0/女:1):");
	sex Sex = other;
	scanf("%d", &Sex);
	ShowText("请输入分配的学号:");
	int ID = 0;
	scanf("%d", &ID);
	ShowLine();

	ShowText("请输入生日(年/月/日):");
	Date Dat = { 0 };
	scanf("%d %d %d", &Dat.Year,&Dat.Month,&Dat.Day);
	ShowLine();
	ShowText("请输入家乡:");
	char Htn[_MAX_PATH] = "";
	scanf("%s", Htn);
	ShowLine();
	ShowText("请输入身份证:");
	char IDC[_MAX_PATH] = "";
	scanf("%s", IDC);
	ShowLine();
	ShowText("请输入邮箱:");
	char MaB[_MAX_PATH] = "";
	scanf("%s", MaB);
	ShowLine();
	ShowText("请输入电话号码:");
	char PhN[_MAX_PATH] = "";
	scanf("%s", PhN);

	Inf = MakeStudentInformation(Sex, Dat, Htn, IDC, MaB, PhN, "NoPhoto");
	Inf.pwdHash=0;
	char pwd[] = "1234";
	for (int a = 0; a < strlen(pwd); a++) {
		Inf.pwdHash += pwd[a];
		Inf.pwdHash = Inf.pwdHash % 13;
	}
	New = MakeStudent(Inf, name, ID);

	int ret=NewStudent(&New);
	if (ret < 0) {
		printf("出现错误，错误码:%d", ret);
		getch();
		return;
	}
	CenterBorder(50, 5);
	ShowText("添加完毕！");
	getch();
}
void AddTea()
{
	ShowTitle("添加教师操作");
	ShowLine();
	Teacher New;
	TeacherInformation Inf;
	ShowText("请输入姓名:");
	char name[_MAX_PATH] = "";
	scanf("%s", name);
	ShowText("请输入性别(男:0/女:1):");
	sex Sex = other;
	scanf("%d", &Sex);
	ShowText("请输入分配的工号:");
	int ID = 0;
	scanf("%d", &ID);
	ShowLine();

	ShowText("请输入生日(年/月/日):");
	Date Dat = { 0 };
	scanf("%d %d %d", &Dat.Year, &Dat.Month, &Dat.Day);
	ShowLine();
	ShowText("请输入家乡:");
	char Htn[_MAX_PATH] = "";
	scanf("%s", Htn);
	ShowLine();
	ShowText("请输入身份证:");
	char IDC[_MAX_PATH] = "";
	scanf("%s", IDC);
	ShowLine();
	ShowText("请输入邮箱:");
	char MaB[_MAX_PATH] = "";
	scanf("%s", MaB);
	ShowLine();
	ShowText("请输入电话号码:");
	char PhN[_MAX_PATH] = "";
	scanf("%s", PhN);
	ShowLine();
	ShowText("请输入职称:");
	char Prof[_MAX_PATH] = "";
	scanf("%s", Prof);
	ShowLine();
	ShowText("请输入个人介绍:");
	char Int[_MAX_PATH] = "";
	scanf("%s", Int);

	Inf = MakeTeacherInformation(Sex, Dat, Htn, IDC, MaB, PhN, "NoPhoto", Prof, Int);
	Inf.pwdHash = 0;
	char pwd[] = "1234";
	for (int a = 0; a < strlen(pwd); a++) {
		Inf.pwdHash += pwd[a];
		Inf.pwdHash = Inf.pwdHash % 13;
	}
	New = MakeTeacher(Inf, name, ID);
	int ret = NewTeacher(&New);
	CenterBorder(50, 5);
	if (ret < 0) {
		printf("出现错误，错误码:%d", ret);
		getch();
		return;
	}
	ShowText("添加完毕！");
	getch();
}
void ListStu(){
	PrintBorder();
	ShowTitle("列出学生"); ShowLine();
	Student* stu = NULL;
	int Count = 0;
	for (int a = -1; (stu = IterateStudent(&a)) != NULL;) {
		printf("%d .姓名:%s\t 性别:%d\t电话:%s\n#", stu->Id, stu->name, stu->Inf.Sex, stu->Inf.PhoneNumber);
		ShowLine();
		Count++;
		if (Count >= 10) {
			printf("按任意键滚动到下一页");
			getch();
			PrintBorder();
			Count = 0;
		}
	}
	printf("显示完毕，按任意键返回");
	getch();
}
void ListTea() {
	PrintBorder();
	ShowTitle("列出老师"); ShowLine();
	Teacher* stu = NULL;
	int Count = 0;
	for (int a = -1; (stu = IterateTeacher(&a)) != NULL;) {
		printf("%d .姓名:%s\t 职称:%s\t电话:%s\n#", stu->Id, stu->name, stu->Inf.Title, stu->Inf.PhoneNumber);
		ShowLine();
		Count++;
		if (Count >= 5) {
			printf("按任意键滚动到下一页");
			getch();
			PrintBorder();
			Count = 0;
		}
	}
	printf("显示完毕，按任意键返回");
	getch();
}

void StuDetail(Student* pStu)
{
	int Select = 1;
	while (Select != -1) {
		PrintBorder();
		ShowTitle("学生个人信息");
		ShowLine();//4
		printf("  姓名：%s\t学号：%d\t性别：%c\n", pStu->name, pStu->Id, pStu->Inf.Sex == male ? '男' : '女');
		ShowLine();//6
		printf("  出生年月：%d/%d/%d\n", pStu->Inf.Birthday.Year, pStu->Inf.Birthday.Month, pStu->Inf.Birthday.Day);
		ShowLine();//8
		printf("  家乡：%s\n", pStu->Inf.Hometown);
		ShowLine();//10
		printf(" 身份证：%s\n", pStu->Inf.IDCard);
		ShowLine();
		printf("  邮箱：%s\n", pStu->Inf.Mail);
		ShowLine();
		printf("  电话：%s\n", pStu->Inf.PhoneNumber);
		gotoxy(1, Select * 2 + 1);
		printf("→");

		gotoxy(2, 14);
		ShowLine();
		ShowText("若要修改→项，请按ENTER,若要退出请按ESC,上选按w，下选按s");//ENT13 ESC27 w 119 s 115
		int In = getch();
		char Input[_MAX_PATH] = "";
		switch (In) {
		case 119:
			Select--;
			break;
		case 115:
			Select++;
			break;
		case 13:
			switch (Select) {
			case 3:
				printf("输入新的故乡：");
				scanf("%s", Input);
				strcpy(pStu->Inf.Hometown, Input);
				CenterBorder(50, 5);
				ShowText("修改成功！");
				break;
			}
			break;
		case 27:
			Select = -1;
			break;
		}
		if (Select == 0)
			Select = 1;
		if (Select > 6)
			Select = 6;
	}

}

void StuChoClass(Student* pStu)
{
	PrintBorder();
	ShowText("课程列表");
	ShowLine();
	Class* pCl = NULL;
	Course* pCou = NULL;

	for (int a = -1; (pCl = IterateClass(&a)) != NULL;) {
		pCou = SeekCourseById(pCl->course);
		Teacher* pTea = SeekTeacher((TeaID)pCou->Teacher);
		printf("课程编号：%d\t课程名称：%s\t任教老师：%s\n#", pCl->ID, pCou->CourseName, pTea->name);
		ShowLine();
	}
	printf("请输入选课编号：");
	int Select = 0;
	scanf("%d", &Select);
	if (Select == 0) {
		CenterBorder(50, 3);
		ShowText("取消选课");
		getch();
		return;
	}
	if ((pCl = SeekClassById(Select)) == NULL) {
		CenterBorder(50, 3);
		ShowText("找不到此课");
		getch();
		return;
	}
	CHECK_NULLANY(pStu->ClassList, pStu->ClassList = (int*)calloc(64, sizeof(int));
	memset(pStu->ClassList,0,sizeof(int)*64);)else {
		int a;
		for (a = 0; pStu->ClassList[a] != 0; a++);
		pStu->ClassList[a] = Select;
	}
	ChooseClass(pCl->ID, pStu->Id);

	CenterBorder(50, 3);
	ShowText("选课成功");
	getch();
}

void SeeGrade(Student* pStu)
{
	PrintBorder();
	ShowTitle("成绩情况");
	Class* pCl = NULL;
	Course* pCou = NULL;
	for (int a = 0; pStu->ClassList[a] != 0; a++) {
		ShowLine();
		int Val = SeekGrade(pStu->ClassList[a], pStu->Id);
		if (Val < 0) {

		}
		else {
			pCl = SeekClassById(pStu->ClassList[a]);
			pCou = SeekCourseById(pCl->course);
			printf("科目：%s\t考试分数:%d\n#", pCou->CourseName, Val);
		}
	}
	ShowLine();
	ShowText("按任意键返回");
	getch();
}

void ChangePwdStu(Student* pStu)
{
	char* newpwd = InputBox("请输入新密码:", 1);
	char* newpwd2 = InputBox("请再次输入新密码:", 1);
	if (strcmp(newpwd, newpwd2) != 0) {
		ShowText("两次输入密码不一致");
		getch();
		return;
	}
	int hash = 0;
	for (int a = 0; a < strlen(newpwd); a++) {
		hash += newpwd[a];
		hash = hash % 13;
	}
	pStu->Inf.pwdHash = hash;
	ShowText("修改成功");
	getch();
}

void OpenCourse(Teacher* pTea)
{
	char str[_MAX_PATH] = "";
	printf("课程名称：");
	scanf("%s", str);
	int id;
	printf("课程编号：");
	scanf("%d", &id);
	Course Cou = MakeCourse(MakeCourseInformation("CODEC", "Introduction"),
		str, 100, id, pTea->Id,0);
	NewCourse(&Cou);

	CenterBorder(50, 3);
	ShowText("开课成功");
	getch();
}

void OpenClass(Teacher* pTea)
{
	int id,Couid;
	printf("开课课程编号：");
	scanf("%d", &Couid);
	if (SeekCourseById(Couid) == NULL) {
		ShowText("找不到课程");
		getch();
		return;
	}
	printf("班级编号：");
	scanf("%d", &id);
	Class Cou = MakeClass(id, { 1,10 }, Mon, Couid);
	NewClass(&Cou);

	CenterBorder(50, 3);
	ShowText("开班成功");
	getch();
}

void ScoreStu(Teacher* pTea)
{
	PrintBorder();
	ShowTitle("打分");
	for (int a = 0; pTea->ClassList[a] != 0; a++) {
		Class* pCl = SeekClassById(pTea->ClassList[a]);
		Course* pCou = SeekCourseById(pCl->course);
		printf("课程名称：%s\t 班级ID:%d\n#", pCou->CourseName, pCl->ID);
		ShowLine();
	}
	int ClassID = -1;
	printf("选择班级:");
	scanf("%d", &ClassID);
	Class* pCl = SeekClassById(ClassID);
	ClassInformation Inf = ReturnClassStudentName(ClassID);

	int Count = 0;
	ShowLine();
	for (int a = 0;a< Inf.NumOfStus; a++) {
		Student* pStu = SeekStudent(Inf.StuSitus[a].StuID);
		printf("%d. %s\t",Inf.StuSitus[a].StuID,pStu->name);
		if (Count > 7) {
			printf("\n");
			ShowLine();
			Count = 0;
		}
		Count++;
	}
	int SStuID = -1;
	printf("选择同学ID:");
	scanf("%d", &SStuID);

	int NewGrade = -1;
	printf("输入新成绩");
	scanf("%d", &NewGrade);

	LoginGrade(ClassID, SStuID,NewGrade);

	CenterBorder(50, 3);
	ShowText("打分完毕！");
}
