#include "Common.h"
#include "Include.h"

/*
** Global Variable
*/
HWND hWnd;//���ھ��
HINSTANCE hInst;//��ǰʵ�����ھ��
TCHAR szTitle[MAX_LOADSTRING];//�������ı�
TCHAR szWindowClass[MAX_LOADSTRING];//����������

//UserArea
int g_nUserScreenWidth = 0;//�û����򴰿ڿ��
int g_nUserScreenHeight = 0;//�û����򴰿ڸ߶�

//TimeCaculate
float g_fLastTime = 0.0f;//�ϴγ������е�ʱ��
float g_fCurrentTime = 0.0f;//��ǰ�������е�ʱ��
float g_fDeltaTime = 0.0f;//ʱ���ֵ(Caculate fps)

/*
** Function Declaration
*/
ATOM MyRegisterClass(HINSTANCE hInstance);//ע�ᴰ����
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);//��ʼ������������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);//���������ڵ���Ϣ
void ReleaseWindows(void);//�ͷŴ�����Դ

/*
** WinMain(IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nCmdShow)
** Purpose:  ����������
*/
int WINAPI WinMain(IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);//��������������hPrevInstance����
	UNREFERENCED_PARAMETER(lpCmdLine);//��������������lpCmdLine����

	MSG msg;//������Ϣ

	LoadString(hInstance, IDS_TITLE, szTitle, MAX_LOADSTRING);//�������Ƶ���szTitle����
	LoadString(hInstance, IDS_CLASS, szWindowClass, MAX_LOADSTRING);//������������szWindowClass����

	MyRegisterClass(hInstance);//ע�ᴰ����

	if(!InitInstance(hInstance, nCmdShow))//ִ��Ӧ�ó����ʼ��
	{
		return FALSE;
	}

	//ConsoleInit();//Console����̨���Գ�ʼ��
	
	ZeroMemory(&msg, sizeof(msg));//�����Ϣ��Ϣ
	while(msg.message != WM_QUIT)//����Ϣѭ��
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
			Direct3D_Update(hWnd, g_fDeltaTime);//Direct3D����
			Direct3D_Render(hWnd, g_fDeltaTime);//Direct3D��Ⱦ
			g_fCurrentTime = (float)GetTickCount()*0.001f;
		}
	}

	ReleaseWindows();//�ͷŴ�����Դ

	return (int)msg.wParam;
}

/*
** Function:  MyRegisterClass()
** Purpose:  ע�ᴰ����
*/
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);//WNDCLASSEX���С

	wcex.style = CS_HREDRAW | CS_VREDRAW;//��������ʽ
	wcex.lpfnWndProc = WndProc;//������ص�����
	wcex.cbClsExtra = 0;//������ṹ�����ֽ���
	wcex.cbWndExtra = 0;//����ʵ�������ֽ���
	wcex.hInstance = hInstance;//���ھ��
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_TH));//ͼ����
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);//�����
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//������ˢ��ɫ(�Զ�)
	wcex.lpszMenuName = MAKEINTRESOURCE(IDI_ICON_TH);//�˵�ָ��
	wcex.lpszClassName = szWindowClass;//����ָ��
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_TH));//Сͼ����

	return RegisterClassEx(&wcex);
}

/*
** Function:  InitInstance(HINSTANCE, int)
** Purpose:  ����ʵ�����������������
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hwnd;//���ھ��
	WINDOWINFO Windowinfo;//������Ϣ
	RECT Rect;//������������
	int ScreenWidth = 0;//��Ļ���
	int ScreenHeight = 0;//��Ļ�߶�
	int PosX = 0;//����X����
	int PosY = 0;//����Y����

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	//����Ӧ�������
	SetRect(&Rect, 0, 0, USER_SCREENWIDTH, USER_SCREENHEIGHT);
	AdjustWindowRect(&Rect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);
	hwnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, 0, (Rect.right - Rect.left), (Rect.bottom - Rect.top), NULL, NULL, hInstance, NULL);

	if(!hwnd)
	{
		return FALSE;
	}

	hWnd = hwnd;	// �����ھ���洢��ȫ�ֱ�����

	//��ȡ��ǰ������Ϣ
	GetWindowInfo(hwnd, &Windowinfo);//��ȡ������Ϣ
	g_nUserScreenWidth = Windowinfo.rcClient.right - Windowinfo.rcClient.left;//�û�������
	g_nUserScreenHeight = Windowinfo.rcClient.bottom - Windowinfo.rcClient.top;//�û�����߶�

	//���㴰�ڳ�ʼ����
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);//��Ļ���
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);//��Ļ�߶�

	//���ô���λ��(Center)
	if(ScreenWidth > g_nUserScreenWidth && ScreenHeight > g_nUserScreenHeight)
	{
		PosX = (ScreenWidth - g_nUserScreenWidth) >> 1;//����X����
		PosY = (ScreenHeight - g_nUserScreenHeight) >> 1;//����Y����
		SetWindowPos(hwnd, HWND_TOP, PosX, PosY, 0, 0, SWP_NOSIZE);//���ô���λ��(ScreenCenter)
	}

	//Direct3D��ʼ��
	if(!(Direct3D_Init(hwnd) == S_OK))
	{
		MessageBox(hwnd, _T("Direct3D��ʼ��ʧ��~!"), _T("Win32"), 0);
	}

	//Direct3DӦ�ó�ʼ��
	if(!(Direct3D_SetUp() == S_OK))
	{
		MessageBox(hwnd, _T("Ӧ�ó�ʼ��ʧ��~!"), _T("Win32"), 0);
	}

	//DirectInput��ʼ��
	if(!(DirectInput_Init(hwnd, hInstance) == S_OK))
	{
		MessageBox(hwnd, _T("DirectInput��ʼ��ʧ��~!"), _T("Win32"), 0);
	}

	//DirectSound��ʼ��
	if(!(DirectSound_Init(hwnd)) == S_OK)
	{
		MessageBox(hwnd, _T("DircetSound��ʼ��ʧ��~!"), _T("Win32"), 0);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	return TRUE;
}

/*
** Function:  WndProc(HWND, UINT, WPARAM, LPARAM)
** Purpose:    ���������ڵ���Ϣ
** WM_COMMAND	- ����Ӧ�ó���˵�
** WM_PAINT - ����������
** WM_DESTROY - �����˳���Ϣ������
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch(message)
	{
	case WM_COMMAND://��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//֪ͨ��
		break;
	case WM_ERASEBKGND://���ڲ���
		break;
	case WM_KEYDOWN://���ڰ���
		break;
	case WM_PAINT://�����ػ�
		hdc = BeginPaint(hWnd, &ps);//��ʼ�ػ�
		EndPaint(hWnd, &ps);//�����ػ�
		break;
	case WM_TIMER:
		break;
	case WM_DESTROY://��������
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/*
** Function:  ReleaseWindows(void)
** Purpose:  �ͷŴ�����Դ
*/
void ReleaseWindows(void)
{
	Direct3D_ClearUp();
}