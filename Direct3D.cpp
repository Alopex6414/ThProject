#include"Common.h"
#include "Include.h"

/*
** Global Variable
*/
//D3DDevice
LPDIRECT3DDEVICE9 g_pD3D9Device = NULL;//Direct3D设备

//D3DVertex/D3DIndex
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;//顶点缓存
LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;//索引缓存

//D3DTexture
LPDIRECT3DTEXTURE9 g_pTexture = NULL;//标题纹理
LPDIRECT3DTEXTURE9 g_pTextureLoading = NULL;//等待纹理
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;//标题纹理
LPDIRECT3DTEXTURE9 g_pTextureLogo = NULL;//标题logo纹理
LPDIRECT3DTEXTURE9 g_pTextureVersion = NULL;//标题版本纹理
LPDIRECT3DTEXTURE9 g_pTextureTitleText = NULL;//标题文字纹理

//D3DFont
LPD3DXFONT g_pFontFPS = NULL;//帧速率字符接口
LPD3DXFONT g_pFontAdapter = NULL;//显卡型号信息字符接口

//DirectInput
DirectInput*g_pDirectInput = new DirectInput();//DirectInput接口

//DirectSound
DirectSound*g_pDirectSound = new DirectSound();//DirectSound接口(音乐)
DirectSound*g_pDirectSoundEffect = new DirectSound();//DirectSound接口(音效)

//Particle
Particle*g_pSnow;//粒子

//Animation
Animation*g_pAnimation;//动画

//Subtitle
//Sig...
Subtitle*g_pSubSig = new Subtitle(0.0f, 0.0f, 640.0f, 480.0f, 0.0f);//Sig背景页面
Subtitle*g_pSubLoad = new Subtitle(415.0f, 375.0f, 128.0f, 32.0f, 0.0f, 0.0f, false);//"少女祈祷中..."
Subtitle*g_pSubLoadEn = new Subtitle(462.0f, 392.0f, 128.0f, 32.0f, 20.0f, 0.0f, true);//"Loading..."
//Title...
Subtitle*g_pSubTitle = new Subtitle(0.0f, 0.0f, 640.0f, 480.0f, 0.0f);//Title背景页面
Subtitle*g_pSubVersion = new Subtitle(304.0f, 431.0f, 64.0f, 16.0f, 0.0f);//Version版本号
Subtitle*g_pSubGameOptionSelect = new Subtitle(540.0f, 256.0f, 128.0f, 32.0f, 0.0f, 0.0f, true);//GameOptionSelect游戏选中项
Subtitle*g_pSubArrayGameOptions = new Subtitle[8]//GameOptions游戏选项(主界面)
{
	Subtitle(540.0f, 256.0f, 128.0f, 32.0f, 540.0f, 0.0f, true),//GameStart选项
	Subtitle(556.0f, 280.0f, 128.0f, 32.0f, 556.0f, 0.0f, true),//ExtraStart选项
	Subtitle(572.0f, 304.0f, 128.0f, 32.0f, 572.0f, 0.0f, true),//PracticeStart选项
	Subtitle(588.0f, 328.0f, 128.0f, 32.0f, 588.0f, 0.0f, true),//Replay选项
	Subtitle(604.0f, 352.0f, 128.0f, 32.0f, 604.0f, 0.0f, true),//PlayerData选项
	Subtitle(620.0f, 376.0f, 128.0f, 32.0f, 620.0f, 0.0f, true),//MusicRoom选项
	Subtitle(636.0f, 400.0f, 128.0f, 32.0f, 636.0f, 0.0f, true),//Option选项
	Subtitle(652.0f, 424.0f, 128.0f, 32.0f, 652.0f, 0.0f, true),//Quit选项
};
//Option...
Subtitle*g_pSubOptionConfigSelect = new Subtitle(540.0f, 304.0f, 128.0f, 32.0f, 540.0f, 0.0f, true);//OptionConfig选中项
Subtitle*g_pSubArrayOptionsConfig = new Subtitle[5]//OptionsConfig选项设置(Option界面)
{
	Subtitle(540.0f, 304.0f, 128.0f, 32.0f, 540.0f, 0.0f, true),//BGM Volume选项
	Subtitle(556.0f, 328.0f, 128.0f, 32.0f, 556.0f, 0.0f, true),//SE Volume选项
	Subtitle(572.0f, 352.0f, 128.0f, 32.0f, 572.0f, 0.0f, true),//Key Config选项
	Subtitle(588.0f, 376.0f, 128.0f, 32.0f, 588.0f, 0.0f, true),//Default选项
	Subtitle(604.0f, 400.0f, 128.0f, 32.0f, 604.0f, 0.0f, true),//Quit选项
};

//Variable
float	g_FPS = 0.0f;//帧速率
wchar_t	g_sFPS[20];//fps帧速率字符数组
wchar_t g_sAdapterName[60];//Adapter显卡名称字符数组
int g_nTitleFlagNow = 0;
int g_nKeyNumber = 0;//Title00
int g_nKeyNumber1 = 0;//Title01
int g_nKeyNumber_Option = 0;//Option
int g_nKeySelectState = 0;//Title状态
bool g_bKeySelectFlashState = false;
bool g_bKeySelectTitleChange = false;

/*
** Function:  Direct3D_Init(HWND hWnd)
** Purpose:  Direct3D初始化
** Para: HWND hWnd(窗口句柄)
** Return:  HRESULT初始化状态
*/
HRESULT Direct3D_Init(HWND hWnd)
{
	IDirect3D9 *pD3D9 = NULL;
	D3DCAPS9 Caps;
	D3DPRESENT_PARAMETERS D3Dpp;
	D3DADAPTER_IDENTIFIER9 Adapter;
	HRESULT Result;
	int Vp;
	int Length;
	wchar_t AdapterName[60] = L"Adapter Version:";

	//Step1 获取IDirect3D9接口指针
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);//获取IDirect3D9接口指针

	if(pD3D9 == NULL)//IDirect3D9接口指针为NULL
	{
		return E_FAIL;//FAIL
	}

	//Step2 校验硬件顶点运算
	Result = pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps);//校验硬件顶点运算

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	if(Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		Vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;//支持硬件顶点运算(硬件顶点运算)
	}
	else
	{
		Vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//不支持硬件顶点运算(软件顶点运算)
	}

	//Step3 填充D3DPRESENT_PARAMETERS结构
	D3Dpp.BackBufferWidth = USER_SCREENWIDTH;//后台缓冲表面宽度(Pixel)
	D3Dpp.BackBufferHeight = USER_SCREENHEIGHT;//后台缓冲表面高度(Pixel)
	D3Dpp.BackBufferFormat = D3DFMT_A8R8G8B8;//后台缓冲像素格式
	D3Dpp.BackBufferCount = 1;//后台缓冲数量(1)
	D3Dpp.MultiSampleType = D3DMULTISAMPLE_NONE;//后台缓冲多重采样类型
	D3Dpp.MultiSampleQuality = 0;//后台缓冲多重采样质量
	D3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//交换链页面置换方式
	D3Dpp.hDeviceWindow = hWnd;//设备相关窗口句柄
	D3Dpp.Windowed = true;//窗口模式:true/全屏模式:false
	D3Dpp.EnableAutoDepthStencil = true;//Direct3D自动创建维护深度缓冲和模板缓冲
	D3Dpp.AutoDepthStencilFormat = D3DFMT_D24S8;//深度缓冲和模板缓冲像素格式
	D3Dpp.Flags = 0;//无标记格式
	D3Dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//Direct3D默认刷新频率
	D3Dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//Direct3D提交频率(默认提交)

	//Step4 创建IDirect3DDevice9接口
	Result = pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, Vp, &D3Dpp, &g_pD3D9Device);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//Step4 获取Adapter显卡型号
	pD3D9->GetAdapterIdentifier(0, 0, &Adapter);
	Length = MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, g_sAdapterName, Length);
	wcscat_s(AdapterName, g_sAdapterName);
	wcscpy_s(g_sAdapterName, AdapterName);

	//Step5 释放资源
	SAFE_RELEASE(pD3D9);

	return S_OK;
}

