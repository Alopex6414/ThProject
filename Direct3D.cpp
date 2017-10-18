#include"Common.h"
#include "Include.h"

/*
** Global Variable
*/
//D3DDevice
LPDIRECT3DDEVICE9 g_pD3D9Device = NULL;//Direct3D�豸

//D3DVertex/D3DIndex
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;//���㻺��
LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;//��������

//D3DTexture
LPDIRECT3DTEXTURE9 g_pTexture = NULL;//��������
LPDIRECT3DTEXTURE9 g_pTextureLoading = NULL;//�ȴ�����
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;//��������
LPDIRECT3DTEXTURE9 g_pTextureLogo = NULL;//����logo����
LPDIRECT3DTEXTURE9 g_pTextureVersion = NULL;//����汾����
LPDIRECT3DTEXTURE9 g_pTextureTitleText = NULL;//������������

//D3DFont
LPD3DXFONT g_pFontFPS = NULL;//֡�����ַ��ӿ�
LPD3DXFONT g_pFontAdapter = NULL;//�Կ��ͺ���Ϣ�ַ��ӿ�

//DirectInput
DirectInput*g_pDirectInput = new DirectInput();//DirectInput�ӿ�

//DirectSound
DirectSound*g_pDirectSound = new DirectSound();//DirectSound�ӿ�(����)
DirectSound*g_pDirectSoundEffect = new DirectSound();//DirectSound�ӿ�(��Ч)

//Particle
Particle*g_pSnow;//����

//Animation
Animation*g_pAnimation;//����

//Subtitle
//Sig...
Subtitle*g_pSubSig = new Subtitle(0.0f, 0.0f, 640.0f, 480.0f, 0.0f);//Sig����ҳ��
Subtitle*g_pSubLoad = new Subtitle(415.0f, 375.0f, 128.0f, 32.0f, 0.0f, 0.0f, false);//"��Ů����..."
Subtitle*g_pSubLoadEn = new Subtitle(462.0f, 392.0f, 128.0f, 32.0f, 20.0f, 0.0f, true);//"Loading..."
//Title...
Subtitle*g_pSubTitle = new Subtitle(0.0f, 0.0f, 640.0f, 480.0f, 0.0f);//Title����ҳ��
Subtitle*g_pSubVersion = new Subtitle(304.0f, 431.0f, 64.0f, 16.0f, 0.0f);//Version�汾��
Subtitle*g_pSubGameOptionSelect = new Subtitle(540.0f, 256.0f, 128.0f, 32.0f, 0.0f, 0.0f, true);//GameOptionSelect��Ϸѡ����
Subtitle*g_pSubArrayGameOptions = new Subtitle[8]//GameOptions��Ϸѡ��(������)
{
	Subtitle(540.0f, 256.0f, 128.0f, 32.0f, 540.0f, 0.0f, true),//GameStartѡ��
	Subtitle(556.0f, 280.0f, 128.0f, 32.0f, 556.0f, 0.0f, true),//ExtraStartѡ��
	Subtitle(572.0f, 304.0f, 128.0f, 32.0f, 572.0f, 0.0f, true),//PracticeStartѡ��
	Subtitle(588.0f, 328.0f, 128.0f, 32.0f, 588.0f, 0.0f, true),//Replayѡ��
	Subtitle(604.0f, 352.0f, 128.0f, 32.0f, 604.0f, 0.0f, true),//PlayerDataѡ��
	Subtitle(620.0f, 376.0f, 128.0f, 32.0f, 620.0f, 0.0f, true),//MusicRoomѡ��
	Subtitle(636.0f, 400.0f, 128.0f, 32.0f, 636.0f, 0.0f, true),//Optionѡ��
	Subtitle(652.0f, 424.0f, 128.0f, 32.0f, 652.0f, 0.0f, true),//Quitѡ��
};
//Option...
Subtitle*g_pSubOptionConfigSelect = new Subtitle(540.0f, 304.0f, 128.0f, 32.0f, 540.0f, 0.0f, true);//OptionConfigѡ����
Subtitle*g_pSubArrayOptionsConfig = new Subtitle[5]//OptionsConfigѡ������(Option����)
{
	Subtitle(540.0f, 304.0f, 128.0f, 32.0f, 540.0f, 0.0f, true),//BGM Volumeѡ��
	Subtitle(556.0f, 328.0f, 128.0f, 32.0f, 556.0f, 0.0f, true),//SE Volumeѡ��
	Subtitle(572.0f, 352.0f, 128.0f, 32.0f, 572.0f, 0.0f, true),//Key Configѡ��
	Subtitle(588.0f, 376.0f, 128.0f, 32.0f, 588.0f, 0.0f, true),//Defaultѡ��
	Subtitle(604.0f, 400.0f, 128.0f, 32.0f, 604.0f, 0.0f, true),//Quitѡ��
};

//Variable
float	g_FPS = 0.0f;//֡����
wchar_t	g_sFPS[20];//fps֡�����ַ�����
wchar_t g_sAdapterName[60];//Adapter�Կ������ַ�����
int g_nTitleFlagNow = 0;
int g_nKeyNumber = 0;//Title00
int g_nKeyNumber1 = 0;//Title01
int g_nKeyNumber_Option = 0;//Option
int g_nKeySelectState = 0;//Title״̬
bool g_bKeySelectFlashState = false;
bool g_bKeySelectTitleChange = false;

