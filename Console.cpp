#include "Common.h"
#include "Include.h"
#include "Console.h"

//Console
HANDLE g_hConsole;//Console����̨����

/*
** Function: ConsoleInit()
** Purpose: Console����̨���Գ�ʼ��
*/
void ConsoleInit()
{
	AllocConsole();//Console����̨���Խ���
	g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡConsole���
}

/*
** Function: ConsoleWrite()
** Purpose: Console����̨�����ʾ
** Para: const char *lpStr		//����ַ���
** Para: float fValue				//����������
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