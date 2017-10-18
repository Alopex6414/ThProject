#include "Common.h"
#include "Include.h"
#include "Console.h"

//Console
HANDLE g_hConsole;//Console控制台窗口

/*
** Function: ConsoleInit()
** Purpose: Console控制台调试初始化
*/
void ConsoleInit()
{
	AllocConsole();//Console控制台调试界面
	g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//获取Console句柄
}

/*
** Function: ConsoleWrite()
** Purpose: Console控制台输出显示
** Para: const char *lpStr		//输出字符串
** Para: float fValue				//输出浮点变量
*/
void ConsoleWrite(const char *lpStr, float fValue)
{
	char ConsoleStr[20];
	DWORD dw;

	_gcvt_s(ConsoleStr, 20, fValue, 6);
	WriteConsoleA(g_hConsole, lpStr, strlen(lpStr), &dw, NULL);
	WriteConsoleA(g_hConsole, ConsoleStr, strlen(ConsoleStr), &dw, NULL);
	WriteConsoleA(g_hConsole, "\n", strlen("\n"), &dw, NULL);
}