#include "Common.h"
#include "Include.h"

/*
** Global Variable
*/
HWND hWnd;//窗口句柄
HINSTANCE hInst;//当前实例窗口句柄
TCHAR szTitle[MAX_LOADSTRING];//标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];//主窗口类名

//UserArea
int g_nUserScreenWidth = 0;//用户区域窗口宽度
int g_nUserScreenHeight = 0;//用户区域窗口高度

//TimeCaculate
float g_fLastTime = 0.0f;//上次程序运行的时间
float g_fCurrentTime = 0.0f;//当前程序运行的时间
float g_fDeltaTime = 0.0f;//时间差值(Caculate fps)

/*
** Function Declaration
*/
ATOM MyRegisterClass(HINSTANCE hInstance);//注册窗口类
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);//初始化创建主窗口
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);//处理主窗口的消息
void ReleaseWindows(void);//释放窗口资源

/*
** WinMain(IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nCmdShow)
** Purpose:  窗口主函数
*/
int WINAPI WinMain(IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);//编译器不检测变量hPrevInstance警告
	UNREFERENCED_PARAMETER(lpCmdLine);//编译器不检测变量lpCmdLine警告

	MSG msg;//窗口消息

	LoadString(hInstance, IDS_TITLE, szTitle, MAX_LOADSTRING);//窗口名称导入szTitle数组
	LoadString(hInstance, IDS_CLASS, szWindowClass, MAX_LOADSTRING);//窗口类名导入szWindowClass数组

	MyRegisterClass(hInstance);//注册窗口类

	if(!InitInstance(hInstance, nCmdShow))//执行应用程序初始化
	{
		return FALSE;
	}

	//ConsoleInit();//Console控制台调试初始化
	
	ZeroMemory(&msg, sizeof(msg));//清除消息信息
	while(msg.message != WM_QUIT)//主消息循环
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_fDeltaTime = g_fCurrentTime - g_fLastTime;
			g_fLastTime = (float)GetTickCount()*0.001f;
			Direct3D_Update(hWnd, g_fDeltaTime);//Direct3D更新
			Direct3D_Render(hWnd, g_fDeltaTime);//Direct3D渲染
			g_fCurrentTime = (float)GetTickCount()*0.001f;
		}
	}

	ReleaseWindows();//释放窗口资源

	return (int)msg.wParam;
}

/*
** Function:  MyRegisterClass()
** Purpose:  注册窗口类
*/
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);//WNDCLASSEX类大小

	wcex.style = CS_HREDRAW | CS_VREDRAW;//窗口类样式
	wcex.lpfnWndProc = WndProc;//窗口类回调函数
	wcex.cbClsExtra = 0;//窗口类结构附加字节数
	wcex.cbWndExtra = 0;//窗口实例附加字节数
	wcex.hInstance = hInstance;//窗口句柄
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_TH));//图标句柄
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);//光标句柄
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//背景画刷颜色(自动)
	wcex.lpszMenuName = MAKEINTRESOURCE(IDI_ICON_TH);//菜单指针
	wcex.lpszClassName = szWindowClass;//类名指针
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_TH));//小图标句柄

	return RegisterClassEx(&wcex);
}

/*
** Function:  InitInstance(HINSTANCE, int)
** Purpose:  保存实例句柄并创建主窗口
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hwnd;//窗口句柄
	WINDOWINFO Windowinfo;//窗口信息
	RECT Rect;//创建矩形区域
	int ScreenWidth = 0;//屏幕宽度
	int ScreenHeight = 0;//屏幕高度
	int PosX = 0;//窗口X坐标
	int PosY = 0;//窗口Y坐标

	hInst = hInstance; // 将实例句柄存储在全局变量中

	//自适应窗体界面
	SetRect(&Rect, 0, 0, USER_SCREENWIDTH, USER_SCREENHEIGHT);
	AdjustWindowRect(&Rect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);
	hwnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, 0, (Rect.right - Rect.left), (Rect.bottom - Rect.top), NULL, NULL, hInstance, NULL);

	if(!hwnd)
	{
		return FALSE;
	}

	hWnd = hwnd;	// 将窗口句柄存储在全局变量中

	//获取当前窗口信息
	GetWindowInfo(hwnd, &Windowinfo);//获取窗口信息
	g_nUserScreenWidth = Windowinfo.rcClient.right - Windowinfo.rcClient.left;//用户区域宽度
	g_nUserScreenHeight = Windowinfo.rcClient.bottom - Windowinfo.rcClient.top;//用户区域高度

	//计算窗口初始坐标
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);//屏幕宽度
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);//屏幕高度

	//设置窗口位置(Center)
	if(ScreenWidth > g_nUserScreenWidth && ScreenHeight > g_nUserScreenHeight)
	{
		PosX = (ScreenWidth - g_nUserScreenWidth) >> 1;//窗口X坐标
		PosY = (ScreenHeight - g_nUserScreenHeight) >> 1;//窗口Y坐标
		SetWindowPos(hwnd, HWND_TOP, PosX, PosY, 0, 0, SWP_NOSIZE);//设置窗口位置(ScreenCenter)
	}

	//Direct3D初始化
	if(!(Direct3D_Init(hwnd) == S_OK))
	{
		MessageBox(hwnd, _T("Direct3D初始化失败~!"), _T("Win32"), 0);
	}

	//Direct3D应用初始化
	if(!(Direct3D_SetUp() == S_OK))
	{
		MessageBox(hwnd, _T("应用初始化失败~!"), _T("Win32"), 0);
	}

	//DirectInput初始化
	if(!(DirectInput_Init(hwnd, hInstance) == S_OK))
	{
		MessageBox(hwnd, _T("DirectInput初始化失败~!"), _T("Win32"), 0);
	}

	//DirectSound初始化
	if(!(DirectSound_Init(hwnd)) == S_OK)
	{
		MessageBox(hwnd, _T("DircetSound初始化失败~!"), _T("Win32"), 0);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	return TRUE;
}

/*
** Function:  WndProc(HWND, UINT, WPARAM, LPARAM)
** Purpose:    处理主窗口的消息
** WM_COMMAND	- 处理应用程序菜单
** WM_PAINT - 绘制主窗口
** WM_DESTROY - 发送退出消息并返回
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch(message)
	{
	case WM_COMMAND://窗口命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//通知码
		break;
	case WM_ERASEBKGND://窗口擦除
		break;
	case WM_KEYDOWN://窗口按键
		break;
	case WM_PAINT://窗口重绘
		hdc = BeginPaint(hWnd, &ps);//开始重绘
		EndPaint(hWnd, &ps);//结束重绘
		break;
	case WM_TIMER:
		break;
	case WM_DESTROY://窗口销毁
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/*
** Function:  ReleaseWindows(void)
** Purpose:  释放窗口资源
*/
void ReleaseWindows(void)
{
	Direct3D_ClearUp();
}