/*
** Function:  Direct3D_Init(HWND hWnd)
** Purpose:  Direct3D��ʼ��
** Para: HWND hWnd(���ھ��)
** Return:  HRESULT��ʼ��״̬
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

	//Step1 ��ȡIDirect3D9�ӿ�ָ��
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);//��ȡIDirect3D9�ӿ�ָ��

	if(pD3D9 == NULL)//IDirect3D9�ӿ�ָ��ΪNULL
	{
		return E_FAIL;//FAIL
	}

	//Step2 У��Ӳ����������
	Result = pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps);//У��Ӳ����������

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	if(Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		Vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;//֧��Ӳ����������(Ӳ����������)
	}
	else
	{
		Vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//��֧��Ӳ����������(�����������)
	}

	//Step3 ���D3DPRESENT_PARAMETERS�ṹ
	D3Dpp.BackBufferWidth = USER_SCREENWIDTH;//��̨���������(Pixel)
	D3Dpp.BackBufferHeight = USER_SCREENHEIGHT;//��̨�������߶�(Pixel)
	D3Dpp.BackBufferFormat = D3DFMT_A8R8G8B8;//��̨�������ظ�ʽ
	D3Dpp.BackBufferCount = 1;//��̨��������(1)
	D3Dpp.MultiSampleType = D3DMULTISAMPLE_NONE;//��̨������ز�������
	D3Dpp.MultiSampleQuality = 0;//��̨������ز�������
	D3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//������ҳ���û���ʽ
	D3Dpp.hDeviceWindow = hWnd;//�豸��ش��ھ��
	D3Dpp.Windowed = true;//����ģʽ:true/ȫ��ģʽ:false
	D3Dpp.EnableAutoDepthStencil = true;//Direct3D�Զ�����ά����Ȼ����ģ�建��
	D3Dpp.AutoDepthStencilFormat = D3DFMT_D24S8;//��Ȼ����ģ�建�����ظ�ʽ
	D3Dpp.Flags = 0;//�ޱ�Ǹ�ʽ
	D3Dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//Direct3DĬ��ˢ��Ƶ��
	D3Dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//Direct3D�ύƵ��(Ĭ���ύ)

	//Step4 ����IDirect3DDevice9�ӿ�
	Result = pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, Vp, &D3Dpp, &g_pD3D9Device);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//Step4 ��ȡAdapter�Կ��ͺ�
	pD3D9->GetAdapterIdentifier(0, 0, &Adapter);
	Length = MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, g_sAdapterName, Length);
	wcscat_s(AdapterName, g_sAdapterName);
	wcscpy_s(g_sAdapterName, AdapterName);

	//Step5 �ͷ���Դ
	SAFE_RELEASE(pD3D9);

	return S_OK;
}

/*
** Function:  Direct3D_SetUp()
** Purpose:  Direct3DӦ�ó����ʼ��
** Return:  HRESULT��ʼ��״̬
*/
HRESULT Direct3D_SetUp(void)
{
	HRESULT Result;

	//����D3DFont����

	//D3DFont֡����fps
	Result = D3DXCreateFont(g_pD3D9Device, 12, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("����"), &g_pFontFPS);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//D3DFont�Կ��ͺ�Adapter
	Result = D3DXCreateFont(g_pD3D9Device, 20, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &g_pFontAdapter);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//����D3DVertex/D3DIndex����/����

	//�������㻺��(48)
	Result = g_pD3D9Device->CreateVertexBuffer(48 * sizeof(Vertex), 0, D3DFVF_VERTEX, D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//������������(72)
	Result = g_pD3D9Device->CreateIndexBuffer(72 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIndexBuffer, NULL);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//����D3DTexture����
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

	//��������ϵͳ
	g_pSnow = new Particle(g_pD3D9Device, 420, 350, 160, 90);
	Result = g_pSnow->Initialize(L"Resource\\ascii\\loading.png");

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//��������ϵͳ
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
** Purpose:  DirectInput��ʼ��
** Para: HWND hWnd(���ھ��)
** Para: HINSTANCE hInstance(����ʵ�����)
** Return:  HRESULT��ʼ��״̬
*/
HRESULT DirectInput_Init(HWND hWnd, HINSTANCE hInstance)
{
	HRESULT Result;

	Result = g_pDirectInput->Initialize(hWnd, hInstance, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);//KeyBoard,Mouse��Ϊ�豸��̨ģʽ/����ģʽ(���ó�ǰ̨ģʽ/��ռģʽ?)

	return Result;
}

/*
** Function:  DirectSound_Init(void)
** Purpose:  DirectSound��ʼ��
** Para: HWND hWnd(���ھ��)
** Return:  HRESULT��ʼ��״̬
*/
HRESULT DirectSound_Init(HWND hWnd)
{
	HRESULT Result;

	Result = g_pDirectSound->Initialize(hWnd, DSSCL_NORMAL);//DirectSound BGM����Э������

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = g_pDirectSound->LoadWave(L"Resource\\bgm\\th11_01.wav");//DirectSound BGM���ر�������

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = g_pDirectSoundEffect->Initialize(hWnd, DSSCL_NORMAL);//DirectSound Effect����Э������

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound Effect������Ч

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	return S_OK;
}

/*
** Function:  Direct3D_Update(HWND hWnd, float fDeltaTime)
** Purpose:  Direct3D����
** Para: HWND hWnd(���ھ��)
** Para: float fDeltaTime(ʱ����)
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

	fTimeSum += fDeltaTime;//��Ϸ����ʱ��

	if(fTimeSum < 5.0f)//Loading(Loading����5.0s)
	{
		SceneSig_Update(fTimeSum, fDeltaTime);//Sig��������
	}
	else//Title������(5.0s�����Title����)
	{
		SceneSig_Release();//Sig�����ͷ�

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
		g_pDirectSound->PlayMusic();//���ű���BGM����

		if(g_nTitleFlagNow < 5)//Title������־0~4(��ɫ����)("�|����둵�")
		{
			fAlphaValue = 0.5f;//"��Ů����..."͸����Alpha 0.0f, 0%//0.5f
			bAlphaFlag = true;//Alpha��־���õ���

			g_pSubTitle->SetAlpha(0.0f);//Title���ⱳ��Alpha͸����0.0f(0%)
			g_pSubVersion->SetAlpha(0.0f);//Version�汾��Alpha͸����0.0f(0%)
			g_pSubGameOptionSelect->SetAlpha(0.5f);//��Ϸѡ����Alpha͸����0.5f(50%)
		}

		if(g_nTitleFlagNow == 5)//Title������־5(Title�����������)
		{
			//Title�����������
			g_pSubTitle->AlphaIncrease(1.0f, 0.02f);//Title����͸����Alpha����0.02f,����50֡,0.83s
			if(g_pSubTitle->GetAlpha() >= 0.8f)
			{
				g_pSubVersion->AlphaIncrease(1.0f, 0.1f);//"V1.00a"͸����Alpha����0.1f,����10֡
			}

			//"��Ϸѡ��"����ƫ��
			pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ�ƫ��λ�������ƶ�(8x/frame)
			for(int i = 0; i < 8; i++)
			{
				*(pfVertexX + i) += -8;
				if(*(pfVertexX + i) < 476.0f)//"��Ϸѡ��"��������(X:476.0f)
				{
					*(pfVertexX + i) = 476.0f;
				}
			}

			//-----------------------------------------------��Ϸ����ƫ��----------------------------------------------------
			for(int i = 0; i < 8; i++)
			{
				(g_pSubArrayGameOptions + i)->MoveXDecrease(476.0f, 8.0f);//GameOptions��Ϸѡ��X����ƫ��
			}

			//"��Ϸѡ��"Alpha͸���ȱ仯
			pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ����0.0f,����1.0f,����10֡
			for(int i = 0; i < 8; i++)
			{
				if(*(pfVertexX + i) == 0.0f)//"��Ϸѡ��"�ַ�ֻ�е�һ��AlphaΪ0.0f����,����ַ�Alpha������(break)
				{
					*(pfVertexX + i) += 0.1f;
					break;
				}
				else
				{
					*(pfVertexX + i) += 0.1f;//�����ַ�Alphaֵ��Ϊ0.0f������0.1f
					if(*(pfVertexX + i) > 1.0f)//Alpha�޷�1.0f
					{
						*(pfVertexX + i) = 1.0f;
					}
				}
			}

			//----------------------------------------��Ϸѡ��Alpha͸���ȱ仯-------------------------------------------
			for(int i = 0; i < 8; i++)
			{
				if((g_pSubArrayGameOptions + i)->GetAlpha() == 0.0f)//GameOptions��Ϸѡ��ֻ�е�һ��AlphaΪ0.0f����,����ַ�Alpha������(break)
				{
					(g_pSubArrayGameOptions + i)->AddAlpha(0.1f);
					break;
				}
				else
				{
					(g_pSubArrayGameOptions + i)->AlphaIncrease(1.0f, 0.1f);
				}
			}

			//"��Ϸѡ��"Alphaͨ���仯
			if(bAlphaFlag == true)//Alphaֵ����,����0.025f,����40֡
			{
				fAlphaValue += 0.025f;
				if(fAlphaValue > 1.0f)//Alphaֵ�޷�1.0f
				{
					fAlphaValue = 1.0f;
					bAlphaFlag = false;
				}
			}
			else//Alphaֵ�ݼ�,����0.025f,����40֡
			{
				fAlphaValue += -0.025f;
				if(fAlphaValue < 0.5f)//Alphaֵ�޷�0.5f
				{
					fAlphaValue = 0.5f;
					bAlphaFlag = true;
				}
			}

			//----------------------------------------��Ϸѡ����Alpha͸���ȱ仯-------------------------------------------
			g_pSubGameOptionSelect->AlphaCircle(0.5f, 1.0f, 0.025f);//GameOptionSelect��Ϸѡ����Alpha͸���ȱ仯
		}

		if(g_nTitleFlagNow == 6)//Title������־6(DirectInput�豸(����KeyBoard)������Ϸѡ��)
		{
			g_pDirectInput->GetDeviceState();//��ȡ����״̬(KeyBoard)

			if(g_nKeySelectState == 0)//״̬0:Tilte00����(����������)
			{
				if(bKeySelect == false)//����δѡ��(����״̬��־)
				{
					//��ⷽ���"��"״̬
					if(g_pDirectInput->IsKeyDown(DIK_UP))//�����"��"
					{
						bKeySelect = true;//������ѡ��
						g_nKeyNumber--;
						if(g_nKeyNumber < 0)
						{
							g_nKeyNumber = 7;
						}

						g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_select00.wav");//DirectSound���ز�����Чse_select00.wav

						bKeyMoveFlag = false;
						bKeyMoveDirect = false;
						fKeyMoveX = 0.0f;
					}

					//��ⷽ���"��"״̬
					if(g_pDirectInput->IsKeyDown(DIK_DOWN))//�����"��"
					{
						bKeySelect = true;//������ѡ��
						g_nKeyNumber++;
						if(g_nKeyNumber > 7)
						{
							g_nKeyNumber = 0;
						}

						g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_select00.wav");//DirectSound���ز�����Чse_select00.wav

						bKeyMoveFlag = false;
						bKeyMoveDirect = false;
						fKeyMoveX = 0.0f;
					}

					//���"Z"��״̬
					if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"��ȷ��
					{
						//bKeySelect = true;
						if(bKeySelectZ == false)//"Z"���ͷ�
						{
							bKeySelect = true;

							g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_ok00.wav");//DirectSound���ز�����Чse_ok00.wav

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}
						bKeySelectZ = true;//"Z"������
					}
					else
					{
						bKeySelectZ = false;//"Z"���ͷ�
					}

					//���"X"��/"Esc"��״̬
					if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"��/"Esc"��ȡ��
					{
						if(bKeyCancelLast == false)//"Esc"�˳���־δ��λ
						{
							bKeySelect = true;
							if(g_nKeyNumber != 7)
							{
								g_nKeyNumber = 7;
								g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_cancel00.wav");//DirectSound���ز�����Чse_cancel00.wav
							}
							else
							{
								g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_cancel00.wav");//DirectSound���ز�����Чse_cancel00.wav
								Sleep(500);//�߳�˯��500ms
								DestroyWindow(hWnd);//���ٴ���
							}

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						bKeyCancelLast = true;//"Esc"�˳���־��λ
					}
					else
					{
						bKeyCancelLast = false;//"Esc"�˳���־δ��λ
					}

					//"��Ϸѡ��"Alphaͨ���仯
					if(bAlphaFlag == true)//Alphaֵ����,����0.025f,����40֡
					{
						fAlphaValue += 0.025f;
						if(fAlphaValue > 1.0f)//Alphaֵ�޷�1.0f
						{
							fAlphaValue = 1.0f;
							bAlphaFlag = false;
						}
					}
					else//Alphaֵ�ݼ�,����0.025f,����40֡
					{
						fAlphaValue += -0.025f;
						if(fAlphaValue < 0.5f)//Alphaֵ�޷�0.5f
						{
							fAlphaValue = 0.5f;
							bAlphaFlag = true;
						}
					}
				}
				else//������ѡ��
				{
					if(bKeySelectZ == false)//"Z"���ͷ�״̬
					{
						fKeyPressTime += fDeltaTime;//�������ʱ����
						if(fKeyPressTime > 0.12f && bKeyMoveFlag == true)//��������ʱ��0.12s(KeyBoard����0.12s)
						{
							fKeyPressTime = 0.0f;
							bKeySelect = false;//����δѡ��
						}

						if(bKeyMoveFlag == false)//�����ƶ���־(Left2->Right4->Left2)
						{
							if(bKeyMoveDirect == false)//�����ƶ�����(false:Left/true:Right)
							{
								fKeyMoveX += 2.0f;//�����ƶ�Xƫ������2.0f
								if(fKeyMoveX >= 4.0f)//Left:2֡
								{
									fKeyMoveX = 4.0f;
									bKeyMoveDirect = true;//Right
								}
								if(fKeyMoveX == 0.0f)//Left:2֡
								{
									bKeyMoveFlag = true;
								}
							}
							else//Right:4֡
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
							fKeyMoveX = 0.0f;//����ƫ��λ��ԭ��
						}
					}
					else//"Z"������
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
							//"��Ϸѡ��"����ƫ��
							pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ�ƫ��λ�������ƶ�(8x/frame)
							for(int i = 0; i < 8; i++)
							{
								if(i != g_nKeyNumber)//��ǰ"��Ϸѡ��"�ַ�ƫ�Ʋ���
								{
									*(pfVertexX + i) += 4;
									if(*(pfVertexX + i) > 640.0f)//"��Ϸѡ��"��������(X:640.0f)
									{
										*(pfVertexX + i) = 640.0f;
									}
								}
								else
								{
									if(nKeySelectFlashTimesCount >= 13)
									{
										*(pfVertexX + i) += 8;
										if(*(pfVertexX + i) > 640.0f)//"��Ϸѡ��"��������(X:640.0f)
										{
											*(pfVertexX + i) = 640.0f;
										}
									}
								}
							}

							//"��Ϸѡ��"Alpha͸���ȱ仯
							pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ����0.0f,����1.0f,����10֡(fAlphaValueArray�����ʱֵ��Ϊ1.0f)
							for(int i = 0; i < 8; i++)
							{
								if(i != g_nKeyNumber)//��ǰ"��Ϸѡ��"Alphaͨ������
								{
									*(pfVertexX + i) -= 0.05f;//Alphaͨ��ֵ����0.1f,����10֡
									if(*(pfVertexX + i) < 0.0f)
									{
										*(pfVertexX + i) = 0.0f;
									}
								}
								else
								{
									if(nKeySelectFlashTimesCount >= 13)
									{
										*(pfVertexX + i) -= 0.075f;//Alphaͨ��ֵ����0.1f,����10֡
										if(*(pfVertexX + i) < 0.0f)
										{
											*(pfVertexX + i) = 0.0f;
										}
									}
								}
							}

							if(nKeySelectFlashTimesCount >= 13)//������Ⱦ
							{
								bKeySelectFlashState = false;
							}

							if(nKeySelectFlashTimesCount >= 20)
							{
								nKeySelectFlashTimesCount = 0;
								nKeySelectFlashCount = 0;
								bKeySelectFlashState = false;
								fKeyPressTime = 0.0f;
								bKeySelect = false;//����δѡ��
								g_nKeySelectState = 1;

								if(g_nKeyNumber == 0)//GameStart(Key����������)
								{

								}
								else if(g_nKeyNumber == 1)//ExtraStart(Key����������)
								{

								}
								else if(g_nKeyNumber == 2)//PracticeStart(Key����������)
								{

								}
								else if(g_nKeyNumber == 3)//Replay(Key����������)
								{

								}
								else if(g_nKeyNumber == 4)//PlayerData(Key����������)
								{

								}
								else if(g_nKeyNumber == 5)//MusicRoom(Key����������)
								{

								}
								else if(g_nKeyNumber == 6)//Option(Key����������)
								{
									//Title01ѡ���ʼ��
									g_nKeyNumber1 = 0;

									//"��Ϸѡ��"����ƫ��(�ָ���ʼ״̬)
									pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ��ָ���ʼ״̬
									for(int i = 0; i < 8; i++)
									{
										*(pfVertexX + i) = 540.0f + i * 16.0f;
									}

									//"��Ϸѡ��"Alpha͸���ȱ仯
									pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ���ָ���ʼ״̬
									for(int i = 0; i < 8; i++)
									{
										*(pfVertexX + i) = 0.0f;
									}
								}
								else if(g_nKeyNumber == 7)//Quit
								{
									Sleep(500);//�߳�˯��500ms
									DestroyWindow(hWnd);//���ٴ���
								}
							}


						}
					}

					g_bKeySelectFlashState = bKeySelectFlashState;
				}
			}
			else if(g_nKeySelectState == 1)//״̬1:Tilte01����(ѡ��...���尴ťѡ��)
			{
				if(bKeySelect == false)//����δѡ��(����״̬��־)
				{
					if(g_nKeyNumber == 0)//GameStart
					{
						//��ⷽ���"��"״̬
						if(g_pDirectInput->IsKeyDown(DIK_UP))//�����"��"
						{
							bKeySelect = true;//������ѡ��

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//��ⷽ���"��"״̬
						if(g_pDirectInput->IsKeyDown(DIK_DOWN))//�����"��"
						{
							bKeySelect = true;//������ѡ��

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//���"Z"��״̬
						if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"��ȷ��
						{
							//bKeySelect = true;
							if(bKeySelectZ == false)//"Z"���ͷ�
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound������Чse_ok00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}
							bKeySelectZ = true;//"Z"������
						}
						else
						{
							bKeySelectZ = false;//"Z"���ͷ�
						}

						//���"X"��/"Esc"��״̬
						if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"��/"Esc"��ȡ��
						{
							if(bKeyCancelLast == false)//"Esc"�˳���־δ��λ
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound������Чse_cancel00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								//"��Ϸѡ��"����ƫ��(�ָ���ʼ״̬)
								pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ��ָ���ʼ״̬
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
								}

								//"��Ϸѡ��"Alpha͸���ȱ仯
								pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ���ָ���ʼ״̬
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 0.0f;
								}

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							bKeyCancelLast = true;//"Esc"�˳���־��λ
						}
						else
						{
							bKeyCancelLast = false;//"Esc"�˳���־δ��λ
						}
					}
					else if(g_nKeyNumber == 1)//ExtraStart
					{
						//��ⷽ���"��"״̬
						if(g_pDirectInput->IsKeyDown(DIK_UP))//�����"��"
						{
							bKeySelect = true;//������ѡ��

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//��ⷽ���"��"״̬
						if(g_pDirectInput->IsKeyDown(DIK_DOWN))//�����"��"
						{
							bKeySelect = true;//������ѡ��

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//���"Z"��״̬
						if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"��ȷ��
						{
							//bKeySelect = true;
							if(bKeySelectZ == false)//"Z"���ͷ�
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound������Чse_ok00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}
							bKeySelectZ = true;//"Z"������
						}
						else
						{
							bKeySelectZ = false;//"Z"���ͷ�
						}

						//���"X"��/"Esc"��״̬
						if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"��/"Esc"��ȡ��
						{
							if(bKeyCancelLast == false)//"Esc"�˳���־δ��λ
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound������Чse_cancel00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								//"��Ϸѡ��"����ƫ��(�ָ���ʼ״̬)
								pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ��ָ���ʼ״̬
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
								}

								//"��Ϸѡ��"Alpha͸���ȱ仯
								pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ���ָ���ʼ״̬
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 0.0f;
								}

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							bKeyCancelLast = true;//"Esc"�˳���־��λ
						}
						else
						{
							bKeyCancelLast = false;//"Esc"�˳���־δ��λ
						}
					}
					else if(g_nKeyNumber == 2)//PracticeStart
					{
						//��ⷽ���"��"״̬
						if(g_pDirectInput->IsKeyDown(DIK_UP))//�����"��"
						{
							bKeySelect = true;//������ѡ��

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//��ⷽ���"��"״̬
						if(g_pDirectInput->IsKeyDown(DIK_DOWN))//�����"��"
						{
							bKeySelect = true;//������ѡ��

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//���"Z"��״̬
						if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"��ȷ��
						{
							//bKeySelect = true;
							if(bKeySelectZ == false)//"Z"���ͷ�
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound������Чse_ok00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}
							bKeySelectZ = true;//"Z"������
						}
						else
						{
							bKeySelectZ = false;//"Z"���ͷ�
						}

						//���"X"��/"Esc"��״̬
						if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"��/"Esc"��ȡ��
						{
							if(bKeyCancelLast == false)//"Esc"�˳���־δ��λ
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound������Чse_cancel00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								//"��Ϸѡ��"����ƫ��(�ָ���ʼ״̬)
								pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ��ָ���ʼ״̬
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
								}

								//"��Ϸѡ��"Alpha͸���ȱ仯
								pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ���ָ���ʼ״̬
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 0.0f;
								}

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							bKeyCancelLast = true;//"Esc"�˳���־��λ
						}
						else
						{
							bKeyCancelLast = false;//"Esc"�˳���־δ��λ
						}
					}
					else if(g_nKeyNumber == 3)//Replay
					{
						//��ⷽ���"��"״̬
						if(g_pDirectInput->IsKeyDown(DIK_UP))//�����"��"
						{
							bKeySelect = true;//������ѡ��

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//��ⷽ���"��"״̬
						if(g_pDirectInput->IsKeyDown(DIK_DOWN))//�����"��"
						{
							bKeySelect = true;//������ѡ��

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//���"Z"��״̬
						if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"��ȷ��
						{
							//bKeySelect = true;
							if(bKeySelectZ == false)//"Z"���ͷ�
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound������Чse_ok00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}
							bKeySelectZ = true;//"Z"������
						}
						else
						{
							bKeySelectZ = false;//"Z"���ͷ�
						}

						//���"X"��/"Esc"��״̬
						if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"��/"Esc"��ȡ��
						{
							if(bKeyCancelLast == false)//"Esc"�˳���־δ��λ
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound������Чse_cancel00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								//"��Ϸѡ��"����ƫ��(�ָ���ʼ״̬)
								pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ��ָ���ʼ״̬
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
								}

								//"��Ϸѡ��"Alpha͸���ȱ仯
								pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ���ָ���ʼ״̬
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 0.0f;
								}

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							bKeyCancelLast = true;//"Esc"�˳���־��λ
						}
						else
						{
							bKeyCancelLast = false;//"Esc"�˳���־δ��λ
						}
					}
					else if(g_nKeyNumber == 4)//PlayerData
					{
						//��ⷽ���"��"״̬
						if(g_pDirectInput->IsKeyDown(DIK_UP))//�����"��"
						{
							bKeySelect = true;//������ѡ��

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//��ⷽ���"��"״̬
						if(g_pDirectInput->IsKeyDown(DIK_DOWN))//�����"��"
						{
							bKeySelect = true;//������ѡ��

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//���"Z"��״̬
						if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"��ȷ��
						{
							//bKeySelect = true;
							if(bKeySelectZ == false)//"Z"���ͷ�
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound������Чse_ok00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}
							bKeySelectZ = true;//"Z"������
						}
						else
						{
							bKeySelectZ = false;//"Z"���ͷ�
						}

						//���"X"��/"Esc"��״̬
						if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"��/"Esc"��ȡ��
						{
							if(bKeyCancelLast == false)//"Esc"�˳���־δ��λ
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound������Чse_cancel00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								//"��Ϸѡ��"����ƫ��(�ָ���ʼ״̬)
								pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ��ָ���ʼ״̬
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
								}

								//"��Ϸѡ��"Alpha͸���ȱ仯
								pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ���ָ���ʼ״̬
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 0.0f;
								}

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							bKeyCancelLast = true;//"Esc"�˳���־��λ
						}
						else
						{
							bKeyCancelLast = false;//"Esc"�˳���־δ��λ
						}
					}
					else if(g_nKeyNumber == 5)//MusicRoom
					{
						//��ⷽ���"��"״̬
						if(g_pDirectInput->IsKeyDown(DIK_UP))//�����"��"
						{
							bKeySelect = true;//������ѡ��

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//��ⷽ���"��"״̬
						if(g_pDirectInput->IsKeyDown(DIK_DOWN))//�����"��"
						{
							bKeySelect = true;//������ѡ��

							g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
							g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

							bKeyMoveFlag = false;
							bKeyMoveDirect = false;
							fKeyMoveX = 0.0f;
						}

						//���"Z"��״̬
						if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"��ȷ��
						{
							//bKeySelect = true;
							if(bKeySelectZ == false)//"Z"���ͷ�
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound������Чse_ok00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}
							bKeySelectZ = true;//"Z"������
						}
						else
						{
							bKeySelectZ = false;//"Z"���ͷ�
						}

						//���"X"��/"Esc"��״̬
						if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"��/"Esc"��ȡ��
						{
							if(bKeyCancelLast == false)//"Esc"�˳���־δ��λ
							{
								bKeySelect = true;

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound������Чse_cancel00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								//"��Ϸѡ��"����ƫ��(�ָ���ʼ״̬)
								pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ��ָ���ʼ״̬
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
								}

								//"��Ϸѡ��"Alpha͸���ȱ仯
								pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ���ָ���ʼ״̬
								for(int i = 0; i < 8; i++)
								{
									*(pfVertexX + i) = 0.0f;
								}

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							bKeyCancelLast = true;//"Esc"�˳���־��λ
						}
						else
						{
							bKeyCancelLast = false;//"Esc"�˳���־δ��λ
						}
					}
					else if(g_nKeyNumber == 6)//Option
					{
						//Titel01�л�����ʱ��ֹ����
						bKeyEnableFlag = true;
						pfVertexX = &fVertexXArray[0];
						for(int i = 0; i < 8; i++)
						{
							if(*(pfVertexX + i) != 444.0f)//�����X���겻����440.0f,��ֹ����
							{
								bKeyEnableFlag = false;
								break;
							}
						}

						if(bKeyEnableFlag == true)//����ʹ�ܱ�־�Ž��հ�����Ϣ
						{
							//��ⷽ���"��"״̬
							if(g_pDirectInput->IsKeyDown(DIK_UP))//�����"��"
							{
								bKeySelect = true;//������ѡ��
								g_nKeyNumber1--;
								if(g_nKeyNumber1 < 0)
								{
									g_nKeyNumber1 = 4;
								}

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							//��ⷽ���"��"״̬
							if(g_pDirectInput->IsKeyDown(DIK_DOWN))//�����"��"
							{
								bKeySelect = true;//������ѡ��
								g_nKeyNumber1++;
								if(g_nKeyNumber1 > 4)
								{
									g_nKeyNumber1 = 0;
								}

								g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_select00.wav");//DirectSound������Чse_select00.wav
								g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

								bKeyMoveFlag = false;
								bKeyMoveDirect = false;
								fKeyMoveX = 0.0f;
							}

							//���"Z"��״̬
							if(g_pDirectInput->IsKeyDown(DIK_Z))//"Z"��ȷ��
							{
								//bKeySelect = true;
								if(bKeySelectZ == false)//"Z"���ͷ�
								{
									bKeySelect = true;

									g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_ok00.wav");//DirectSound������Чse_ok00.wav
									g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

									bKeyMoveFlag = false;
									bKeyMoveDirect = false;
									fKeyMoveX = 0.0f;
								}

								bKeySelectZ = true;//"Z"������
							}
							else
							{
								bKeySelectZ = false;//"Z"���ͷ�
							}

							//���"X"��/"Esc"��״̬
							if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//"X"��/"Esc"��ȡ��
							{
								if(bKeyCancelLast == false)//"Esc"�˳���־δ��λ
								{
									if(bKeySelectX == false)//'X'δ���¼�ⰴ��
									{
										bKeySelect = true;

										g_pDirectSoundEffect->LoadWave(L"Resource\\sound\\se_cancel00.wav");//DirectSound������Чse_cancel00.wav
										g_pDirectSoundEffect->PlayWave(PlayState_Once);//DirectSound������Ч(Once)

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

								bKeySelectX = true;//'X'������
							}
							else
							{
								bKeyCancelLast = false;//"Esc"�˳���־δ��λ
								bKeySelectX = false;//'X'��δ����
							}
						}

						//Title01
						//"��Ϸѡ��"����ƫ��
						pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ�ƫ��λ�������ƶ�(8x/frame)
						for(int i = 0; i < 8; i++)
						{
							*(pfVertexX + i) += -8;
							if(*(pfVertexX + i) < 444.0f)//"��Ϸѡ��"��������(X:476.0f)
							{
								*(pfVertexX + i) = 444.0f;
							}
						}

						//"��Ϸѡ��"Alpha͸���ȱ仯
						pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ����0.0f,����1.0f,����10֡
						for(int i = 0; i < 8; i++)
						{
							if(*(pfVertexX + i) == 0.0f)//"��Ϸѡ��"�ַ�ֻ�е�һ��AlphaΪ0.0f����,����ַ�Alpha������(break)
							{
								*(pfVertexX + i) += 0.1f;
								break;
							}
							else
							{
								*(pfVertexX + i) += 0.1f;//�����ַ�Alphaֵ��Ϊ0.0f������0.1f
								if(*(pfVertexX + i) > 1.0f)//Alpha�޷�1.0f
								{
									*(pfVertexX + i) = 1.0f;
								}
							}
						}

						//"��Ϸѡ��"Alphaͨ���仯
						if(bAlphaFlag == true)//Alphaֵ����,����0.025f,����40֡
						{
							fAlphaValue += 0.025f;
							if(fAlphaValue > 1.0f)//Alphaֵ�޷�1.0f
							{
								fAlphaValue = 1.0f;
								bAlphaFlag = false;
							}
						}
						else//Alphaֵ�ݼ�,����0.025f,����40֡
						{
							fAlphaValue += -0.025f;
							if(fAlphaValue < 0.5f)//Alphaֵ�޷�0.5f
							{
								fAlphaValue = 0.5f;
								bAlphaFlag = true;
							}
						}

					}

				}
				else//������ѡ��(����״̬��־)
				{
					//"X"��״̬,"Z"��״̬
					if(bKeyCancelLast == false && bKeySelectZ == false)//"X"���ͷ�״̬,"Z"���ͷ�״̬
					{
						fKeyPressTime += fDeltaTime;//�������ʱ����
						if(fKeyPressTime > 0.12f)//��������ʱ��0.12s(KeyBoard����0.12s)
						{
							fKeyPressTime = 0.0f;
							bKeySelect = false;//����δѡ��
						}

						if(bKeyMoveFlag == false)//�����ƶ���־(Left2->Right4->Left2)
						{
							if(bKeyMoveDirect == false)//�����ƶ�����(false:Left/true:Right)
							{
								fKeyMoveX += 2.0f;//�����ƶ�Xƫ������2.0f
								if(fKeyMoveX >= 4.0f)//Left:2֡
								{
									fKeyMoveX = 4.0f;
									bKeyMoveDirect = true;//Right
								}
								if(fKeyMoveX == 0.0f)//Left:2֡
								{
									bKeyMoveFlag = true;
								}
							}
							else//Right:4֡
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
							fKeyMoveX = 0.0f;//����ƫ��λ��ԭ��
						}
					}
					else if(bKeyCancelLast == true && bKeySelectZ == false)//"X"������״̬,"Z"���ͷ�״̬
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
								//"��Ϸѡ��"����ƫ��
								pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ�ƫ��λ�������ƶ�(8x/frame)
								for(int i = 0; i < 8; i++)
								{
									if(i != g_nKeyNumber)//��ǰ"��Ϸѡ��"�ַ�ƫ�Ʋ���
									{
										*(pfVertexX + i) += 4;
										if(*(pfVertexX + i) > 640.0f)//"��Ϸѡ��"��������(X:640.0f)
										{
											*(pfVertexX + i) = 640.0f;
										}
									}
									else
									{
										if(nKeySelectFlashTimesCount >= 13)
										{
											*(pfVertexX + i) += 8;
											if(*(pfVertexX + i) > 640.0f)//"��Ϸѡ��"��������(X:640.0f)
											{
												*(pfVertexX + i) = 640.0f;
											}
										}
									}
								}

								//"��Ϸѡ��"Alpha͸���ȱ仯
								pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ����0.0f,����1.0f,����10֡(fAlphaValueArray�����ʱֵ��Ϊ1.0f)
								for(int i = 0; i < 8; i++)
								{
									if(i != g_nKeyNumber)//��ǰ"��Ϸѡ��"Alphaͨ������
									{
										*(pfVertexX + i) -= 0.05f;//Alphaͨ��ֵ����0.1f,����10֡
										if(*(pfVertexX + i) < 0.0f)
										{
											*(pfVertexX + i) = 0.0f;
										}
									}
									else
									{
										if(nKeySelectFlashTimesCount >= 13)
										{
											*(pfVertexX + i) -= 0.075f;//Alphaͨ��ֵ����0.1f,����10֡
											if(*(pfVertexX + i) < 0.0f)
											{
												*(pfVertexX + i) = 0.0f;
											}
										}
									}
								}

								if(nKeySelectFlashTimesCount >= 13)//������Ⱦ
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

									//"��Ϸѡ��"����ƫ��(�ָ���ʼ״̬)
									pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ��ָ���ʼ״̬
									for(int i = 0; i < 8; i++)
									{
									*(pfVertexX + i) = 540.0f + i * 16.0f;
									}

									//"��Ϸѡ��"Alpha͸���ȱ仯
									pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ���ָ���ʼ״̬
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
							//"��Ϸѡ��"����ƫ��
							pfVertexX = &fVertexXArray[0];//"��Ϸѡ��"�ַ�ƫ��λ�������ƶ�(8x/frame)
							for(int i = 0; i < 8; i++)
							{
								*(pfVertexX + i) += -8;
								if(*(pfVertexX + i) < 476.0f)//"��Ϸѡ��"��������(X:476.0f)
								{
									*(pfVertexX + i) = 476.0f;
								}
							}

							//"��Ϸѡ��"Alpha͸���ȱ仯
							pfVertexX = &fAlphaValueArray[0];//"��Ϸѡ��"Alphaͨ����0.0f,����1.0f,����10֡
							for(int i = 0; i < 8; i++)
							{
								if(*(pfVertexX + i) == 0.0f)//"��Ϸѡ��"�ַ�ֻ�е�һ��AlphaΪ0.0f����,����ַ�Alpha������(break)
								{
									*(pfVertexX + i) += 0.0625f;//0.1f
									break;
								}
								else
								{
									*(pfVertexX + i) += 0.0625f;//�����ַ�Alphaֵ��Ϊ0.0f������0.1f
									if(*(pfVertexX + i) > 1.0f)//Alpha�޷�1.0f
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
							bKeySelect = false;//����δѡ��
							g_bKeySelectTitleChange = false;
							g_nKeySelectState = 0;
							}
						}
						
					}
					else if(bKeyCancelLast == false && bKeySelectZ == true)//"X"���ͷ�״̬,"Z"������״̬
					{
						fKeyPressTime += fDeltaTime;//�������ʱ����
						if(fKeyPressTime > 0.12f)//��������ʱ��0.12s(KeyBoard����0.12s)
						{
							fKeyPressTime = 0.0f;
							bKeySelect = false;//����δѡ��
						}
					}
					else
					{
						fKeyPressTime += fDeltaTime;//�������ʱ����
						if(fKeyPressTime > 0.12f)//��������ʱ��0.12s(KeyBoard����0.12s)
						{
							fKeyPressTime = 0.0f;
							bKeySelect = false;//����δѡ��
							bKeyCancelLast = false;
							bKeySelectZ = false;
						}
					}

				}
			}
		}

		//���ö��㻺������
		Vertex*pVertices = NULL;
		g_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

		if(g_nKeySelectState == 0)//Title00,State0
		{
			//Title00��������������
			pVertices[0] = Vertex(0.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, 0.0f);
			pVertices[1] = Vertex(640.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), 0.0f);
			pVertices[2] = Vertex(640.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), (480.0f / 512.0f));
			pVertices[3] = Vertex(0.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, (480.0f / 512.0f));

			//"Copyright"����������
			pVertices[4] = Vertex(22.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), 0.0f);
			pVertices[5] = Vertex(227.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), 0.0f);
			pVertices[6] = Vertex(227.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), (16.0f / 256.0f));
			pVertices[7] = Vertex(22.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), (16.0f / 256.0f));

			//"Ver1.00a"����������
			pVertices[8] = Vertex(304.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 0.0f);
			pVertices[9] = Vertex(368.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 0.0f);
			pVertices[10] = Vertex(368.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 1.0f);
			pVertices[11] = Vertex(304.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 1.0f);

			//"GameStart"����������
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

			//"ExtraStart"����������
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

			//"PracticeStart"����������
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

			//"Replay"����������
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

			//"PlayerData"����������
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

			//"MusicRoom"����������
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

			//"Option"����������
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

			//"Quit"����������
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

			if(bKeySelectZ == false)//"Z"���ͷ�״̬
			{
				if(g_nKeyNumber == 0)//"GameStart"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[0] + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), 0.0f, 0.0f);
					pVertices[45] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), (128.0f / 512.0f), 0.0f);
					pVertices[46] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), (128.0f / 512.0f), (32.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[0] + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), 0.0f, (32.0f / 512.0f));
				}
				else if(g_nKeyNumber == 1)//"ExtraStart"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[1] + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), 0.0f, (32.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), (128.0f / 512.0f), (32.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), (128.0f / 512.0f), (64.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[1] + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), 0.0f, (64.0f / 512.0f));
				}
				else if(g_nKeyNumber == 2)//"PracticeStart"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[2] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), 0.0f, (64.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), (128.0f / 512.0f), (64.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), (128.0f / 512.0f), (96.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[2] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), 0.0f, (96.0f / 512.0f));
				}
				else if(g_nKeyNumber == 3)//"Replay"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[3] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), 0.0f, (96.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), (128.0f / 512.0f), (96.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), (128.0f / 512.0f), (128.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[3] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), 0.0f, (128.0f / 512.0f));
				}
				else if(g_nKeyNumber == 4)//"PlayerData"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[4] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), 0.0f, (128.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), (128.0f / 512.0f), (128.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), (128.0f / 512.0f), (160.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[4] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), 0.0f, (160.0f / 512.0f));
				}
				else if(g_nKeyNumber == 5)//"MusicRoom"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[5] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), 0.0f, (160.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), (128.0f / 512.0f), (160.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), (128.0f / 512.0f), (192.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[5] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), 0.0f, (192.0f / 512.0f));
				}
				else if(g_nKeyNumber == 6)//"Option"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[6] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), 0.0f, (192.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), (128.0f / 512.0f), (192.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), (128.0f / 512.0f), (224.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[6] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), 0.0f, (224.0f / 512.0f));
				}
				else if(g_nKeyNumber == 7)//"Quit"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[7] + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), 0.0f, (224.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), (128.0f / 512.0f), (224.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), (128.0f / 512.0f), (256.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[7] + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), 0.0f, (256.0f / 512.0f));
				}
			}
			else//"Z"������
			{
				if(g_nKeyNumber == 0)//"GameStart"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[0] + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), 0.0f, 0.0f);
					pVertices[45] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), 0.0f);
					pVertices[46] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (32.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[0] + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), 0.0f, (32.0f / 512.0f));
				}
				else if(g_nKeyNumber == 1)//"ExtraStart"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[1] + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), 0.0f, (32.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (32.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (64.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[1] + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), 0.0f, (64.0f / 512.0f));
				}
				else if(g_nKeyNumber == 2)//"PracticeStart"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[2] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), 0.0f, (64.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (64.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (96.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[2] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), 0.0f, (96.0f / 512.0f));
				}
				else if(g_nKeyNumber == 3)//"Replay"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[3] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), 0.0f, (96.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (96.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (128.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[3] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), 0.0f, (128.0f / 512.0f));
				}
				else if(g_nKeyNumber == 4)//"PlayerData"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[4] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), 0.0f, (128.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (128.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (160.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[4] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), 0.0f, (160.0f / 512.0f));
				}
				else if(g_nKeyNumber == 5)//"MusicRoom"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[5] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), 0.0f, (160.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (160.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (192.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[5] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), 0.0f, (192.0f / 512.0f));
				}
				else if(g_nKeyNumber == 6)//"Option"ѡ������������
				{
					pVertices[44] = Vertex((fVertexXArray[6] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), 0.0f, (192.0f / 512.0f));
					pVertices[45] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (192.0f / 512.0f));
					pVertices[46] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (224.0f / 512.0f));
					pVertices[47] = Vertex((fVertexXArray[6] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), 0.0f, (224.0f / 512.0f));
				}
				else if(g_nKeyNumber == 7)//"Quit"ѡ������������
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
				//Title00��������������
				pVertices[0] = Vertex(0.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, 0.0f);
				pVertices[1] = Vertex(640.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), 0.0f);
				pVertices[2] = Vertex(640.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), (480.0f / 512.0f));
				pVertices[3] = Vertex(0.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, (480.0f / 512.0f));

				//"Copyright"����������
				pVertices[4] = Vertex(22.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), 0.0f);
				pVertices[5] = Vertex(227.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), 0.0f);
				pVertices[6] = Vertex(227.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), (16.0f / 256.0f));
				pVertices[7] = Vertex(22.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), (16.0f / 256.0f));

				//"Ver1.00a"����������
				pVertices[8] = Vertex(304.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 0.0f);
				pVertices[9] = Vertex(368.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 0.0f);
				pVertices[10] = Vertex(368.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 1.0f);
				pVertices[11] = Vertex(304.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 1.0f);

				if(g_bKeySelectTitleChange == false)
				{
					//"BGM Volume"����������
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

					//"SE Volume"����������
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

					//"Key Config"����������
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

					//"Default"����������
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

					//"Quit"����������
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
						if(g_nKeyNumber1 == 0)//"BGM Volume"ѡ������������
						{
							pVertices[32] = Vertex((fVertexXArray[0] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), 0.0f, (256.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), (128.0f / 512.0f), (256.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), (128.0f / 512.0f), (288.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[0] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), 0.0f, (288.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 1)//"SE Volume"ѡ������������
						{
							pVertices[32] = Vertex((fVertexXArray[1] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), 0.0f, (288.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), (128.0f / 512.0f), (288.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), (128.0f / 512.0f), (320.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[1] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), 0.0f, (320.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 2)//"Key Config"ѡ������������
						{
							pVertices[32] = Vertex((fVertexXArray[2] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), 0.0f, (352.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), (128.0f / 512.0f), (352.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), (128.0f / 512.0f), (384.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[2] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), 0.0f, (384.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 3)//"Default"ѡ������������
						{
							pVertices[32] = Vertex((fVertexXArray[3] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), 0.0f, (320.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), (128.0f / 512.0f), (320.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), (128.0f / 512.0f), (352.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[3] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), 0.0f, (352.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 4)//"Quit"ѡ������������
						{
							pVertices[32] = Vertex((fVertexXArray[4] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), 0.0f, (224.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), (128.0f / 512.0f), (224.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), (128.0f / 512.0f), (256.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[4] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), 0.0f, (256.0f / 512.0f));
						}
					}
					else
					{
						if(g_nKeyNumber1 == 0)//"BGM Volume"ѡ������������
						{
							pVertices[32] = Vertex((fVertexXArray[0] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), 0.0f, (256.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (256.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (288.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[0] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), 0.0f, (288.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 1)//"SE Volume"ѡ������������
						{
							pVertices[32] = Vertex((fVertexXArray[1] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), 0.0f, (288.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (288.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (320.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[1] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), 0.0f, (320.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 2)//"Key Config"ѡ������������
						{
							pVertices[32] = Vertex((fVertexXArray[2] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), 0.0f, (352.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (352.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (384.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[2] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), 0.0f, (384.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 3)//"Default"ѡ������������
						{
							pVertices[32] = Vertex((fVertexXArray[3] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), 0.0f, (320.0f / 512.0f));
							pVertices[33] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (320.0f / 512.0f));
							pVertices[34] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (352.0f / 512.0f));
							pVertices[35] = Vertex((fVertexXArray[3] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), 0.0f, (352.0f / 512.0f));
						}
						else if(g_nKeyNumber1 == 4)//"Quit"ѡ������������
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
					//"GameStart"����������
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

					//"ExtraStart"����������
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

					//"PracticeStart"����������
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

					//"Replay"����������
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

					//"PlayerData"����������
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

					//"MusicRoom"����������
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

					//"Option"����������
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

					//"Quit"����������
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

					if(bKeySelectZ == false)//"Z"���ͷ�״̬
					{
						if(g_nKeyNumber == 0)//"GameStart"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[0] + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), 0.0f, 0.0f);
							pVertices[45] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), (128.0f / 512.0f), 0.0f);
							pVertices[46] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), (128.0f / 512.0f), (32.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[0] + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[0]), 0.0f, (32.0f / 512.0f));
						}
						else if(g_nKeyNumber == 1)//"ExtraStart"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[1] + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), 0.0f, (32.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), (128.0f / 512.0f), (32.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), (128.0f / 512.0f), (64.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[1] + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[1]), 0.0f, (64.0f / 512.0f));
						}
						else if(g_nKeyNumber == 2)//"PracticeStart"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[2] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), 0.0f, (64.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), (128.0f / 512.0f), (64.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), (128.0f / 512.0f), (96.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[2] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[2]), 0.0f, (96.0f / 512.0f));
						}
						else if(g_nKeyNumber == 3)//"Replay"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[3] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), 0.0f, (96.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), (128.0f / 512.0f), (96.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), (128.0f / 512.0f), (128.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[3] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[3]), 0.0f, (128.0f / 512.0f));
						}
						else if(g_nKeyNumber == 4)//"PlayerData"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[4] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), 0.0f, (128.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), (128.0f / 512.0f), (128.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), (128.0f / 512.0f), (160.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[4] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[4]), 0.0f, (160.0f / 512.0f));
						}
						else if(g_nKeyNumber == 5)//"MusicRoom"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[5] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), 0.0f, (160.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), (128.0f / 512.0f), (160.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), (128.0f / 512.0f), (192.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[5] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[5]), 0.0f, (192.0f / 512.0f));
						}
						else if(g_nKeyNumber == 6)//"Option"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[6] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), 0.0f, (192.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), (128.0f / 512.0f), (192.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), (128.0f / 512.0f), (224.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[6] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[6]), 0.0f, (224.0f / 512.0f));
						}
						else if(g_nKeyNumber == 7)//"Quit"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[7] + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), 0.0f, (224.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), (128.0f / 512.0f), (224.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[7] + 128 + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), (128.0f / 512.0f), (256.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[7] + fKeyMoveX), 456.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValue*fAlphaValueArray[7]), 0.0f, (256.0f / 512.0f));
						}
					}
					else//"Z"������
					{
						if(g_nKeyNumber == 0)//"GameStart"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[0] + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), 0.0f, 0.0f);
							pVertices[45] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 256.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), 0.0f);
							pVertices[46] = Vertex((fVertexXArray[0] + 128 + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), (128.0f / 512.0f), (32.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[0] + fKeyMoveX), 288.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[0]), 0.0f, (32.0f / 512.0f));
						}
						else if(g_nKeyNumber == 1)//"ExtraStart"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[1] + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), 0.0f, (32.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 280.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (32.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[1] + 128 + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), (128.0f / 512.0f), (64.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[1] + fKeyMoveX), 312.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[1]), 0.0f, (64.0f / 512.0f));
						}
						else if(g_nKeyNumber == 2)//"PracticeStart"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[2] + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), 0.0f, (64.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 304.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (64.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[2] + 128 + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), (128.0f / 512.0f), (96.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[2] + fKeyMoveX), 336.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[2]), 0.0f, (96.0f / 512.0f));
						}
						else if(g_nKeyNumber == 3)//"Replay"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[3] + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), 0.0f, (96.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 328.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (96.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[3] + 128 + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), (128.0f / 512.0f), (128.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[3] + fKeyMoveX), 360.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[3]), 0.0f, (128.0f / 512.0f));
						}
						else if(g_nKeyNumber == 4)//"PlayerData"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[4] + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), 0.0f, (128.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 352.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (128.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[4] + 128 + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), (128.0f / 512.0f), (160.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[4] + fKeyMoveX), 384.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[4]), 0.0f, (160.0f / 512.0f));
						}
						else if(g_nKeyNumber == 5)//"MusicRoom"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[5] + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), 0.0f, (160.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 376.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (160.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[5] + 128 + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), (128.0f / 512.0f), (192.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[5] + fKeyMoveX), 408.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[5]), 0.0f, (192.0f / 512.0f));
						}
						else if(g_nKeyNumber == 6)//"Option"ѡ������������
						{
							pVertices[44] = Vertex((fVertexXArray[6] + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), 0.0f, (192.0f / 512.0f));
							pVertices[45] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 400.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (192.0f / 512.0f));
							pVertices[46] = Vertex((fVertexXArray[6] + 128 + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), (128.0f / 512.0f), (224.0f / 512.0f));
							pVertices[47] = Vertex((fVertexXArray[6] + fKeyMoveX), 432.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlphaValueArray[6]), 0.0f, (224.0f / 512.0f));
						}
						else if(g_nKeyNumber == 7)//"Quit"ѡ������������
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

		//����������������
		WORD*pIndices = NULL;
		g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);

		//Title00����������������
		pIndices[0] = 0;
		pIndices[1] = 1;
		pIndices[2] = 2;

		pIndices[3] = 0;
		pIndices[4] = 2;
		pIndices[5] = 3;

		//"Copyright"������������
		pIndices[6] = 4;
		pIndices[7] = 5;
		pIndices[8] = 6;

		pIndices[9] = 4;
		pIndices[10] = 6;
		pIndices[11] = 7;

		//"Ver1.00a"������������
		pIndices[12] = 8;
		pIndices[13] = 9;
		pIndices[14] = 10;

		pIndices[15] = 8;
		pIndices[16] = 10;
		pIndices[17] = 11;

		//"GameStart"������������
		pIndices[18] = 12;
		pIndices[19] = 13;
		pIndices[20] = 14;

		pIndices[21] = 12;
		pIndices[22] = 14;
		pIndices[23] = 15;

		//"ExtraStart"������������
		pIndices[24] = 16;
		pIndices[25] = 17;
		pIndices[26] = 18;

		pIndices[27] = 16;
		pIndices[28] = 18;
		pIndices[29] = 19;

		//"PracticeStart"������������
		pIndices[30] = 20;
		pIndices[31] = 21;
		pIndices[32] = 22;

		pIndices[33] = 20;
		pIndices[34] = 22;
		pIndices[35] = 23;

		//"Replay"������������
		pIndices[36] = 24;
		pIndices[37] = 25;
		pIndices[38] = 26;

		pIndices[39] = 24;
		pIndices[40] = 26;
		pIndices[41] = 27;

		//"PlayerData"������������
		pIndices[42] = 28;
		pIndices[43] = 29;
		pIndices[44] = 30;

		pIndices[45] = 28;
		pIndices[46] = 30;
		pIndices[47] = 31;

		//"MusicRoom"������������
		pIndices[48] = 32;
		pIndices[49] = 33;
		pIndices[50] = 34;

		pIndices[51] = 32;
		pIndices[52] = 34;
		pIndices[53] = 35;

		//"Option"������������
		pIndices[54] = 36;
		pIndices[55] = 37;
		pIndices[56] = 38;

		pIndices[57] = 36;
		pIndices[58] = 38;
		pIndices[59] = 39;

		//"Quit"������������
		pIndices[60] = 40;
		pIndices[61] = 41;
		pIndices[62] = 42;

		pIndices[63] = 40;
		pIndices[64] = 42;
		pIndices[65] = 43;

		//��Ϸѡ��ѡ��������������
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
** Purpose:  Direct3D��Ⱦ
** Para: HWND hWnd(���ھ��)
** Para: float fDeltaTime(ʱ����)
*/
void Direct3D_Render(HWND hWnd, float fDeltaTime)
{
	RECT FormatRect;
	static float fTimeSum = 0.0f;

	GetClientRect(hWnd, &FormatRect);//��ȡ�û�����(UserArea)

	//Step1 Clear���
	g_pD3D9Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//Step2 ��ʼ��Ⱦ
	g_pD3D9Device->BeginScene();

	//Step3 ��Ⱦ����
	fTimeSum += fDeltaTime;

	if(fTimeSum < 5.0f)//Loading
	{
		SceneSig_Render(fTimeSum);//Sig������Ⱦ
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
			//���Ʊ���
			g_pD3D9Device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex));
			g_pD3D9Device->SetFVF(D3DFVF_VERTEX);
			g_pD3D9Device->SetIndices(g_pIndexBuffer);
			g_pD3D9Device->SetTexture(0, g_pTextureTitle);
			g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

			//��������
			g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
			g_pD3D9Device->SetTexture(0, g_pTextureLogo);
			g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 4, 0, 2);
			g_pD3D9Device->SetTexture(0, g_pTextureVersion);
			g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8, 0, 4, 0, 2);

			//������Ϸѡ��
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

			//���ƾ��鶯��
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
				//���Ʊ���
				g_pD3D9Device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex));
				g_pD3D9Device->SetFVF(D3DFVF_VERTEX);
				g_pD3D9Device->SetIndices(g_pIndexBuffer);
				g_pD3D9Device->SetTexture(0, g_pTextureTitle);
				g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

				//��������
				g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
				g_pD3D9Device->SetTexture(0, g_pTextureLogo);
				g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 4, 0, 2);
				g_pD3D9Device->SetTexture(0, g_pTextureVersion);
				g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8, 0, 4, 0, 2);

				if(g_bKeySelectTitleChange == false)
				{
					//������Ϸѡ��
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
					//������Ϸѡ��
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
				
				//���ƾ��鶯��
				g_nTitleFlagNow = g_pAnimation->RenderTitle(fDeltaTime);
			}
		}

		g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);*/
	}

	//��������
	int CharCount = swprintf_s(g_sFPS, 20, _T("%0.1ffps"), Direct3D_FPS());
	g_pFontFPS->DrawText(NULL, g_sFPS, CharCount, &FormatRect, DT_BOTTOM | DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//fps֡����

	//g_pFontAdapter->DrawText(NULL, g_sAdapterName, -1, &FormatRect, DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));//Adapter�ͺ�

	//Step4 ������Ⱦ
	g_pD3D9Device->EndScene();

	//Step5 Display��ʾ
	g_pD3D9Device->Present(NULL, NULL, NULL, NULL);
}