/*
** Function:  Direct3D_SetUp()
** Purpose:  Direct3D应用程序初始化
** Return:  HRESULT初始化状态
*/
HRESULT Direct3D_SetUp(void)
{
	HRESULT Result;

	//创建D3DFont字体

	//D3DFont帧速率fps
	Result = D3DXCreateFont(g_pD3D9Device, 12, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("等线"), &g_pFontFPS);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//D3DFont显卡型号Adapter
	Result = D3DXCreateFont(g_pD3D9Device, 20, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &g_pFontAdapter);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//创建D3DVertex/D3DIndex顶点/索引

	//创建顶点缓存(48)
	Result = g_pD3D9Device->CreateVertexBuffer(48 * sizeof(Vertex), 0, D3DFVF_VERTEX, D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//创建索引缓存(72)
	Result = g_pD3D9Device->CreateIndexBuffer(72 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIndexBuffer, NULL);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//创建D3DTexture纹理
	Result = D3DXCreateTextureFromFileEx(g_pD3D9Device, L"Resource\\loading\\sig.png", 1024, 512, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, D3DCOLOR_XRGB(0, 0, 0), NULL, NULL, &g_pTexture);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = D3DXCreateTextureFromFileEx(g_pD3D9Device, L"Resource\\title\\title00a.png", 1024, 512, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, D3DCOLOR_XRGB(0, 0, 0), NULL, NULL, &g_pTextureTitle);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = D3DXCreateTextureFromFile(g_pD3D9Device, L"Resource\\ascii\\loading.png", &g_pTextureLoading);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = D3DXCreateTextureFromFile(g_pD3D9Device, L"Resource\\title\\title_logo.png", &g_pTextureLogo);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = D3DXCreateTextureFromFile(g_pD3D9Device, L"Resource\\title\\title_ver.png", &g_pTextureVersion);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = D3DXCreateTextureFromFile(g_pD3D9Device, L"Resource\\title\\title01.png", &g_pTextureTitleText);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//创建粒子系统
	g_pSnow = new Particle(g_pD3D9Device, 420, 350, 160, 90);
	Result = g_pSnow->Initialize(L"Resource\\ascii\\loading.png");

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//创建动画系统
	g_pAnimation = new Animation(g_pD3D9Device);
	Result = g_pAnimation->Initialize(L"Resource\\title\\title_logo.png");

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	return S_OK;
}

/*
** Function:  DirectInput_Init(void)
** Purpose:  DirectInput初始化
** Para: HWND hWnd(窗口句柄)
** Para: HINSTANCE hInstance(窗口实例句柄)
** Return:  HRESULT初始化状态
*/
HRESULT DirectInput_Init(HWND hWnd, HINSTANCE hInstance)
{
	HRESULT Result;

	Result = g_pDirectInput->Initialize(hWnd, hInstance, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);//KeyBoard,Mouse均为设备后台模式/共享模式(设置成前台模式/独占模式?)

	return Result;
}

/*
** Function:  DirectSound_Init(void)
** Purpose:  DirectSound初始化
** Para: HWND hWnd(窗口句柄)
** Return:  HRESULT初始化状态
*/
HRESULT DirectSound_Init(HWND hWnd)
{
	HRESULT Result;

	Result = g_pDirectSound->Initialize(hWnd, DSSCL_NORMAL);//DirectSound BGM正常协作级别

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = g_pDirectSound->LoadWave(L"Resource\\bgm\\th11_01.wav");//DirectSound BGM加载背景音乐

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = g_pDirectSoundEffect->Initialize(hWnd, DSSCL_NORMAL);//DirectSound Effect正常协作级别

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound Effect加载音效

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	return S_OK;
}

/*
** Function:  Direct3D_Update(HWND hWnd, float fDeltaTime)
** Purpose:  Direct3D更新
** Para: HWND hWnd(窗口句柄)
** Para: float fDeltaTime(时间间隔)
*/
void Direct3D_Update(HWND hWnd, float fDeltaTime)
{
	static float fTimeSum = 0.0f;
	static float fAlphaValue = 0.0f;
	static bool bAlphaFlag = true;
	static float fAlphaValueArray[8] = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	static float fVertexXArray[8] = { 540.0f,556.0f,572.0f,588.0f,604.0f,620.0f,636.0f,652.0f };
	static bool bKeySelect = false;
	static float fKeyPressTime = 0.0f;
	static bool bKeyCancelLast = false;
	static bool bKeyMoveFlag = false;
	static bool bKeyMoveDirect = false;
	static float fKeyMoveX = 0.0f;
	static bool bKeySelectX = false;
	static bool bKeySelectZ = false;
	static int nKeySelectFlashCount = 0;
	static int nKeySelectFlashTimesCount = 0;
	static bool bKeySelectFlashState = false;
	static bool bKeyEnableFlag = false;
	float *pfVertexX = NULL;

	fTimeSum += fDeltaTime;//游戏运行时间

	if(fTimeSum < 5.0f)//Loading(Loading持续5.0s)
	{
		SceneSig_Update(fTimeSum, fDeltaTime);//Sig场景更新
	}
	else//Title主界面(5.0s后进入Title界面)
	{
		SceneSig_Release();//Sig场景释放

		if(g_nKeySelectState == 0)
		{
			SceneTitle_Update(hWnd, fDeltaTime);
		}
		else if(g_nKeySelectState == 1)
		{
			switch(g_nKeyNumber)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				SceneOption_Update(fDeltaTime);
				break;
			default:
				break;
			}
		}
		else
		{

		}


		/*//Play Start BGM
		g_pDirectSound->PlayMusic();//播放标题BGM音乐

		if(g_nTitleFlagNow < 5)//Title动画标志0~4(黑色背景)("東方地霊殿")
		{
			fAlphaValue = 0.5f;//"少女祈祷中..."透明度Alpha 0.0f, 0%//0.5f
			bAlphaFlag = true;//Alpha标志设置递增

			g_pSubTitle->SetAlpha(0.0f);//Title标题背景Alpha透明度0.0f(0%)
			g_pSubVersion->SetAlpha(0.0f);//Version版本号Alpha透明度0.0f(0%)
			g_pSubGameOptionSelect->SetAlpha(0.5f);//游戏选中项Alpha透明度0.5f(50%)
		}

		if(g_nTitleFlagNow == 5)//Title动画标志5(Title背景纹理加载)
		{
			//Title背景纹理加载
			g_pSubTitle->AlphaIncrease(1.0f, 0.02f);//Title标题透明度Alpha增速0.02f,持续50帧,0.83s
			if(g_pSubTitle->GetAlpha() >= 0.8f)
			{
				g_pSubVersion->AlphaIncrease(1.0f, 0.1f);//"V1.00a"透明度Alpha增速0.1f,持续10帧
			}

			//"游戏选项"坐标偏移
			pfVertexX = &fVertexXArray[0];//"游戏选项"字符偏移位置向左移动(8x/frame)
			for(int i = 0; i < 8; i++)
			{
				*(pfVertexX + i) += -8;
				if(*(pfVertexX + i) < 476.0f)//"游戏选项"坐标限制(X:476.0f)
				{
					*(pfVertexX + i) = 476.0f;
				}
			}

			//-----------------------------------------------游戏坐标偏移----------------------------------------------------
			for(int i = 0; i < 8; i++)
			{
				(g_pSubArrayGameOptions + i)->MoveXDecrease(476.0f, 8.0f);//GameOptions游戏选项X坐标偏移
			}

			//"游戏选项"Alpha透明度变化
			pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道由0.0f,增速1.0f,持续10帧
			for(int i = 0; i < 8; i++)
			{
				if(*(pfVertexX + i) == 0.0f)//"游戏选项"字符只有第一个Alpha为0.0f增加,其后字符Alpha不增加(break)
				{
					*(pfVertexX + i) += 0.1f;
					break;
				}
				else
				{
					*(pfVertexX + i) += 0.1f;//其余字符Alpha值不为0.0f的增速0.1f
					if(*(pfVertexX + i) > 1.0f)//Alpha限幅1.0f
					{
						*(pfVertexX + i) = 1.0f;
					}
				}
			}

			//----------------------------------------游戏选项Alpha透明度变化-------------------------------------------
			for(int i = 0; i < 8; i++)
			{
				if((g_pSubArrayGameOptions + i)->GetAlpha() == 0.0f)//GameOptions游戏选项只有第一个Alpha为0.0f增加,其后字符Alpha不增加(break)
				{
					(g_pSubArrayGameOptions + i)->AddAlpha(0.1f);
					break;
				}
				else
				{
					(g_pSubArrayGameOptions + i)->AlphaIncrease(1.0f, 0.1f);
				}
			}

			//"游戏选项"Alpha通道变化
			if(bAlphaFlag == true)//Alpha值递增,增速0.025f,持续40帧
			{
				fAlphaValue += 0.025f;
				if(fAlphaValue > 1.0f)//Alpha值限幅1.0f
				{
					fAlphaValue = 1.0f;
					bAlphaFlag = false;
				}
			}
			else//Alpha值递减,减速0.025f,持续40帧
			{
				fAlphaValue += -0.025f;
				if(fAlphaValue < 0.5f)//Alpha值限幅0.5f
				{
					fAlphaValue = 0.5f;
					bAlphaFlag = true;
				}
			}

			//----------------------------------------游戏选中项Alpha透明度变化-------------------------------------------
			g_pSubGameOptionSelect->AlphaCircle(0.5f, 1.0f, 0.025f);//GameOptionSelect游戏选中项Alpha透明度变化
		}

		if(g_nTitleFlagNow == 6)//Title动画标志6(DirectInput设备(键盘KeyBoard)控制游戏选项)
		{
			g_pDirectInput->GetDeviceState();//获取按键状态(KeyBoard)

			if(g_nKeySelectState == 0)//状态0:Tilte00界面(按键主界面)
			{
				if(bKeySelect == false)//按键未选中(按键状态标志)
				{
					//检测方向键"↑"状态
					if(g_pDirectInput->IsKeyDown(DIK_UP))//方向键"↑"
					{
						bKeySelect = true;//按键已选中
						g_nKeyNumber--;
						if(g_nKeyNumber < 0)
						{
							g_nKeyNumber = 7;
						}

						g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_select00.wav");//DirectSound加载播放音效se_select00.wav

						bKeyMoveFlag = false;
						bKeyMoveDirect = false;
						fKeyMoveX = 0.0f;
					}

					//检测方向键"↓"状态
					if(g_pDirectInput->IsKeyDown(DIK_DOWN))//方向键"↓"
					{
						bKeySelect = true;//按键已选中
						g_nKeyNumber++;
						if(g_nKeyNumber > 7)
						{
							g_nKeyNumber = 0;
						}

						g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_select00.wav");//DirectSound加载播放音效se_select00.wav

						bKeyMoveFlag = false;
						bKeyMoveDirect = false;
						fKeyMoveX = 0.0f;
					}

					//检测"Z"键状态
					if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"键确定
					{
						//bKeySelect = true;
						if(bKeySelectZ == false)//"Z"键释放
						{
							bKeySelect = true;

							g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_ok00.wav");//DirectSound加载播放音效se_ok00.wav

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}
						bKeySelectZ = true;//"Z"键按下
					}
					else
					{
						bKeySelectZ = false;//"Z"键释放
					}

					//检测"X"键/"Esc"键状态
					if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"键/"Esc"键取消
					{
						if(bKeyCancelLast == false)//"Esc"退出标志未置位
						{
							bKeySelect = true;
							if(g_nKeyNumber != 7)
							{
								g_nKeyNumber = 7;
								g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载播放音效se_cancel00.wav
							}
							else
							{
								g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载播放音效se_cancel00.wav
								Sleep(500);//线程睡眠500ms
								DestroyWindow(hWnd);//销毁窗口
							}

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						bKeyCancelLast = true;//"Esc"退出标志置位
					}
					else
					{
						bKeyCancelLast = false;//"Esc"退出标志未置位
					}

					//"游戏选项"Alpha通道变化
					if(bAlphaFlag == true)//Alpha值递增,增速0.025f,持续40帧
					{
						fAlphaValue += 0.025f;
						if(fAlphaValue > 1.0f)//Alpha值限幅1.0f
						{
							fAlphaValue = 1.0f;
							bAlphaFlag = false;
						}
					}
					else//Alpha值递减,减速0.025f,持续40帧
					{
						fAlphaValue += -0.025f;
						if(fAlphaValue < 0.5f)//Alpha值限幅0.5f
						{
							fAlphaValue = 0.5f;
							bAlphaFlag = true;
						}
					}
				}
				else//按键已选中
				{
					if(bKeySelectZ == false)//"Z"键释放状态
					{
						fKeyPressTime += fDeltaTime;//计算叠加时间间隔
						if(fKeyPressTime > 0.12f && bKeyMoveFlag == true)//按键叠加时间0.12s(KeyBoard消抖0.12s)
						{
							fKeyPressTime = 0.0f;
							bKeySelect = false;//按键未选中
						}

						if(bKeyMoveFlag == false)//按键移动标志(Left2->Right4->Left2)
						{
							if(bKeyMoveDirect == false)//按键移动方向(false:Left/true:Right)
							{
								fKeyMoveX += 2.0f;//按键移动X偏移增量2.0f
								if(fKeyMoveX >= 4.0f)//Left:2帧
								{
									fKeyMoveX = 4.0f;
									bKeyMoveDirect = true;//Right
								}
								if(fKeyMoveX == 0.0f)//Left:2帧
								{
									bKeyMoveFlag = true;
								}
							}
							else//Right:4帧
							{
								fKeyMoveX -= 2.0f;
								if(fKeyMoveX <= -4.0f)
								{
									fKeyMoveX = -4.0f;
									bKeyMoveDirect = false;
								}
							}
						}
						else
						{
							fKeyMoveX = 0.0f;//按键偏移位置原点
						}
					}
					else//"Z"键按下
					{
						if(bKeySelectFlashState == false)
						{
							nKeySelectFlashCount++;
							if(nKeySelectFlashCount >= 2)
							{
								nKeySelectFlashCount = 0;
								bKeySelectFlashState = true;
								nKeySelectFlashTimesCount++;
							}
						}
						else
						{
							nKeySelectFlashCount++;
							if(nKeySelectFlashCount >= 2)
							{
								nKeySelectFlashCount = 0;
								bKeySelectFlashState = false;
								nKeySelectFlashTimesCount++;
							}
						}

						if(nKeySelectFlashTimesCount >= 9)
						{
							//"游戏选项"坐标偏移
							pfVertexX = &fVertexXArray[0];//"游戏选项"字符偏移位置向右移动(8x/frame)
							for(int i = 0; i < 8; i++)
							{
								if(i != g_nKeyNumber)//当前"游戏选项"字符偏移不变
								{
									*(pfVertexX + i) += 4;
									if(*(pfVertexX + i) > 640.0f)//"游戏选项"坐标限制(X:640.0f)
									{
										*(pfVertexX + i) = 640.0f;
									}
								}
								else
								{
									if(nKeySelectFlashTimesCount >= 13)
									{
										*(pfVertexX + i) += 8;
										if(*(pfVertexX + i) > 640.0f)//"游戏选项"坐标限制(X:640.0f)
										{
											*(pfVertexX + i) = 640.0f;
										}
									}
								}
							}

							//"游戏选项"Alpha透明度变化
							pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道由0.0f,增速1.0f,持续10帧(fAlphaValueArray数组此时值均为1.0f)
							for(int i = 0; i < 8; i++)
							{
								if(i != g_nKeyNumber)//当前"游戏选项"Alpha通道不变
								{
									*(pfVertexX + i) -= 0.05f;//Alpha通道值减速0.1f,持续10帧
									if(*(pfVertexX + i) < 0.0f)
									{
										*(pfVertexX + i) = 0.0f;
									}
								}
								else
								{
									if(nKeySelectFlashTimesCount >= 13)
									{
										*(pfVertexX + i) -= 0.075f;//Alpha通道值减速0.1f,持续10帧
										if(*(pfVertexX + i) < 0.0f)
										{
											*(pfVertexX + i) = 0.0f;
										}
									}
								}
							}

							if(nKeySelectFlashTimesCount >= 13)//禁用渲染
							{
								bKeySelectFlashState = false;
							}

							if(nKeySelectFlashTimesCount >= 20)
							{
								nKeySelectFlashTimesCount = 0;
								nKeySelectFlashCount = 0;
								bKeySelectFlashState = false;
								fKeyPressTime = 0.0f;
								bKeySelect = false;//按键未选中
								g_nKeySelectState = 1;

								if(g_nKeyNumber == 0)//GameStart(Key按键被按下)
								{

								}
								else if(g_nKeyNumber == 1)//ExtraStart(Key按键被按下)
								{

								}
								else if(g_nKeyNumber == 2)//PracticeStart(Key按键被按下)
								{

								}
								else if(g_nKeyNumber == 3)//Replay(Key按键被按下)
								{

								}
								else if(g_nKeyNumber == 4)//PlayerData(Key按键被按下)
								{

								}
								else if(g_nKeyNumber == 5)//MusicRoom(Key按键被按下)
								{

								}
								else if(g_nKeyNumber == 6)//Option(Key按键被按下)
								{
									//Title01选项初始化
									g_nKeyNumber1 = 0;

									//"游戏选项"坐标偏移(恢复初始状态)
									pfVertexX = &fVertexXArray[0];//"游戏选项"字符恢复初始状态
									for(int i = 0; i < 8; i++)
									{
										*(pfVertexX + i) = 540.0f + i * 16.0f;
									}

									//"游戏选项"Alpha透明度变化
									pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道恢复初始状态
									for(int i = 0; i < 8; i++)
									{
										*(pfVertexX + i) = 0.0f;
									}
								}
								else if(g_nKeyNumber == 7)//Quit
								{
									Sleep(500);//线程睡眠500ms
									DestroyWindow(hWnd);//销毁窗口
								}
							}


						}
					}

					g_bKeySelectFlashState = bKeySelectFlashState;
				}
			}
			else if(g_nKeySelectState == 1)//状态1:Tilte01界面(选择...具体按钮选项)
			{
				if(bKeySelect == false)//按键未选中(按键状态标志)
				{
					if(g_nKeyNumber == 0)//GameStart
					{
						//检测方向键"↑"状态
						if(g_pDirectInput->IsKeyDown(DIK_UP))//方向键"↑"
						{
							bKeySelect = true;//按键已选中

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//检测方向键"↓"状态
						if(g_pDirectInput->IsKeyDown(DIK_DOWN))//方向键"↓"
						{
							bKeySelect = true;//按键已选中

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//检测"Z"键状态
						if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"键确定
						{
							//bKeySelect = true;
							if(bKeySelectZ == false)//"Z"键释放
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound加载音效se_ok00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}
							bKeySelectZ = true;//"Z"键按下
						}
						else
						{
							bKeySelectZ = false;//"Z"键释放
						}

						//检测"X"键/"Esc"键状态
						if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"键/"Esc"键取消
						{
							if(bKeyCancelLast == false)//"Esc"退出标志未置位
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载音效se_cancel00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								//"游戏选项"坐标偏移(恢复初始状态)
								pfVertexX = &fVertexXArray[0];//"游戏选项"字符恢复初始状态
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
								}

								//"游戏选项"Alpha透明度变化
								pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道恢复初始状态
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 0.0f;
								}

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							bKeyCancelLast = true;//"Esc"退出标志置位
						}
						else
						{
							bKeyCancelLast = false;//"Esc"退出标志未置位
						}
					}
					else if(g_nKeyNumber == 1)//ExtraStart
					{
						//检测方向键"↑"状态
						if(g_pDirectInput->IsKeyDown(DIK_UP))//方向键"↑"
						{
							bKeySelect = true;//按键已选中

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//检测方向键"↓"状态
						if(g_pDirectInput->IsKeyDown(DIK_DOWN))//方向键"↓"
						{
							bKeySelect = true;//按键已选中

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//检测"Z"键状态
						if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"键确定
						{
							//bKeySelect = true;
							if(bKeySelectZ == false)//"Z"键释放
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound加载音效se_ok00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}
							bKeySelectZ = true;//"Z"键按下
						}
						else
						{
							bKeySelectZ = false;//"Z"键释放
						}

						//检测"X"键/"Esc"键状态
						if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"键/"Esc"键取消
						{
							if(bKeyCancelLast == false)//"Esc"退出标志未置位
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载音效se_cancel00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								//"游戏选项"坐标偏移(恢复初始状态)
								pfVertexX = &fVertexXArray[0];//"游戏选项"字符恢复初始状态
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
								}

								//"游戏选项"Alpha透明度变化
								pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道恢复初始状态
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 0.0f;
								}

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							bKeyCancelLast = true;//"Esc"退出标志置位
						}
						else
						{
							bKeyCancelLast = false;//"Esc"退出标志未置位
						}
					}
					else if(g_nKeyNumber == 2)//PracticeStart
					{
						//检测方向键"↑"状态
						if(g_pDirectInput->IsKeyDown(DIK_UP))//方向键"↑"
						{
							bKeySelect = true;//按键已选中

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//检测方向键"↓"状态
						if(g_pDirectInput->IsKeyDown(DIK_DOWN))//方向键"↓"
						{
							bKeySelect = true;//按键已选中

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//检测"Z"键状态
						if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"键确定
						{
							//bKeySelect = true;
							if(bKeySelectZ == false)//"Z"键释放
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound加载音效se_ok00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}
							bKeySelectZ = true;//"Z"键按下
						}
						else
						{
							bKeySelectZ = false;//"Z"键释放
						}

						//检测"X"键/"Esc"键状态
						if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"键/"Esc"键取消
						{
							if(bKeyCancelLast == false)//"Esc"退出标志未置位
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载音效se_cancel00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								//"游戏选项"坐标偏移(恢复初始状态)
								pfVertexX = &fVertexXArray[0];//"游戏选项"字符恢复初始状态
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
								}

								//"游戏选项"Alpha透明度变化
								pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道恢复初始状态
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 0.0f;
								}

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							bKeyCancelLast = true;//"Esc"退出标志置位
						}
						else
						{
							bKeyCancelLast = false;//"Esc"退出标志未置位
						}
					}
					else if(g_nKeyNumber == 3)//Replay
					{
						//检测方向键"↑"状态
						if(g_pDirectInput->IsKeyDown(DIK_UP))//方向键"↑"
						{
							bKeySelect = true;//按键已选中

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//检测方向键"↓"状态
						if(g_pDirectInput->IsKeyDown(DIK_DOWN))//方向键"↓"
						{
							bKeySelect = true;//按键已选中

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//检测"Z"键状态
						if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"键确定
						{
							//bKeySelect = true;
							if(bKeySelectZ == false)//"Z"键释放
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound加载音效se_ok00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}
							bKeySelectZ = true;//"Z"键按下
						}
						else
						{
							bKeySelectZ = false;//"Z"键释放
						}

						//检测"X"键/"Esc"键状态
						if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"键/"Esc"键取消
						{
							if(bKeyCancelLast == false)//"Esc"退出标志未置位
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载音效se_cancel00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								//"游戏选项"坐标偏移(恢复初始状态)
								pfVertexX = &fVertexXArray[0];//"游戏选项"字符恢复初始状态
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
								}

								//"游戏选项"Alpha透明度变化
								pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道恢复初始状态
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 0.0f;
								}

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							bKeyCancelLast = true;//"Esc"退出标志置位
						}
						else
						{
							bKeyCancelLast = false;//"Esc"退出标志未置位
						}
					}
					else if(g_nKeyNumber == 4)//PlayerData
					{
						//检测方向键"↑"状态
						if(g_pDirectInput->IsKeyDown(DIK_UP))//方向键"↑"
						{
							bKeySelect = true;//按键已选中

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//检测方向键"↓"状态
						if(g_pDirectInput->IsKeyDown(DIK_DOWN))//方向键"↓"
						{
							bKeySelect = true;//按键已选中

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//检测"Z"键状态
						if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"键确定
						{
							//bKeySelect = true;
							if(bKeySelectZ == false)//"Z"键释放
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound加载音效se_ok00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}
							bKeySelectZ = true;//"Z"键按下
						}
						else
						{
							bKeySelectZ = false;//"Z"键释放
						}

						//检测"X"键/"Esc"键状态
						if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"键/"Esc"键取消
						{
							if(bKeyCancelLast == false)//"Esc"退出标志未置位
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载音效se_cancel00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								//"游戏选项"坐标偏移(恢复初始状态)
								pfVertexX = &fVertexXArray[0];//"游戏选项"字符恢复初始状态
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
								}

								//"游戏选项"Alpha透明度变化
								pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道恢复初始状态
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 0.0f;
								}

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							bKeyCancelLast = true;//"Esc"退出标志置位
						}
						else
						{
							bKeyCancelLast = false;//"Esc"退出标志未置位
						}
					}
					else if(g_nKeyNumber == 5)//MusicRoom
					{
						//检测方向键"↑"状态
						if(g_pDirectInput->IsKeyDown(DIK_UP))//方向键"↑"
						{
							bKeySelect = true;//按键已选中

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//检测方向键"↓"状态
						if(g_pDirectInput->IsKeyDown(DIK_DOWN))//方向键"↓"
						{
							bKeySelect = true;//按键已选中

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//检测"Z"键状态
						if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"键确定
						{
							//bKeySelect = true;
							if(bKeySelectZ == false)//"Z"键释放
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound加载音效se_ok00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}
							bKeySelectZ = true;//"Z"键按下
						}
						else
						{
							bKeySelectZ = false;//"Z"键释放
						}

						//检测"X"键/"Esc"键状态
						if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"键/"Esc"键取消
						{
							if(bKeyCancelLast == false)//"Esc"退出标志未置位
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载音效se_cancel00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								//"游戏选项"坐标偏移(恢复初始状态)
								pfVertexX = &fVertexXArray[0];//"游戏选项"字符恢复初始状态
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
								}

								//"游戏选项"Alpha透明度变化
								pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道恢复初始状态
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 0.0f;
								}

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							bKeyCancelLast = true;//"Esc"退出标志置位
						}
						else
						{
							bKeyCancelLast = false;//"Esc"退出标志未置位
						}
					}
					else if(g_nKeyNumber == 6)//Option
					{
						//Titel01切换动画时禁止按键
						bKeyEnableFlag = true;
						pfVertexX = &fVertexXArray[0];
						for(int i = 0; i < 8; i++)
						{
							if(*(pfVertexX + i) != 444.0f)//如果有X坐标不等于440.0f,禁止按键
							{
								bKeyEnableFlag = false;
								break;
							}
						}

						if(bKeyEnableFlag == true)//按键使能标志才接收按键信息
						{
							//检测方向键"↑"状态
							if(g_pDirectInput->IsKeyDown(DIK_UP))//方向键"↑"
							{
								bKeySelect = true;//按键已选中
								g_nKeyNumber1--;
								if(g_nKeyNumber1 < 0)
								{
									g_nKeyNumber1 = 4;
								}

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							//检测方向键"↓"状态
							if(g_pDirectInput->IsKeyDown(DIK_DOWN))//方向键"↓"
							{
								bKeySelect = true;//按键已选中
								g_nKeyNumber1++;
								if(g_nKeyNumber1 > 4)
								{
									g_nKeyNumber1 = 0;
								}

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound加载音效se_select00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							//检测"Z"键状态
							if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"键确定
							{
								//bKeySelect = true;
								if(bKeySelectZ == false)//"Z"键释放
								{
									bKeySelect = true;

									g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound加载音效se_ok00.wav
									g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

									bKeyMoveFlag = false;
									bKeyMoveDirect = false;
									fKeyMoveX = 0.0f;
								}

								bKeySelectZ = true;//"Z"键按下
							}
							else
							{
								bKeySelectZ = false;//"Z"键释放
							}

							//检测"X"键/"Esc"键状态
							if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"键/"Esc"键取消
							{
								if(bKeyCancelLast == false)//"Esc"退出标志未置位
								{
									if(bKeySelectX == false)//'X'未按下检测按键
									{
										bKeySelect = true;

										g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载音效se_cancel00.wav
										g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound播放音效(Once)

										if(g_nKeyNumber1 != 4)
										{
											g_nKeyNumber1 = 4;
										}
										else
										{
											bKeyCancelLast = true;
										}

										bKeyMoveFlag = false;
										bKeyMoveDirect = false;
										fKeyMoveX = 0.0f;
									}
								}

								bKeySelectX = true;//'X'键按下
							}
							else
							{
								bKeyCancelLast = false;//"Esc"退出标志未置位
								bKeySelectX = false;//'X'键未按下
							}
						}

						//Title01
						//"游戏选项"坐标偏移
						pfVertexX = &fVertexXArray[0];//"游戏选项"字符偏移位置向左移动(8x/frame)
						for(int i = 0; i < 8; i++)
						{
							*(pfVertexX + i) += -8;
							if(*(pfVertexX + i) < 444.0f)//"游戏选项"坐标限制(X:476.0f)
							{
								*(pfVertexX + i) = 444.0f;
							}
						}

						//"游戏选项"Alpha透明度变化
						pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道由0.0f,增速1.0f,持续10帧
						for(int i = 0; i < 8; i++)
						{
							if(*(pfVertexX + i) == 0.0f)//"游戏选项"字符只有第一个Alpha为0.0f增加,其后字符Alpha不增加(break)
							{
								*(pfVertexX + i) += 0.1f;
								break;
							}
							else
							{
								*(pfVertexX + i) += 0.1f;//其余字符Alpha值不为0.0f的增速0.1f
								if(*(pfVertexX + i) > 1.0f)//Alpha限幅1.0f
								{
									*(pfVertexX + i) = 1.0f;
								}
							}
						}

						//"游戏选项"Alpha通道变化
						if(bAlphaFlag == true)//Alpha值递增,增速0.025f,持续40帧
						{
							fAlphaValue += 0.025f;
							if(fAlphaValue > 1.0f)//Alpha值限幅1.0f
							{
								fAlphaValue = 1.0f;
								bAlphaFlag = false;
							}
						}
						else//Alpha值递减,减速0.025f,持续40帧
						{
							fAlphaValue += -0.025f;
							if(fAlphaValue < 0.5f)//Alpha值限幅0.5f
							{
								fAlphaValue = 0.5f;
								bAlphaFlag = true;
							}
						}

					}

				}
				else//按键已选中(按键状态标志)
				{
					//"X"键状态,"Z"键状态
					if(bKeyCancelLast == false && bKeySelectZ == false)//"X"键释放状态,"Z"键释放状态
					{
						fKeyPressTime += fDeltaTime;//计算叠加时间间隔
						if(fKeyPressTime > 0.12f)//按键叠加时间0.12s(KeyBoard消抖0.12s)
						{
							fKeyPressTime = 0.0f;
							bKeySelect = false;//按键未选中
						}

						if(bKeyMoveFlag == false)//按键移动标志(Left2->Right4->Left2)
						{
							if(bKeyMoveDirect == false)//按键移动方向(false:Left/true:Right)
							{
								fKeyMoveX += 2.0f;//按键移动X偏移增量2.0f
								if(fKeyMoveX >= 4.0f)//Left:2帧
								{
									fKeyMoveX = 4.0f;
									bKeyMoveDirect = true;//Right
								}
								if(fKeyMoveX == 0.0f)//Left:2帧
								{
									bKeyMoveFlag = true;
								}
							}
							else//Right:4帧
							{
								fKeyMoveX -= 2.0f;
								if(fKeyMoveX <= -4.0f)
								{
									fKeyMoveX = -4.0f;
									bKeyMoveDirect = false;
								}
							}
						}
						else
						{
							fKeyMoveX = 0.0f;//按键偏移位置原点
						}
					}
					else if(bKeyCancelLast == true && bKeySelectZ == false)//"X"键按下状态,"Z"键释放状态
					{
						if(g_bKeySelectTitleChange == false)
						{
							if(bKeySelectFlashState == false)
							{
								nKeySelectFlashCount++;
								if(nKeySelectFlashCount >= 2)
								{
									nKeySelectFlashCount = 0;
									bKeySelectFlashState = true;
									nKeySelectFlashTimesCount++;
								}
							}
							else
							{
								nKeySelectFlashCount++;
								if(nKeySelectFlashCount >= 2)
								{
									nKeySelectFlashCount = 0;
									bKeySelectFlashState = false;
									nKeySelectFlashTimesCount++;
								}
							}

							if(nKeySelectFlashTimesCount >= 9)
							{
								//"游戏选项"坐标偏移
								pfVertexX = &fVertexXArray[0];//"游戏选项"字符偏移位置向右移动(8x/frame)
								for(int i = 0; i < 8; i++)
								{
									if(i != g_nKeyNumber)//当前"游戏选项"字符偏移不变
									{
										*(pfVertexX + i) += 4;
										if(*(pfVertexX + i) > 640.0f)//"游戏选项"坐标限制(X:640.0f)
										{
											*(pfVertexX + i) = 640.0f;
										}
									}
									else
									{
										if(nKeySelectFlashTimesCount >= 13)
										{
											*(pfVertexX + i) += 8;
											if(*(pfVertexX + i) > 640.0f)//"游戏选项"坐标限制(X:640.0f)
											{
												*(pfVertexX + i) = 640.0f;
											}
										}
									}
								}

								//"游戏选项"Alpha透明度变化
								pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道由0.0f,增速1.0f,持续10帧(fAlphaValueArray数组此时值均为1.0f)
								for(int i = 0; i < 8; i++)
								{
									if(i != g_nKeyNumber)//当前"游戏选项"Alpha通道不变
									{
										*(pfVertexX + i) -= 0.05f;//Alpha通道值减速0.1f,持续10帧
										if(*(pfVertexX + i) < 0.0f)
										{
											*(pfVertexX + i) = 0.0f;
										}
									}
									else
									{
										if(nKeySelectFlashTimesCount >= 13)
										{
											*(pfVertexX + i) -= 0.075f;//Alpha通道值减速0.1f,持续10帧
											if(*(pfVertexX + i) < 0.0f)
											{
												*(pfVertexX + i) = 0.0f;
											}
										}
									}
								}

								if(nKeySelectFlashTimesCount >= 13)//禁用渲染
								{
									bKeySelectFlashState = false;
								}

								if(nKeySelectFlashTimesCount >= 20)
								{
									nKeySelectFlashTimesCount = 0;
									nKeySelectFlashCount = 0;
									bKeySelectFlashState = false;
									fKeyPressTime = 0.0f;
									g_bKeySelectTitleChange = true;

									//"游戏选项"坐标偏移(恢复初始状态)
									pfVertexX = &fVertexXArray[0];//"游戏选项"字符恢复初始状态
									for(int i = 0; i < 8; i++)
									{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
									}

									//"游戏选项"Alpha透明度变化
									pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道恢复初始状态
									for(int i = 0; i < 8; i++)
									{
									*(pfVertexX + i) = 0.0f;
									}
								}
							}

							g_bKeySelectFlashState = bKeySelectFlashState;
						}
						else
						{
							//"游戏选项"坐标偏移
							pfVertexX = &fVertexXArray[0];//"游戏选项"字符偏移位置向左移动(8x/frame)
							for(int i = 0; i < 8; i++)
							{
								*(pfVertexX + i) += -8;
								if(*(pfVertexX + i) < 476.0f)//"游戏选项"坐标限制(X:476.0f)
								{
									*(pfVertexX + i) = 476.0f;
								}
							}

							//"游戏选项"Alpha透明度变化
							pfVertexX = &fAlphaValueArray[0];//"游戏选项"Alpha通道由0.0f,增速1.0f,持续10帧
							for(int i = 0; i < 8; i++)
							{
								if(*(pfVertexX + i) == 0.0f)//"游戏选项"字符只有第一个Alpha为0.0f增加,其后字符Alpha不增加(break)
								{
									*(pfVertexX + i) += 0.0625f;//0.1f
									break;
								}
								else
								{
									*(pfVertexX + i) += 0.0625f;//其余字符Alpha值不为0.0f的增速0.1f
									if(*(pfVertexX + i) > 1.0f)//Alpha限幅1.0f
									{
										*(pfVertexX + i) = 1.0f;
									}
								}
							}

							nKeySelectFlashTimesCount++;
							if(nKeySelectFlashTimesCount >= 22)
							{
							nKeySelectFlashTimesCount = 0;
							nKeySelectFlashCount = 0;
							bKeySelectFlashState = false;
							fKeyPressTime = 0.0f;
							bKeySelect = false;//按键未选中
							g_bKeySelectTitleChange = false;
							g_nKeySelectState = 0;
							}
						}
						
					}
					else if(bKeyCancelLast == false && bKeySelectZ == true)//"X"键释放状态,"Z"键按下状态
					{
						fKeyPressTime += fDeltaTime;//计算叠加时间间隔
						if(fKeyPressTime > 0.12f)//按键叠加时间0.12s(KeyBoard消抖0.12s)
						{
							fKeyPressTime = 0.0f;
							bKeySelect = false;//按键未选中
						}
					}
					else
					{
						fKeyPressTime += fDeltaTime;//计算叠加时间间隔
						if(fKeyPressTime > 0.12f)//按键叠加时间0.12s(KeyBoard消抖0.12s)
						{
							fKeyPressTime = 0.0f;
							bKeySelect = false;//按键未选中
							bKeyCancelLast = false;
							bKeySelectZ = false;
						}
					}

				}
			}
		}

		//设置顶点缓存数据
		Vertex*pVertices = NULL;
		g_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

		if(g_nKeySelectState == 0)//Title00,State0
		{
			//Title00背景纹理顶点数组
			pVertices[0] = Vertex(0.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, 0.0f);
			pVertices[1] = Vertex(640.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), 0.0f);
			pVertices[2] = Vertex(640.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), (480.0f / 512.0f));
			pVertices[3] = Vertex(0.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, (480.0f / 512.0f));

			//"Copyright"纹理顶点数组
			pVertices[4] = Vertex(22.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), 0.0f);
			pVertices[5] = Vertex(227.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), 0.0f);
			pVertices[6] = Vertex(227.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), (16.0f / 256.0f));
			pVertices[7] = Vertex(22.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), (16.0f / 256.0f));

			//"Ver1.00a"纹理顶点数组
			pVertices[8] = Vertex(304.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 0.0f);
			pVertices[9] = Vertex(368.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 0.0f);
			pVertices[10] = Vertex(368.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 1.0f);
			pVertices[11] = Vertex(304.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 1.0f);

			//"GameStart"纹理顶点数组
			if(g_nKeyNumber == 0)
			{
				pVertices[12] = Vertex((fVertexXArray[0] + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), 0.0f);
				pVertices[13] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (256.0f / 512.0f), 0.0f);
				pVertices[14] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (256.0f / 512.0f), (32.0f / 512.0f));
				pVertices[15] = Vertex((fVertexXArray[0] + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (32.0f / 512.0f));
			}
			else
			{
				pVertices[12] = Vertex(fVertexXArray[0], 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), 0.0f);
				pVertices[13] = Vertex((fVertexXArray[0] + 128), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (256.0f / 512.0f), 0.0f);
				pVertices[14] = Vertex((fVertexXArray[0] + 128), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (256.0f / 512.0f), (32.0f / 512.0f));
				pVertices[15] = Vertex(fVertexXArray[0], 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (32.0f / 512.0f));
			}

			//"ExtraStart"纹理顶点数组
			if(g_nKeyNumber == 1)
			{
				pVertices[16] = Vertex((fVertexXArray[1] + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (32.0f / 512.0f));
				pVertices[17] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (256.0f / 512.0f), (32.0f / 512.0f));
				pVertices[18] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (256.0f / 512.0f), (64.0f / 512.0f));
				pVertices[19] = Vertex((fVertexXArray[1] + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (64.0f / 512.0f));
			}
			else
			{
				pVertices[16] = Vertex(fVertexXArray[1], 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (32.0f / 512.0f));
				pVertices[17] = Vertex((fVertexXArray[1] + 128), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (256.0f / 512.0f), (32.0f / 512.0f));
				pVertices[18] = Vertex((fVertexXArray[1] + 128), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (256.0f / 512.0f), (64.0f / 512.0f));
				pVertices[19] = Vertex(fVertexXArray[1], 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (64.0f / 512.0f));
			}

			//"PracticeStart"纹理顶点数组
			if(g_nKeyNumber == 2)
			{
				pVertices[20] = Vertex((fVertexXArray[2] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (64.0f / 512.0f));
				pVertices[21] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (256.0f / 512.0f), (64.0f / 512.0f));
				pVertices[22] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (256.0f / 512.0f), (96.0f / 512.0f));
				pVertices[23] = Vertex((fVertexXArray[2] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (96.0f / 512.0f));
			}
			else
			{
				pVertices[20] = Vertex(fVertexXArray[2], 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (64.0f / 512.0f));
				pVertices[21] = Vertex((fVertexXArray[2] + 128), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (256.0f / 512.0f), (64.0f / 512.0f));
				pVertices[22] = Vertex((fVertexXArray[2] + 128), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (256.0f / 512.0f), (96.0f / 512.0f));
				pVertices[23] = Vertex(fVertexXArray[2], 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (96.0f / 512.0f));
			}

			//"Replay"纹理顶点数组
			if(g_nKeyNumber == 3)
			{
				pVertices[24] = Vertex((fVertexXArray[3] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (96.0f / 512.0f));
				pVertices[25] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (256.0f / 512.0f), (96.0f / 512.0f));
				pVertices[26] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (256.0f / 512.0f), (128.0f / 512.0f));
				pVertices[27] = Vertex((fVertexXArray[3] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (128.0f / 512.0f));
			}
			else
			{
				pVertices[24] = Vertex(fVertexXArray[3], 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (96.0f / 512.0f));
				pVertices[25] = Vertex((fVertexXArray[3] + 128), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (256.0f / 512.0f), (96.0f / 512.0f));
				pVertices[26] = Vertex((fVertexXArray[3] + 128), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (256.0f / 512.0f), (128.0f / 512.0f));
				pVertices[27] = Vertex(fVertexXArray[3], 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (128.0f / 512.0f));
			}

			//"PlayerData"纹理顶点数组
			if(g_nKeyNumber == 4)
			{
				pVertices[28] = Vertex((fVertexXArray[4] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (128.0f / 512.0f));
				pVertices[29] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (256.0f / 512.0f), (128.0f / 512.0f));
				pVertices[30] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (256.0f / 512.0f), (160.0f / 512.0f));
				pVertices[31] = Vertex((fVertexXArray[4] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (160.0f / 512.0f));
			}
			else
			{
				pVertices[28] = Vertex(fVertexXArray[4], 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (128.0f / 512.0f));
				pVertices[29] = Vertex((fVertexXArray[4] + 128), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (256.0f / 512.0f), (128.0f / 512.0f));
				pVertices[30] = Vertex((fVertexXArray[4] + 128), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (256.0f / 512.0f), (160.0f / 512.0f));
				pVertices[31] = Vertex(fVertexXArray[4], 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (160.0f / 512.0f));
			}

			//"MusicRoom"纹理顶点数组
			if(g_nKeyNumber == 5)
			{
				pVertices[32] = Vertex((fVertexXArray[5] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (160.0f / 512.0f));
				pVertices[33] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (256.0f / 512.0f), (160.0f / 512.0f));
				pVertices[34] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (256.0f / 512.0f), (192.0f / 512.0f));
				pVertices[35] = Vertex((fVertexXArray[5] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (192.0f / 512.0f));
			}
			else
			{
				pVertices[32] = Vertex(fVertexXArray[5], 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (160.0f / 512.0f));
				pVertices[33] = Vertex((fVertexXArray[5] + 128), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (256.0f / 512.0f), (160.0f / 512.0f));
				pVertices[34] = Vertex((fVertexXArray[5] + 128), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (256.0f / 512.0f), (192.0f / 512.0f));
				pVertices[35] = Vertex(fVertexXArray[5], 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (192.0f / 512.0f));
			}

			//"Option"纹理顶点数组
			if(g_nKeyNumber == 6)
			{
				pVertices[36] = Vertex((fVertexXArray[6] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (192.0f / 512.0f));
				pVertices[37] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (256.0f / 512.0f), (192.0f / 512.0f));
				pVertices[38] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (256.0f / 512.0f), (224.0f / 512.0f));
				pVertices[39] = Vertex((fVertexXArray[6] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (224.0f / 512.0f));
			}
			else
			{
				pVertices[36] = Vertex(fVertexXArray[6], 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (192.0f / 512.0f));
				pVertices[37] = Vertex((fVertexXArray[6] + 128), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (256.0f / 512.0f), (192.0f / 512.0f));
				pVertices[38] = Vertex((fVertexXArray[6] + 128), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (256.0f / 512.0f), (224.0f / 512.0f));
				pVertices[39] = Vertex(fVertexXArray[6], 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (224.0f / 512.0f));
			}

			//"Quit"纹理顶点数组
			if(g_nKeyNumber == 7)
			{
				pVertices[40] = Vertex((fVertexXArray[7] + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (128.0f / 512.0f), (224.0f / 512.0f));
				pVertices[41] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (256.0f / 512.0f), (224.0f / 512.0f));
				pVertices[42] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (256.0f / 512.0f), (256.0f / 512.0f));
				pVertices[43] = Vertex((fVertexXArray[7] + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (128.0f / 512.0f), (256.0f / 512.0f));
			}
			else
			{
				pVertices[40] = Vertex(fVertexXArray[7], 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (128.0f / 512.0f), (224.0f / 512.0f));
				pVertices[41] = Vertex((fVertexXArray[7] + 128), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (256.0f / 512.0f), (224.0f / 512.0f));
				pVertices[42] = Vertex((fVertexXArray[7] + 128), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (256.0f / 512.0f), (256.0f / 512.0f));
				pVertices[43] = Vertex(fVertexXArray[7], 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (128.0f / 512.0f), (256.0f / 512.0f));
			}

			if(bKeySelectZ == false)//"Z"键释放状态
			{
				if(g_nKeyNumber == 0)//"GameStart"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[0] + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), 0.0f, 0.0f);
					pVertices[45] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), (128.0f / 512.0f), 0.0f);
					pVertices[46] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), (128.0f / 512.0f), (32.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[0] + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), 0.0f, (32.0f / 512.0f));
				}
				else if(g_nKeyNumber == 1)//"ExtraStart"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[1] + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), 0.0f, (32.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), (128.0f / 512.0f), (32.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), (128.0f / 512.0f), (64.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[1] + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), 0.0f, (64.0f / 512.0f));
				}
				else if(g_nKeyNumber == 2)//"PracticeStart"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[2] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), 0.0f, (64.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), (128.0f / 512.0f), (64.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), (128.0f / 512.0f), (96.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[2] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), 0.0f, (96.0f / 512.0f));
				}
				else if(g_nKeyNumber == 3)//"Replay"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[3] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), 0.0f, (96.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), (128.0f / 512.0f), (96.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), (128.0f / 512.0f), (128.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[3] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), 0.0f, (128.0f / 512.0f));
				}
				else if(g_nKeyNumber == 4)//"PlayerData"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[4] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), 0.0f, (128.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), (128.0f / 512.0f), (128.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), (128.0f / 512.0f), (160.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[4] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), 0.0f, (160.0f / 512.0f));
				}
				else if(g_nKeyNumber == 5)//"MusicRoom"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[5] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), 0.0f, (160.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), (128.0f / 512.0f), (160.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), (128.0f / 512.0f), (192.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[5] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), 0.0f, (192.0f / 512.0f));
				}
				else if(g_nKeyNumber == 6)//"Option"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[6] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), 0.0f, (192.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), (128.0f / 512.0f), (192.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), (128.0f / 512.0f), (224.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[6] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), 0.0f, (224.0f / 512.0f));
				}
				else if(g_nKeyNumber == 7)//"Quit"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[7] + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), 0.0f, (224.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), (128.0f / 512.0f), (224.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), (128.0f / 512.0f), (256.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[7] + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), 0.0f, (256.0f / 512.0f));
				}
			}
			else//"Z"键按下
			{
				if(g_nKeyNumber == 0)//"GameStart"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[0] + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), 0.0f, 0.0f);
					pVertices[45] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), 0.0f);
					pVertices[46] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (32.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[0] + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), 0.0f, (32.0f / 512.0f));
				}
				else if(g_nKeyNumber == 1)//"ExtraStart"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[1] + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), 0.0f, (32.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (32.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (64.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[1] + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), 0.0f, (64.0f / 512.0f));
				}
				else if(g_nKeyNumber == 2)//"PracticeStart"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[2] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), 0.0f, (64.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (64.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (96.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[2] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), 0.0f, (96.0f / 512.0f));
				}
				else if(g_nKeyNumber == 3)//"Replay"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[3] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), 0.0f, (96.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (96.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (128.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[3] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), 0.0f, (128.0f / 512.0f));
				}
				else if(g_nKeyNumber == 4)//"PlayerData"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[4] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), 0.0f, (128.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (128.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (160.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[4] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), 0.0f, (160.0f / 512.0f));
				}
				else if(g_nKeyNumber == 5)//"MusicRoom"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[5] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), 0.0f, (160.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (160.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (192.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[5] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), 0.0f, (192.0f / 512.0f));
				}
				else if(g_nKeyNumber == 6)//"Option"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[6] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), 0.0f, (192.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (192.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (224.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[6] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), 0.0f, (224.0f / 512.0f));
				}
				else if(g_nKeyNumber == 7)//"Quit"选中纹理顶点数组
				{
					pVertices[44] = Vertex((fVertexXArray[7] + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), 0.0f, (224.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (128.0f / 512.0f), (224.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (128.0f / 512.0f), (256.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[7] + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), 0.0f, (256.0f / 512.0f));
				}
			}
		}
		else if(g_nKeySelectState == 1)
		{
			if(g_nKeyNumber == 0)//GameStart
			{

			}
			else if(g_nKeyNumber == 1)//ExtraStart
			{

			}
			else if(g_nKeyNumber == 2)//PracticeStart
			{

			}
			else if(g_nKeyNumber == 3)//Replay
			{

			}
			else if(g_nKeyNumber == 4)//PlayerData
			{

			}
			else if(g_nKeyNumber == 5)//MusicRoom
			{

			}
			else if(g_nKeyNumber == 6)//Option
			{
				//Title00背景纹理顶点数组
				pVertices[0] = Vertex(0.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, 0.0f);
				pVertices[1] = Vertex(640.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), 0.0f);
				pVertices[2] = Vertex(640.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), (480.0f / 512.0f));
				pVertices[3] = Vertex(0.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, (480.0f / 512.0f));

				//"Copyright"纹理顶点数组
				pVertices[4] = Vertex(22.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), 0.0f);
				pVertices[5] = Vertex(227.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), 0.0f);
				pVertices[6] = Vertex(227.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), (16.0f / 256.0f));
				pVertices[7] = Vertex(22.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), (16.0f / 256.0f));

				//"Ver1.00a"纹理顶点数组
				pVertices[8] = Vertex(304.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 0.0f);
				pVertices[9] = Vertex(368.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 0.0f);
				pVertices[10] = Vertex(368.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 1.0f);
				pVertices[11] = Vertex(304.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 1.0f);

				if(g_bKeySelectTitleChange == false)
				{
					//"BGM Volume"纹理顶点数组
					if(g_nKeyNumber1 == 0)
					{
						pVertices[12] = Vertex((fVertexXArray[0] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (256.0f / 512.0f));
						pVertices[13] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (256.0f / 512.0f), (256.0f / 512.0f));
						pVertices[14] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (256.0f / 512.0f), (288.0f / 512.0f));
						pVertices[15] = Vertex((fVertexXArray[0] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (288.0f / 512.0f));
					}
					else
					{
						pVertices[12] = Vertex(fVertexXArray[0], 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (256.0f / 512.0f));
						pVertices[13] = Vertex((fVertexXArray[0] + 128), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (256.0f / 512.0f), (256.0f / 512.0f));
						pVertices[14] = Vertex((fVertexXArray[0] + 128), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (256.0f / 512.0f), (288.0f / 512.0f));
						pVertices[15] = Vertex(fVertexXArray[0], 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (288.0f / 512.0f));
					}

					//"SE Volume"纹理顶点数组
					if(g_nKeyNumber1 == 1)
					{
						pVertices[16] = Vertex((fVertexXArray[1] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (288.0f / 512.0f));
						pVertices[17] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (256.0f / 512.0f), (288.0f / 512.0f));
						pVertices[18] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (256.0f / 512.0f), (320.0f / 512.0f));
						pVertices[19] = Vertex((fVertexXArray[1] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (320.0f / 512.0f));
					}
					else
					{
						pVertices[16] = Vertex(fVertexXArray[1], 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (288.0f / 512.0f));
						pVertices[17] = Vertex((fVertexXArray[1] + 128), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (256.0f / 512.0f), (288.0f / 512.0f));
						pVertices[18] = Vertex((fVertexXArray[1] + 128), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (256.0f / 512.0f), (320.0f / 512.0f));
						pVertices[19] = Vertex(fVertexXArray[1], 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (320.0f / 512.0f));
					}

					//"Key Config"纹理顶点数组
					if(g_nKeyNumber1 == 2)
					{
						pVertices[20] = Vertex((fVertexXArray[2] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (352.0f / 512.0f));
						pVertices[21] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (256.0f / 512.0f), (352.0f / 512.0f));
						pVertices[22] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (256.0f / 512.0f), (384.0f / 512.0f));
						pVertices[23] = Vertex((fVertexXArray[2] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (384.0f / 512.0f));
					}
					else
					{
						pVertices[20] = Vertex(fVertexXArray[2], 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (352.0f / 512.0f));
						pVertices[21] = Vertex((fVertexXArray[2] + 128), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (256.0f / 512.0f), (352.0f / 512.0f));
						pVertices[22] = Vertex((fVertexXArray[2] + 128), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (256.0f / 512.0f), (384.0f / 512.0f));
						pVertices[23] = Vertex(fVertexXArray[2], 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (384.0f / 512.0f));
					}

					//"Default"纹理顶点数组
					if(g_nKeyNumber1 == 3)
					{
						pVertices[24] = Vertex((fVertexXArray[3] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (320.0f / 512.0f));
						pVertices[25] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (256.0f / 512.0f), (320.0f / 512.0f));
						pVertices[26] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (256.0f / 512.0f), (352.0f / 512.0f));
						pVertices[27] = Vertex((fVertexXArray[3] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (352.0f / 512.0f));
					}
					else
					{
						pVertices[24] = Vertex(fVertexXArray[3], 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (320.0f / 512.0f));
						pVertices[25] = Vertex((fVertexXArray[3] + 128), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (256.0f / 512.0f), (320.0f / 512.0f));
						pVertices[26] = Vertex((fVertexXArray[3] + 128), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (256.0f / 512.0f), (352.0f / 512.0f));
						pVertices[27] = Vertex(fVertexXArray[3], 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (352.0f / 512.0f));
					}

					//"Quit"纹理顶点数组
					if(g_nKeyNumber1 == 4)
					{
						pVertices[28] = Vertex((fVertexXArray[4] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (224.0f / 512.0f));
						pVertices[29] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (256.0f / 512.0f), (224.0f / 512.0f));
						pVertices[30] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (256.0f / 512.0f), (256.0f / 512.0f));
						pVertices[31] = Vertex((fVertexXArray[4] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (256.0f / 512.0f));
					}
					else
					{
						pVertices[28] = Vertex(fVertexXArray[4], 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (224.0f / 512.0f));
						pVertices[29] = Vertex((fVertexXArray[4] + 128), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (256.0f / 512.0f), (224.0f / 512.0f));
						pVertices[30] = Vertex((fVertexXArray[4] + 128), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (256.0f / 512.0f), (256.0f / 512.0f));
						pVertices[31] = Vertex(fVertexXArray[4], 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (256.0f / 512.0f));
					}

					if(bKeyCancelLast == false)
					{
						if(g_nKeyNumber1 == 0)//"BGM Volume"选中纹理顶点数组
						{
							pVertices[32] = Vertex((fVertexXArray[0] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), 0.0f, (256.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), (128.0f / 512.0f), (256.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), (128.0f / 512.0f), (288.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[0] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), 0.0f, (288.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 1)//"SE Volume"选中纹理顶点数组
						{
							pVertices[32] = Vertex((fVertexXArray[1] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), 0.0f, (288.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), (128.0f / 512.0f), (288.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), (128.0f / 512.0f), (320.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[1] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), 0.0f, (320.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 2)//"Key Config"选中纹理顶点数组
						{
							pVertices[32] = Vertex((fVertexXArray[2] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), 0.0f, (352.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), (128.0f / 512.0f), (352.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), (128.0f / 512.0f), (384.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[2] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), 0.0f, (384.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 3)//"Default"选中纹理顶点数组
						{
							pVertices[32] = Vertex((fVertexXArray[3] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), 0.0f, (320.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), (128.0f / 512.0f), (320.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), (128.0f / 512.0f), (352.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[3] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), 0.0f, (352.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 4)//"Quit"选中纹理顶点数组
						{
							pVertices[32] = Vertex((fVertexXArray[4] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), 0.0f, (224.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), (128.0f / 512.0f), (224.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), (128.0f / 512.0f), (256.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[4] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), 0.0f, (256.0f / 512.0f));
						}
					}
					else
					{
						if(g_nKeyNumber1 == 0)//"BGM Volume"选中纹理顶点数组
						{
							pVertices[32] = Vertex((fVertexXArray[0] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), 0.0f, (256.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (256.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (288.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[0] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), 0.0f, (288.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 1)//"SE Volume"选中纹理顶点数组
						{
							pVertices[32] = Vertex((fVertexXArray[1] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), 0.0f, (288.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (288.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (320.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[1] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), 0.0f, (320.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 2)//"Key Config"选中纹理顶点数组
						{
							pVertices[32] = Vertex((fVertexXArray[2] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), 0.0f, (352.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (352.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (384.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[2] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), 0.0f, (384.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 3)//"Default"选中纹理顶点数组
						{
							pVertices[32] = Vertex((fVertexXArray[3] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), 0.0f, (320.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (320.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (352.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[3] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), 0.0f, (352.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 4)//"Quit"选中纹理顶点数组
						{
							pVertices[32] = Vertex((fVertexXArray[4] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), 0.0f, (224.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (224.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (256.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[4] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), 0.0f, (256.0f / 512.0f));
						}
					}
				}
				else
				{
					//"GameStart"纹理顶点数组
					if(g_nKeyNumber == 0)
					{
						pVertices[12] = Vertex((fVertexXArray[0] + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), 0.0f);
						pVertices[13] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (256.0f / 512.0f), 0.0f);
						pVertices[14] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (256.0f / 512.0f), (32.0f / 512.0f));
						pVertices[15] = Vertex((fVertexXArray[0] + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (32.0f / 512.0f));
					}
					else
					{
						pVertices[12] = Vertex(fVertexXArray[0], 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), 0.0f);
						pVertices[13] = Vertex((fVertexXArray[0] + 128), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (256.0f / 512.0f), 0.0f);
						pVertices[14] = Vertex((fVertexXArray[0] + 128), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (256.0f / 512.0f), (32.0f / 512.0f));
						pVertices[15] = Vertex(fVertexXArray[0], 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (32.0f / 512.0f));
					}

					//"ExtraStart"纹理顶点数组
					if(g_nKeyNumber == 1)
					{
						pVertices[16] = Vertex((fVertexXArray[1] + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (32.0f / 512.0f));
						pVertices[17] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (256.0f / 512.0f), (32.0f / 512.0f));
						pVertices[18] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (256.0f / 512.0f), (64.0f / 512.0f));
						pVertices[19] = Vertex((fVertexXArray[1] + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (64.0f / 512.0f));
					}
					else
					{
						pVertices[16] = Vertex(fVertexXArray[1], 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (32.0f / 512.0f));
						pVertices[17] = Vertex((fVertexXArray[1] + 128), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (256.0f / 512.0f), (32.0f / 512.0f));
						pVertices[18] = Vertex((fVertexXArray[1] + 128), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (256.0f / 512.0f), (64.0f / 512.0f));
						pVertices[19] = Vertex(fVertexXArray[1], 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (64.0f / 512.0f));
					}

					//"PracticeStart"纹理顶点数组
					if(g_nKeyNumber == 2)
					{
						pVertices[20] = Vertex((fVertexXArray[2] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (64.0f / 512.0f));
						pVertices[21] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (256.0f / 512.0f), (64.0f / 512.0f));
						pVertices[22] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (256.0f / 512.0f), (96.0f / 512.0f));
						pVertices[23] = Vertex((fVertexXArray[2] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (96.0f / 512.0f));
					}
					else
					{
						pVertices[20] = Vertex(fVertexXArray[2], 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (64.0f / 512.0f));
						pVertices[21] = Vertex((fVertexXArray[2] + 128), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (256.0f / 512.0f), (64.0f / 512.0f));
						pVertices[22] = Vertex((fVertexXArray[2] + 128), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (256.0f / 512.0f), (96.0f / 512.0f));
						pVertices[23] = Vertex(fVertexXArray[2], 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (96.0f / 512.0f));
					}

					//"Replay"纹理顶点数组
					if(g_nKeyNumber == 3)
					{
						pVertices[24] = Vertex((fVertexXArray[3] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (96.0f / 512.0f));
						pVertices[25] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (256.0f / 512.0f), (96.0f / 512.0f));
						pVertices[26] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (256.0f / 512.0f), (128.0f / 512.0f));
						pVertices[27] = Vertex((fVertexXArray[3] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (128.0f / 512.0f));
					}
					else
					{
						pVertices[24] = Vertex(fVertexXArray[3], 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (96.0f / 512.0f));
						pVertices[25] = Vertex((fVertexXArray[3] + 128), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (256.0f / 512.0f), (96.0f / 512.0f));
						pVertices[26] = Vertex((fVertexXArray[3] + 128), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (256.0f / 512.0f), (128.0f / 512.0f));
						pVertices[27] = Vertex(fVertexXArray[3], 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (128.0f / 512.0f));
					}

					//"PlayerData"纹理顶点数组
					if(g_nKeyNumber == 4)
					{
						pVertices[28] = Vertex((fVertexXArray[4] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (128.0f / 512.0f));
						pVertices[29] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (256.0f / 512.0f), (128.0f / 512.0f));
						pVertices[30] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (256.0f / 512.0f), (160.0f / 512.0f));
						pVertices[31] = Vertex((fVertexXArray[4] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (160.0f / 512.0f));
					}
					else
					{
						pVertices[28] = Vertex(fVertexXArray[4], 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (128.0f / 512.0f));
						pVertices[29] = Vertex((fVertexXArray[4] + 128), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (256.0f / 512.0f), (128.0f / 512.0f));
						pVertices[30] = Vertex((fVertexXArray[4] + 128), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (256.0f / 512.0f), (160.0f / 512.0f));
						pVertices[31] = Vertex(fVertexXArray[4], 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (160.0f / 512.0f));
					}

					//"MusicRoom"纹理顶点数组
					if(g_nKeyNumber == 5)
					{
						pVertices[32] = Vertex((fVertexXArray[5] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (160.0f / 512.0f));
						pVertices[33] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (256.0f / 512.0f), (160.0f / 512.0f));
						pVertices[34] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (256.0f / 512.0f), (192.0f / 512.0f));
						pVertices[35] = Vertex((fVertexXArray[5] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (192.0f / 512.0f));
					}
					else
					{
						pVertices[32] = Vertex(fVertexXArray[5], 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (160.0f / 512.0f));
						pVertices[33] = Vertex((fVertexXArray[5] + 128), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (256.0f / 512.0f), (160.0f / 512.0f));
						pVertices[34] = Vertex((fVertexXArray[5] + 128), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (256.0f / 512.0f), (192.0f / 512.0f));
						pVertices[35] = Vertex(fVertexXArray[5], 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (192.0f / 512.0f));
					}

					//"Option"纹理顶点数组
					if(g_nKeyNumber == 6)
					{
						pVertices[36] = Vertex((fVertexXArray[6] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (192.0f / 512.0f));
						pVertices[37] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (256.0f / 512.0f), (192.0f / 512.0f));
						pVertices[38] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (256.0f / 512.0f), (224.0f / 512.0f));
						pVertices[39] = Vertex((fVertexXArray[6] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (224.0f / 512.0f));
					}
					else
					{
						pVertices[36] = Vertex(fVertexXArray[6], 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (192.0f / 512.0f));
						pVertices[37] = Vertex((fVertexXArray[6] + 128), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (256.0f / 512.0f), (192.0f / 512.0f));
						pVertices[38] = Vertex((fVertexXArray[6] + 128), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (256.0f / 512.0f), (224.0f / 512.0f));
						pVertices[39] = Vertex(fVertexXArray[6], 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (224.0f / 512.0f));
					}

					//"Quit"纹理顶点数组
					if(g_nKeyNumber == 7)
					{
						pVertices[40] = Vertex((fVertexXArray[7] + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (128.0f / 512.0f), (224.0f / 512.0f));
						pVertices[41] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (256.0f / 512.0f), (224.0f / 512.0f));
						pVertices[42] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (256.0f / 512.0f), (256.0f / 512.0f));
						pVertices[43] = Vertex((fVertexXArray[7] + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (128.0f / 512.0f), (256.0f / 512.0f));
					}
					else
					{
						pVertices[40] = Vertex(fVertexXArray[7], 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (128.0f / 512.0f), (224.0f / 512.0f));
						pVertices[41] = Vertex((fVertexXArray[7] + 128), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (256.0f / 512.0f), (224.0f / 512.0f));
						pVertices[42] = Vertex((fVertexXArray[7] + 128), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (256.0f / 512.0f), (256.0f / 512.0f));
						pVertices[43] = Vertex(fVertexXArray[7], 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (128.0f / 512.0f), (256.0f / 512.0f));
					}

					if(bKeySelectZ == false)//"Z"键释放状态
					{
						if(g_nKeyNumber == 0)//"GameStart"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[0] + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), 0.0f, 0.0f);
							pVertices[45] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), (128.0f / 512.0f), 0.0f);
							pVertices[46] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), (128.0f / 512.0f), (32.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[0] + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), 0.0f, (32.0f / 512.0f));
						}
						else if(g_nKeyNumber == 1)//"ExtraStart"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[1] + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), 0.0f, (32.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), (128.0f / 512.0f), (32.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), (128.0f / 512.0f), (64.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[1] + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), 0.0f, (64.0f / 512.0f));
						}
						else if(g_nKeyNumber == 2)//"PracticeStart"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[2] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), 0.0f, (64.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), (128.0f / 512.0f), (64.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), (128.0f / 512.0f), (96.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[2] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), 0.0f, (96.0f / 512.0f));
						}
						else if(g_nKeyNumber == 3)//"Replay"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[3] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), 0.0f, (96.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), (128.0f / 512.0f), (96.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), (128.0f / 512.0f), (128.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[3] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), 0.0f, (128.0f / 512.0f));
						}
						else if(g_nKeyNumber == 4)//"PlayerData"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[4] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), 0.0f, (128.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), (128.0f / 512.0f), (128.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), (128.0f / 512.0f), (160.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[4] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), 0.0f, (160.0f / 512.0f));
						}
						else if(g_nKeyNumber == 5)//"MusicRoom"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[5] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), 0.0f, (160.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), (128.0f / 512.0f), (160.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), (128.0f / 512.0f), (192.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[5] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), 0.0f, (192.0f / 512.0f));
						}
						else if(g_nKeyNumber == 6)//"Option"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[6] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), 0.0f, (192.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), (128.0f / 512.0f), (192.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), (128.0f / 512.0f), (224.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[6] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), 0.0f, (224.0f / 512.0f));
						}
						else if(g_nKeyNumber == 7)//"Quit"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[7] + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), 0.0f, (224.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), (128.0f / 512.0f), (224.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), (128.0f / 512.0f), (256.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[7] + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), 0.0f, (256.0f / 512.0f));
						}
					}
					else//"Z"键按下
					{
						if(g_nKeyNumber == 0)//"GameStart"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[0] + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), 0.0f, 0.0f);
							pVertices[45] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), 0.0f);
							pVertices[46] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (32.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[0] + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), 0.0f, (32.0f / 512.0f));
						}
						else if(g_nKeyNumber == 1)//"ExtraStart"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[1] + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), 0.0f, (32.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (32.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (64.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[1] + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), 0.0f, (64.0f / 512.0f));
						}
						else if(g_nKeyNumber == 2)//"PracticeStart"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[2] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), 0.0f, (64.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (64.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (96.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[2] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), 0.0f, (96.0f / 512.0f));
						}
						else if(g_nKeyNumber == 3)//"Replay"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[3] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), 0.0f, (96.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (96.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (128.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[3] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), 0.0f, (128.0f / 512.0f));
						}
						else if(g_nKeyNumber == 4)//"PlayerData"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[4] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), 0.0f, (128.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (128.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (160.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[4] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), 0.0f, (160.0f / 512.0f));
						}
						else if(g_nKeyNumber == 5)//"MusicRoom"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[5] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), 0.0f, (160.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (160.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (192.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[5] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), 0.0f, (192.0f / 512.0f));
						}
						else if(g_nKeyNumber == 6)//"Option"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[6] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), 0.0f, (192.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (192.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (224.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[6] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), 0.0f, (224.0f / 512.0f));
						}
						else if(g_nKeyNumber == 7)//"Quit"选中纹理顶点数组
						{
							pVertices[44] = Vertex((fVertexXArray[7] + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), 0.0f, (224.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (128.0f / 512.0f), (224.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), (128.0f / 512.0f), (256.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[7] + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[7]), 0.0f, (256.0f / 512.0f));
						}
					}
				}

			}
		}

		g_pVertexBuffer->Unlock();

		//设置索引缓存数据
		WORD*pIndices = NULL;
		g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);

		//Title00背景纹理索引数组
		pIndices[0] = 0;
		pIndices[1] = 1;
		pIndices[2] = 2;

		pIndices[3] = 0;
		pIndices[4] = 2;
		pIndices[5] = 3;

		//"Copyright"纹理索引数组
		pIndices[6] = 4;
		pIndices[7] = 5;
		pIndices[8] = 6;

		pIndices[9] = 4;
		pIndices[10] = 6;
		pIndices[11] = 7;

		//"Ver1.00a"纹理索引数组
		pIndices[12] = 8;
		pIndices[13] = 9;
		pIndices[14] = 10;

		pIndices[15] = 8;
		pIndices[16] = 10;
		pIndices[17] = 11;

		//"GameStart"纹理索引数组
		pIndices[18] = 12;
		pIndices[19] = 13;
		pIndices[20] = 14;

		pIndices[21] = 12;
		pIndices[22] = 14;
		pIndices[23] = 15;

		//"ExtraStart"纹理索引数组
		pIndices[24] = 16;
		pIndices[25] = 17;
		pIndices[26] = 18;

		pIndices[27] = 16;
		pIndices[28] = 18;
		pIndices[29] = 19;

		//"PracticeStart"纹理索引数组
		pIndices[30] = 20;
		pIndices[31] = 21;
		pIndices[32] = 22;

		pIndices[33] = 20;
		pIndices[34] = 22;
		pIndices[35] = 23;

		//"Replay"纹理索引数组
		pIndices[36] = 24;
		pIndices[37] = 25;
		pIndices[38] = 26;

		pIndices[39] = 24;
		pIndices[40] = 26;
		pIndices[41] = 27;

		//"PlayerData"纹理索引数组
		pIndices[42] = 28;
		pIndices[43] = 29;
		pIndices[44] = 30;

		pIndices[45] = 28;
		pIndices[46] = 30;
		pIndices[47] = 31;

		//"MusicRoom"纹理索引数组
		pIndices[48] = 32;
		pIndices[49] = 33;
		pIndices[50] = 34;

		pIndices[51] = 32;
		pIndices[52] = 34;
		pIndices[53] = 35;

		//"Option"纹理索引数组
		pIndices[54] = 36;
		pIndices[55] = 37;
		pIndices[56] = 38;

		pIndices[57] = 36;
		pIndices[58] = 38;
		pIndices[59] = 39;

		//"Quit"纹理索引数组
		pIndices[60] = 40;
		pIndices[61] = 41;
		pIndices[62] = 42;

		pIndices[63] = 40;
		pIndices[64] = 42;
		pIndices[65] = 43;

		//游戏选项选中纹理索引数组
		pIndices[66] = 44;
		pIndices[67] = 45;
		pIndices[68] = 46;

		pIndices[69] = 44;
		pIndices[70] = 46;
		pIndices[71] = 47;

		g_pIndexBuffer->Unlock();*/
	}
}

