#pragma once

#ifndef __COMMON_H_
#define __COMMON_H_

#define WIN32_LEAN_AND_MEAN		//�� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ

//Windows ͷ�ļ�: 
#include <windows.h>

//C ����ʱͷ�ļ�
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <mmreg.h>
#include <wchar.h>
#include <tchar.h>
#include <time.h>
#include <mmsystem.h>

//����DirectX�ļ�
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <dinput.h>
#include <dsound.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"WinMM.lib")

//���� SDKDDKVer.h ��������õ���߰汾�� Windows ƽ̨
#include <SDKDDKVer.h>

//������Դ�ļ�
#include "resource.h"

//Macro Definition
#define SAFE_RELEASE(Pointer)	{if(Pointer){(Pointer)->Release();(Pointer) = NULL;}}//Release Pointer
#define SAFE_DELETE(Pointer)	{if(Pointer){delete(Pointer);(Pointer) = NULL;}}//Delete Pointer
#define SAFE_DELETE_ARRAY(Pointer) {if(Pointer){delete[](Pointer);(Pointer) = NULL;}}//Delete Pointer Array


#endif // !__COMMON_H_