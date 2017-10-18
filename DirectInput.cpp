#include"Common.h"
#include "Include.h"
#include "DirectInput.h"


/*
** Function:	 DirectInput()
** Purpose: DirectInput构造函数
*/
DirectInput::DirectInput()
{
	m_pDirectInput = NULL;//IDirectInput8接口对象指针NULL
	m_pKeyBoardDevice = NULL;//KeyBoard键盘设备指针NULL
	m_pMouseDevice = NULL;//Mouse鼠标设备指针NULL
	ZeroMemory(&m_cKeyBoradBuffer, sizeof(char) * KEYBOARD_ARRAYSIZE);//清除KeyBoradBuffer
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));//清除MouseState
}

/*
** Function: ~DirectInput()
** Purpose: DirectInput析构函数
*/
DirectInput::~DirectInput()
{
	if(m_pMouseDevice != NULL)//Mouse鼠标设备指针不为NULL
	{
		m_pMouseDevice->Unacquire();//Mouse鼠标设备释放控制权
	}
	if(m_pKeyBoardDevice != NULL)//KeyBoard键盘设备指针不为NULL
	{
		m_pKeyBoardDevice->Unacquire();//KeyBoard键盘设备释放控制权
	}

	SAFE_RELEASE(m_pMouseDevice);//Mouse鼠标设备释放
	SAFE_RELEASE(m_pKeyBoardDevice);//KeyBoard键盘设备释放
	SAFE_RELEASE(m_pDirectInput);//DirectInput释放
}

/*
** Function: DirectInput::Initialize(HWND hWnd, HINSTANCE hInstance, DWORD KeyBoardCoopFlags, DWORD MouseCoopFlags)
** Purpose: DirectInput初始化
** Para: HWND hWnd(窗口句柄)
** Para: HINSTANCE hInstance(窗口实例句柄)
** Para: DWORD KeyBoardCoopFlags
** Para: DWORD MouseCoopFlags
** Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
*/
HRESULT DirectInput::Initialize(HWND hWnd, HINSTANCE hInstance, DWORD KeyBoardCoopFlags, DWORD MouseCoopFlags)
{
	HRESULT Result;

	//创建IDirectInput8接口对象
	Result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	
	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//创建IDirectInputDevice8接口对象(KeyBoard设备对象)
	Result = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoardDevice, NULL);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//KeyBoard设备对象初始化
	Result = m_pKeyBoardDevice->SetCooperativeLevel(hWnd, KeyBoardCoopFlags);//设置协作级别

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pKeyBoardDevice->SetDataFormat(&c_dfDIKeyboard);//设置数据格式(标准键盘结构)

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pKeyBoardDevice->Acquire();//获取设备控制权

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pKeyBoardDevice->Poll();//轮询设备

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//创建IDirectInputDevice8接口对象(Mouse鼠标设备对象)
	Result = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pMouseDevice->SetCooperativeLevel(hWnd, MouseCoopFlags);//设置协作级别

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pMouseDevice->SetDataFormat(&c_dfDIMouse);//设置数据格式(标准鼠标结构)

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pMouseDevice->Acquire();//获取设备控制权

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = m_pMouseDevice->Poll();//轮询设备

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	return S_OK;
}

/*
** Function: DirectInput::GetDeviceState(void)
** Purpose: DirectInput获取设备信息
*/
void DirectInput::GetDeviceState(void)
{
	HRESULT Result;

	//获取KeyBoard键盘设备状态
	Result = m_pKeyBoardDevice->GetDeviceState(sizeof(m_cKeyBoradBuffer), (LPVOID)m_cKeyBoradBuffer);//获取KeyBoard键盘设备状态
	if(Result)
	{
		m_pKeyBoardDevice->Poll();//轮询设备
		m_pKeyBoardDevice->Acquire();//获取控制权
		Result = m_pKeyBoardDevice->GetDeviceState(sizeof(m_cKeyBoradBuffer), (LPVOID)m_cKeyBoradBuffer);//获取KeyBoard键盘设备状态
	}

	//获取Mouse鼠标设备状态
	Result = m_pMouseDevice->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState);//获取Mouse鼠标设备状态
	if(Result)
	{
		m_pMouseDevice->Poll();//轮询设备
		m_pMouseDevice->Acquire();//获取控制权
		Result = m_pMouseDevice->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState);//获取Mouse鼠标设备状态
	}
}

/*
** Function: DirectInput::IsKeyDown(void)
** Purpose: DirectInput获取KeyBoard键盘按键状态
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
** Purpose: DirectInput获取Mouse鼠标按键状态
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
** Purpose: DirectInput获取Mouse鼠标X轴坐标
** Return: float(Mouse鼠标X轴坐标)
*/
float DirectInput::GetMouseX(void)
{
	return (float)m_MouseState.lX;
}

/*
** Function: DirectInput::GetMouseY(void)
** Purpose: DirectInput获取Mouse鼠标Y轴坐标
** Return: float(Mouse鼠标Y轴坐标)
*/
float DirectInput::GetMouseY(void)
{
	return (float)m_MouseState.lY;
}

/*
** Function: DirectInput::GetMouseZ(void)
** Purpose: DirectInput获取Mouse鼠标Z轴坐标(滚轮)
** Return: float(Mouse鼠标Z轴坐标)
*/
float DirectInput::GetMouseZ(void)
{
	return (float)m_MouseState.lZ;
}