/*
** Function:  Direct3D_Render(HWND hWnd, float fDeltaTime)
** Purpose:  Direct3D渲染
** Para: HWND hWnd(窗口句柄)
** Para: float fDeltaTime(时间间隔)
*/
void Direct3D_Render(HWND hWnd, float fDeltaTime)
{
	RECT FormatRect;
	static float fTimeSum = 0.0f;

	GetClientRect(hWnd, &FormatRect);//获取用户区域(UserArea)

	//Step1 Clear清除
	g_pD3D9Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//Step2 开始渲染
	g_pD3D9Device->BeginScene();

	//Step3 渲染代码
	fTimeSum += fDeltaTime;

	if(fTimeSum < 5.0f)//Loading
	{
		SceneSig_Render(fTimeSum);//Sig场景渲染
	}
	else//Title
	{
		if(g_nKeySelectState == 0)
		{
			SceneTitle_Render(fDeltaTime);
		}
		else if(g_nKeySelectState == 1)
		{
			switch(g_nKeyNumber)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				SceneOption_Render(fDeltaTime);
				break;
			default:
				break;
			}
		}
		else
		{

		}

		/*g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		g_pD3D9Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		g_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);//D3DTA_DIFFUSE//D3DTA_TEXTURE
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

		if(g_nKeySelectState == 0)
		{
			//绘制标题
			g_pD3D9Device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex));
			g_pD3D9Device->SetFVF(D3DFVF_VERTEX);
			g_pD3D9Device->SetIndices(g_pIndexBuffer);
			g_pD3D9Device->SetTexture(0, g_pTextureTitle);
			g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

			//绘制文字
			g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
			g_pD3D9Device->SetTexture(0, g_pTextureLogo);
			g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 4, 0, 2);
			g_pD3D9Device->SetTexture(0, g_pTextureVersion);
			g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8, 0, 4, 0, 2);

			//绘制游戏选项
			g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 0.65f, 0.65f, 1.0f));
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
			g_pD3D9Device->SetTexture(0, g_pTextureTitleText);
			g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 12, 0, 32, 0, 16);
			if(g_bKeySelectFlashState == false)
			{
				g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			}
			else
			{
				g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
				g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
			}
			g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 44, 0, 4, 0, 2);

			//绘制精灵动画
			g_nTitleFlagNow = g_pAnimation->RenderTitle(fDeltaTime);
		}
		else if(g_nKeySelectState == 1)
		{
			if(g_nKeyNumber == 0)//GameStart
			{

			}
			else if(g_nKeyNumber == 1)//ExtraStart
			{

			}
			else if(g_nKeyNumber == 2)//PracticeStart
			{

			}
			else if(g_nKeyNumber == 3)//Replay
			{

			}
			else if(g_nKeyNumber == 4)//PlayerData
			{

			}
			else if(g_nKeyNumber == 5)//MusicRoom
			{

			}
			else if(g_nKeyNumber == 6)//Option
			{
				//绘制标题
				g_pD3D9Device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex));
				g_pD3D9Device->SetFVF(D3DFVF_VERTEX);
				g_pD3D9Device->SetIndices(g_pIndexBuffer);
				g_pD3D9Device->SetTexture(0, g_pTextureTitle);
				g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

				//绘制文字
				g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
				g_pD3D9Device->SetTexture(0, g_pTextureLogo);
				g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 4, 0, 2);
				g_pD3D9Device->SetTexture(0, g_pTextureVersion);
				g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8, 0, 4, 0, 2);

				if(g_bKeySelectTitleChange == false)
				{
					//绘制游戏选项
					g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 0.65f, 0.65f, 1.0f));
					g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
					g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
					g_pD3D9Device->SetTexture(0, g_pTextureTitleText);
					g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 12, 0, 20, 0, 10);
					if(g_bKeySelectFlashState == false)
					{
						g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
						g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					}
					else
					{
						g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
						g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
						g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
						g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
					}
					g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 32, 0, 4, 0, 2);
				}
				else
				{
					//绘制游戏选项
					g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 0.65f, 0.65f, 1.0f));
					g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
					g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
					g_pD3D9Device->SetTexture(0, g_pTextureTitleText);
					g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 12, 0, 32, 0, 16);
					if(g_bKeySelectFlashState == false)
					{
						g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
						g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					}
					else
					{
						g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
						g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
						g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
						g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
					}
					g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 44, 0, 4, 0, 2);
				}
				
				//绘制精灵动画
				g_nTitleFlagNow = g_pAnimation->RenderTitle(fDeltaTime);
			}
		}

		g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);*/
	}

	//绘制文字
	int CharCount = swprintf_s(g_sFPS, 20, _T("%0.1ffps"), Direct3D_FPS());
	g_pFontFPS->DrawText(NULL, g_sFPS, CharCount, &FormatRect, DT_BOTTOM | DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//fps帧速率

	//g_pFontAdapter->DrawText(NULL, g_sAdapterName, -1, &FormatRect, DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));//Adapter型号

	//Step4 结束渲染
	g_pD3D9Device->EndScene();

	//Step5 Display显示
	g_pD3D9Device->Present(NULL, NULL, NULL, NULL);
}

