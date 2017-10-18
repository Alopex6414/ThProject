#pragma once

#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include "Common.h"

//Variable Declaration
extern HANDLE g_hConsole;

//Function Declaration
extern void ConsoleInit();
extern void ConsoleWrite(const char *lpStr, float fValue);

#endif // !__CONSOLE_H_

