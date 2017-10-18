#pragma once

#ifndef __DIRECTINPUT_H_
#define __DIRECTINPUT_H_

#include "Common.h"

//Macro Definition
#define KEYBOARD_ARRAYSIZE	256//KeyBoard�����С

//Enum Definition
enum MouseButton
{
	Mouse_LeftButton = 0,	//������
	Mouse_RightButton = 1,	//����Ҽ�
};

class DirectInput
{
private:
	LPDIRECTINPUT8 m_pDirectInput;//IDirectInput8�ӿڶ���ָ��
	LPDIRECTINPUTDEVICE8 m_pKeyBoardDevice;//IDirectInputDevice8�ӿڶ���ָ��(KeyBoard�����豸)
	LPDIRECTINPUTDEVICE8 m_pMouseDevice;//IDirectInputDevice8�ӿڶ���ָ��(Mouse����豸)
	DIMOUSESTATE m_MouseState;//Mouse����ֵ�ṹ�����
	char m_cKeyBoradBuffer[KEYBOARD_ARRAYSIZE];//KeyBoard���̼�ֵ״̬����

public:
	DirectInput();
	~DirectInput();
	HRESULT Initialize(HWND hWnd, HINSTANCE hInstance, DWORD KeyBoardCoopFlags, DWORD MouseCoopFlags);//KeyBoard����/Mouse����豸��ʼ��
	void GetDeviceState(void);//��ȡDevice�豸״̬
	bool IsKeyDown(int KeyValue);//��ȡKeyBoard����״̬
	bool IsMouseDown(MouseButton MouseValue);//��ȡMouse���״̬
	float GetMouseX(void);//��ȡMouse���X������
	float GetMouseY(void);//��ȡMouse���Y������
	float GetMouseZ(void);//��ȡMouse���Z������(����)
};

#endif // !__DIRECTINPUT_H_