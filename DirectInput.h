#pragma once

#ifndef __DIRECTINPUT_H_
#define __DIRECTINPUT_H_

#include "Common.h"

//Macro Definition
#define KEYBOARD_ARRAYSIZE	256//KeyBoard数组大小

//Enum Definition
enum MouseButton
{
	Mouse_LeftButton = 0,	//鼠标左键
	Mouse_RightButton = 1,	//鼠标右键
};

class DirectInput
{
private:
	LPDIRECTINPUT8 m_pDirectInput;//IDirectInput8接口对象指针
	LPDIRECTINPUTDEVICE8 m_pKeyBoardDevice;//IDirectInputDevice8接口对象指针(KeyBoard键盘设备)
	LPDIRECTINPUTDEVICE8 m_pMouseDevice;//IDirectInputDevice8接口对象指针(Mouse鼠标设备)
	DIMOUSESTATE m_MouseState;//Mouse鼠标键值结构体对象
	char m_cKeyBoradBuffer[KEYBOARD_ARRAYSIZE];//KeyBoard键盘键值状态数组

public:
	DirectInput();
	~DirectInput();
	HRESULT Initialize(HWND hWnd, HINSTANCE hInstance, DWORD KeyBoardCoopFlags, DWORD MouseCoopFlags);//KeyBoard键盘/Mouse鼠标设备初始化
	void GetDeviceState(void);//获取Device设备状态
	bool IsKeyDown(int KeyValue);//获取KeyBoard键盘状态
	bool IsMouseDown(MouseButton MouseValue);//获取Mouse鼠标状态
	float GetMouseX(void);//获取Mouse鼠标X轴坐标
	float GetMouseY(void);//获取Mouse鼠标Y轴坐标
	float GetMouseZ(void);//获取Mouse鼠标Z轴坐标(滚轮)
};

#endif // !__DIRECTINPUT_H_