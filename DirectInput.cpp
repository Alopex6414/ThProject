#include"Common.h"
#include "Include.h"
#include "DirectInput.h"


/*
** Function:	 DirectInput()
** Purpose: DirectInput���캯��
*/
DirectInput::DirectInput()
{
	m_pDirectInput = NULL;//IDirectInput8�ӿڶ���ָ��NULL
	m_pKeyBoardDevice = NULL;//KeyBoard�����豸ָ��NULL
	m_pMouseDevice = NULL;//Mouse����豸ָ��NULL
	ZeroMemory(&m_cKeyBoradBuffer, sizeof(char) * KEYBOARD_ARRAYSIZE);//���KeyBoradBuffer
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));//���MouseState
}

/*
** Function: ~DirectInput()
** Purpose: DirectInput��������
*/
DirectInput::~DirectInput()
{
	if(m_pMouseDevice != NULL)//Mouse����豸ָ�벻ΪNULL
	{
		m_pMouseDevice->Unacquire();//Mouse����豸�ͷſ���Ȩ
	}
	if(m_pKeyBoardDevice != NULL)//KeyBoard�����豸ָ�벻ΪNULL
	{
		m_pKeyBoardDevice->Unacquire();//KeyBoard�����豸�ͷſ���Ȩ
	}

	SAFE_RELEASE(m_pMouseDevice);//Mouse����豸�ͷ�
	SAFE_RELEASE(m_pKeyBoardDevice);//KeyBoard�����豸�ͷ�
	SAFE_RELEASE(m_pDirectInput);//DirectInput�ͷ�
}

/*
** Function: DirectInput::Initialize(HWND hWnd, HINSTANCE hInstance, DWORD KeyBoardCoopFlags, DWORD MouseCoopFlags)
** Purpose: DirectInput��ʼ��
** Para: HWND hWnd(���ھ��)
** Para: HINSTANCE hInstance(����ʵ�����)
** Para: DWORD KeyBoardCoopFlags
** Para: DWORD MouseCoopFlags
** Return: HRESULT(��ʼ��״̬:�ɹ�:S_OK,ʧ��:E_FAIL)
*/
HRESULT DirectInput::Initialize(HWND hWnd, HINSTANCE hInstance, DWORD KeyBoardCoopFlags, DWORD MouseCoopFlags)
{
	HRESULT Result;

	//����IDirectInput8�ӿڶ���
	Result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	
	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//����IDirectInputDevice8�ӿڶ���(KeyBoard�豸����)
	Result = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoardDevice, NULL);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//KeyBoard�豸�����ʼ��
	Result = m_pKeyBoardDevice->SetCooperativeLevel(hWnd, KeyBoardCoopFlags);//����Э������

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pKeyBoardDevice->SetDataFormat(&c_dfDIKeyboard);//�������ݸ�ʽ(��׼���̽ṹ)

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pKeyBoardDevice->Acquire();//��ȡ�豸����Ȩ

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pKeyBoardDevice->Poll();//��ѯ�豸

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//����IDirectInputDevice8�ӿڶ���(Mouse����豸����)
	Result = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pMouseDevice->SetCooperativeLevel(hWnd, MouseCoopFlags);//����Э������

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pMouseDevice->SetDataFormat(&c_dfDIMouse);//�������ݸ�ʽ(��׼���ṹ)

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pMouseDevice->Acquire();//��ȡ�豸����Ȩ

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pMouseDevice->Poll();//��ѯ�豸

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	return S_OK;
}

/*
** Function: DirectInput::GetDeviceState(void)
** Purpose: DirectInput��ȡ�豸��Ϣ
*/
void DirectInput::GetDeviceState(void)
{
	HRESULT Result;

	//��ȡKeyBoard�����豸״̬
	Result = m_pKeyBoardDevice->GetDeviceState(sizeof(m_cKeyBoradBuffer), (LPVOID)m_cKeyBoradBuffer);//��ȡKeyBoard�����豸״̬
	if(Result)
	{
		m_pKeyBoardDevice->Poll();//��ѯ�豸
		m_pKeyBoardDevice->Acquire();//��ȡ����Ȩ
		Result = m_pKeyBoardDevice->GetDeviceState(sizeof(m_cKeyBoradBuffer), (LPVOID)m_cKeyBoradBuffer);//��ȡKeyBoard�����豸״̬
	}

	//��ȡMouse����豸״̬
	Result = m_pMouseDevice->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState);//��ȡMouse����豸״̬
	if(Result)
	{
		m_pMouseDevice->Poll();//��ѯ�豸
		m_pMouseDevice->Acquire();//��ȡ����Ȩ
		Result = m_pMouseDevice->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState);//��ȡMouse����豸״̬
	}
}

/*
** Function: DirectInput::IsKeyDown(void)
** Purpose: DirectInput��ȡKeyBoard���̰���״̬
** Return: bool(true:KeyDown,false:KeyUp)
*/
bool DirectInput::IsKeyDown(int KeyValue)
{
	if(m_cKeyBoradBuffer[KeyValue] & 0x80)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
** Function: DirectInput::IsMouseDown(MouseButton MouseValue)
** Purpose: DirectInput��ȡMouse��갴��״̬
** Return: bool(true:KeyDown,false:KeyUp)
*/
bool DirectInput::IsMouseDown(MouseButton MouseValue)
{
	if(m_MouseState.rgbButtons[MouseValue] & 0x80)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
** Function: DirectInput::GetMouseX(void)
** Purpose: DirectInput��ȡMouse���X������
** Return: float(Mouse���X������)
*/
float DirectInput::GetMouseX(void)
{
	return (float)m_MouseState.lX;
}

/*
** Function: DirectInput::GetMouseY(void)
** Purpose: DirectInput��ȡMouse���Y������
** Return: float(Mouse���Y������)
*/
float DirectInput::GetMouseY(void)
{
	return (float)m_MouseState.lY;
}

/*
** Function: DirectInput::GetMouseZ(void)
** Purpose: DirectInput��ȡMouse���Z������(����)
** Return: float(Mouse���Z������)
*/
float DirectInput::GetMouseZ(void)
{
	return (float)m_MouseState.lZ;
}