/*
** Function:  Direct3D_ClearUp()
** Purpose:  Direct3D释放资源
*/
void Direct3D_ClearUp(void)
{
	SAFE_DELETE(g_pSnow);//释放Particle粒子
	SAFE_DELETE(g_pAnimation);//释放Animation动画
	SAFE_DELETE(g_pDirectInput);//释放DirectInput输入设备
	SAFE_DELETE(g_pDirectSound);//释放DirectSound输出设备
	SAFE_DELETE(g_pDirectSoundEffect);//释放DirectSoundEffect输出设备
	SAFE_RELEASE(g_pFontFPS);//释放字体(帧速率fps)
	SAFE_RELEASE(g_pFontAdapter);//释放字体(显卡型号Adapter)
	SAFE_RELEASE(g_pTexture);//释放纹理(Texture)
	SAFE_RELEASE(g_pTextureTitle);//释放纹理(Texture:标题00)
	SAFE_RELEASE(g_pTextureLoading);//释放纹理(Texture)
	SAFE_RELEASE(g_pTextureLogo);//释放纹理(Texture:标题00logo)
	SAFE_RELEASE(g_pTextureVersion);//释放纹理(Texture:标题00Version)
	SAFE_RELEASE(g_pTextureTitleText);//释放纹理(Texture:标题01Text)
	SAFE_RELEASE(g_pVertexBuffer);//释放顶点缓存(VertexBuffer)
	SAFE_RELEASE(g_pIndexBuffer);//释放索引缓存(IndexBuffer)
	SAFE_RELEASE(g_pD3D9Device);//释放设备(D3D9Device)
}