/*
** Function:  Direct3D_ClearUp()
** Purpose:  Direct3D�ͷ���Դ
*/
void Direct3D_ClearUp(void)
{
	SAFE_DELETE(g_pSnow);//�ͷ�Particle����
	SAFE_DELETE(g_pAnimation);//�ͷ�Animation����
	SAFE_DELETE(g_pDirectInput);//�ͷ�DirectInput�����豸
	SAFE_DELETE(g_pDirectSound);//�ͷ�DirectSound����豸
	SAFE_DELETE(g_pDirectSoundEffect);//�ͷ�DirectSoundEffect����豸
	SAFE_RELEASE(g_pFontFPS);//�ͷ�����(֡����fps)
	SAFE_RELEASE(g_pFontAdapter);//�ͷ�����(�Կ��ͺ�Adapter)
	SAFE_RELEASE(g_pTexture);//�ͷ�����(Texture)
	SAFE_RELEASE(g_pTextureTitle);//�ͷ�����(Texture:����00)
	SAFE_RELEASE(g_pTextureLoading);//�ͷ�����(Texture)
	SAFE_RELEASE(g_pTextureLogo);//�ͷ�����(Texture:����00logo)
	SAFE_RELEASE(g_pTextureVersion);//�ͷ�����(Texture:����00Version)
	SAFE_RELEASE(g_pTextureTitleText);//�ͷ�����(Texture:����01Text)
	SAFE_RELEASE(g_pVertexBuffer);//�ͷŶ��㻺��(VertexBuffer)
	SAFE_RELEASE(g_pIndexBuffer);//�ͷ���������(IndexBuffer)
	SAFE_RELEASE(g_pD3D9Device);//�ͷ��豸(D3D9Device)
}