/*
** Function:  Direct3D_FPS()
** Purpose:  Direct3D计算fps
** Return:  float fps
*/
float Direct3D_FPS(void)
{
	static int FrameCount = 0;
	float CurrentTime = 0.0f;
	static float LastTime = 0.0f;
	static float fps = 0.0f;
	float TimeDelta = 0.0f;

	FrameCount++;
	CurrentTime = (float)GetTickCount();
	TimeDelta = (CurrentTime - LastTime) * 0.001f;

	if(TimeDelta >= 1.0f)
	{
		fps = (float)(FrameCount * 1.0f / TimeDelta);
		LastTime = CurrentTime;
		FrameCount = 0;
	}

	return fps;
}

/*
** Function:  MatrixSet()
** Purpose:  Direct3D坐标变换
*/
void MatrixSet(void)
{
	//WorldSpace世界坐标系
	D3DXMATRIX MatrixWorld, Rx, Ry, Rz;
	float Sx = 1.0f, Sy = 1.0f, Sz = 1.0f;
	D3DXMatrixIdentity(&MatrixWorld);//单位化矩阵
	D3DXMatrixRotationX(&Rx, 0.0f);
	D3DXMatrixRotationY(&Ry, 0.0f);
	D3DXMatrixRotationZ(&Rz, 0.0f);
	D3DXMatrixScaling(&MatrixWorld, Sx, Sy, Sz);
	D3DXMatrixTranslation(&MatrixWorld, 0.0f, 0.0f, 0.0f);
	MatrixWorld = Rx*Ry*Rz*MatrixWorld;
	g_pD3D9Device->SetTransform(D3DTS_WORLD, &MatrixWorld);

	//ViewSpace取景坐标系
	D3DXMATRIX MatrixView;
	D3DXVECTOR3 Vector_Eye(0.0f, 15.0f, -25.0f);
	D3DXVECTOR3 Vector_At(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Vector_Up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&MatrixView, &Vector_Eye, &Vector_At, &Vector_Up);
	g_pD3D9Device->SetTransform(D3DTS_VIEW, &MatrixView);

	//PerspectiveProjection投影坐标系
	D3DXMATRIX MatrixProject;
	D3DXMatrixPerspectiveFovLH(&MatrixProject, D3DX_PI * 0.5f, (float)USER_SCREENWIDTH / (float)USER_SCREENHEIGHT, 1.0f, 1000.0f);
	g_pD3D9Device->SetTransform(D3DTS_PROJECTION, &MatrixProject);

	//ViewPortTransform视口坐标系
	D3DVIEWPORT9 ViewPort;
	ViewPort.X = 0;
	ViewPort.Y = 0;
	ViewPort.Width = USER_SCREENWIDTH;
	ViewPort.Height = USER_SCREENHEIGHT;
	ViewPort.MinZ = 0.0f;
	ViewPort.MaxZ = 1.0f;
	g_pD3D9Device->SetViewport(&ViewPort);

}