/*
** Function:  Direct3D_FPS()
** Purpose:  Direct3D����fps
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
** Purpose:  Direct3D����任
*/
void MatrixSet(void)
{
	//WorldSpace��������ϵ
	D3DXMATRIX MatrixWorld, Rx, Ry, Rz;
	float Sx = 1.0f, Sy = 1.0f, Sz = 1.0f;
	D3DXMatrixIdentity(&MatrixWorld);//��λ������
	D3DXMatrixRotationX(&Rx, 0.0f);
	D3DXMatrixRotationY(&Ry, 0.0f);
	D3DXMatrixRotationZ(&Rz, 0.0f);
	D3DXMatrixScaling(&MatrixWorld, Sx, Sy, Sz);
	D3DXMatrixTranslation(&MatrixWorld, 0.0f, 0.0f, 0.0f);
	MatrixWorld = Rx*Ry*Rz*MatrixWorld;
	g_pD3D9Device->SetTransform(D3DTS_WORLD, &MatrixWorld);

	//ViewSpaceȡ������ϵ
	D3DXMATRIX MatrixView;
	D3DXVECTOR3 Vector_Eye(0.0f, 15.0f, -25.0f);
	D3DXVECTOR3 Vector_At(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Vector_Up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&MatrixView, &Vector_Eye, &Vector_At, &Vector_Up);
	g_pD3D9Device->SetTransform(D3DTS_VIEW, &MatrixView);

	//PerspectiveProjectionͶӰ����ϵ
	D3DXMATRIX MatrixProject;
	D3DXMatrixPerspectiveFovLH(&MatrixProject, D3DX_PI * 0.5f, (float)USER_SCREENWIDTH / (float)USER_SCREENHEIGHT, 1.0f, 1000.0f);
	g_pD3D9Device->SetTransform(D3DTS_PROJECTION, &MatrixProject);

	//ViewPortTransform�ӿ�����ϵ
	D3DVIEWPORT9 ViewPort;
	ViewPort.X = 0;
	ViewPort.Y = 0;
	ViewPort.Width = USER_SCREENWIDTH;
	ViewPort.Height = USER_SCREENHEIGHT;
	ViewPort.MinZ = 0.0f;
	ViewPort.MaxZ = 1.0f;
	g_pD3D9Device->SetViewport(&ViewPort);

}