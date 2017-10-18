#include"Common.h"
#include "Include.h"
#include "Particle.h"


/*
** Function:	 Particle()
** Purpose: Particle构造函数
*/
Particle::Particle()
{
	m_pD3D9Device = NULL;
	m_pTexture = NULL;
	m_pSprite = NULL;
	ZeroMemory(&m_Snow, sizeof(SnowParticle)*PARTICLE_NUMBER);
	m_nPosX = 0;
	m_nPosY = 0;
	m_nWidth = USER_SCREENWIDTH;
	m_nHeight = USER_SCREENHEIGHT;
}

/*
** Function:	 ~Particle()
** Purpose: Particle析构函数
*/
Particle::~Particle()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pSprite);
}

/*
** Function:	 Particle(LPDIRECT3DDEVICE9 pD3D9Device, int X, int Y, int Width, int Height)
** Purpose: Particle构造函数(重载+1)
** Para: int X(粒子区域X坐标)
** Para: int Y(粒子区域Y坐标)
** Para: int Width(粒子区域Width宽度)
** Para: int Height(粒子区域Height高度)
*/
Particle::Particle(LPDIRECT3DDEVICE9 pD3D9Device, int X, int Y, int Width, int Height)
{
	m_pD3D9Device = pD3D9Device;
	m_pTexture = NULL;
	m_pSprite = NULL;
	ZeroMemory(&m_Snow, sizeof(SnowParticle)*PARTICLE_NUMBER);
	m_nPosX = X;
	m_nPosY = Y;
	m_nWidth = Width;
	m_nHeight = Height;
}

/*
** Function: Particle::Initialize(int X, int Y, int Width, int Height, LPCWSTR lpSrcFile)
** Para: LPCWSTR lpSrcFile(纹理文件路径)
** Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
*/
HRESULT Particle::Initialize(LPCWSTR lpSrcFile)
{
	HRESULT Result;

	srand(GetTickCount());//设置随机数种子

	for(int i = 0; i < PARTICLE_NUMBER; i++)
	{
		m_Snow[i].X = (float)(m_nPosX + rand() % m_nWidth);
		m_Snow[i].Y = (float)(m_nPosY + rand() % m_nHeight - m_nHeight);
		m_Snow[i].Scale = (float)(0.2f + ((rand() % 80)*1.0f / 100.0f)*1.0f);
		m_Snow[i].Rotate = (float)(((rand() % 100)*1.0f / 600.0f)*D3DX_PI);
		m_Snow[i].Alpha = (float)(0.5f + ((rand() % 100)*1.0f / 200.0f)*1.0f);
		m_Snow[i].FallSpeed = (float)(m_nHeight*1.0f / 2.0f + ((rand() % 100)*1.0f / 200.0f)*m_nHeight);
		m_Snow[i].MoveSpeed = (float)(((rand() % 100)*1.0f / 500.0f)*m_nWidth);
		m_Snow[i].MoveDirect = rand() % 2;
		m_Snow[i].RotateSpeed = (float)(1.0f*D3DX_PI + ((rand() % 100)*1.0f / 200.0f)*D3DX_PI);
		m_Snow[i].RotateDirect = rand() % 2;
	}

	Result = D3DXCreateTextureFromFile(m_pD3D9Device, lpSrcFile, &m_pTexture);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	Result = D3DXCreateSprite(m_pD3D9Device, &m_pSprite);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	return S_OK;
}

/*
** Function: Particle::Update(float fDeltaTime)
** Purpose: Particle粒子状态刷新
** Para: float fDeltaTime(时间间隙)
*/
void Particle::Update(float fDeltaTime)
{
	for(int i = 0; i < PARTICLE_NUMBER; i++)
	{
		m_Snow[i].Y += m_Snow[i].FallSpeed*fDeltaTime;
		if(m_Snow[i].Y > (m_nPosY + m_nHeight))
		{
			m_Snow[i].X = (float)(m_nPosX + rand() % m_nWidth);
			m_Snow[i].Y = (float)(m_nPosY + rand() % m_nHeight);
			m_Snow[i].Scale = (float)(0.2f + ((rand() % 80)*1.0f / 100.0f)*1.0f);
			m_Snow[i].Rotate = (float)(((rand() % 100)*1.0f / 600.0f)*D3DX_PI);
			m_Snow[i].Alpha = (float)(0.5f + ((rand() % 100)*1.0f / 200.0f)*1.0f);
			m_Snow[i].FallSpeed = (float)(m_nHeight*1.0f / 2.0f + ((rand() % 100)*1.0f / 200.0f)*m_nHeight);
			m_Snow[i].MoveSpeed = (float)(((rand() % 100)*1.0f / 500.0f)*m_nWidth);
			m_Snow[i].MoveDirect = rand() % 2;
			m_Snow[i].RotateSpeed = (float)(1.0f*D3DX_PI + ((rand() % 100)*1.0f / 200.0f)*D3DX_PI);
			m_Snow[i].RotateDirect = rand() % 2;
		}

		if(m_Snow[i].Y > (m_nPosY + m_nHeight * 0.8f))
		{
			m_Snow[i].Alpha -= 0.05f;
			if(m_Snow[i].Alpha <= 0)
			{
				m_Snow[i].Alpha = 0;
			}
		}

		if(m_Snow[i].MoveDirect == 0)
		{
			m_Snow[i].X += m_Snow[i].MoveSpeed*fDeltaTime;
		}
		else
		{
			m_Snow[i].X += -m_Snow[i].MoveSpeed*fDeltaTime;
		}
		
		if(m_Snow[i].RotateDirect == 0)
		{
			m_Snow[i].Rotate += m_Snow[i].RotateSpeed*fDeltaTime;
		}
		else
		{
			m_Snow[i].Rotate -= m_Snow[i].RotateSpeed*fDeltaTime;
		}
	}
}

/*
** Function: Particle::Render(void)
** Purpose: Particle粒子绘制
*/
void Particle::Render(void)
{
	RECT Rect;
	D3DXMATRIX MatrixOriginal;
	D3DXMATRIX MatrixNow, MatrixScale, MatrixRotate, MatrixTranslate, MatrixInvrsT;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	//粒子渲染模式
	m_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE4X);
	m_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	//粒子绘制
	SetRect(&Rect, 0, 66, 32, 94);
	m_pSprite->GetTransform(&MatrixOriginal);

	for(int i = 0; i < PARTICLE_NUMBER; i++)
	{
		m_pSprite->SetTransform(&MatrixOriginal);
		MatrixNow = MatrixOriginal;
		D3DXMatrixScaling(&MatrixScale, m_Snow[i].Scale, m_Snow[i].Scale, 1.0f);
		D3DXMatrixRotationZ(&MatrixRotate, m_Snow[i].Rotate);
		D3DXMatrixTranslation(&MatrixTranslate, m_Snow[i].X, m_Snow[i].Y, 0.0f);
		D3DXMatrixInverse(&MatrixInvrsT, NULL, &MatrixTranslate);
		MatrixNow = MatrixNow*MatrixInvrsT*MatrixScale*MatrixRotate*MatrixTranslate;
		m_pSprite->SetTransform(&MatrixNow);
		if(m_Snow[i].Y >= m_nPosY)
		{
			m_pSprite->Draw(m_pTexture, &Rect, &D3DXVECTOR3(16.0f, 14.0f, 0.0f), &D3DXVECTOR3(m_Snow[i].X, m_Snow[i].Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Snow[i].Alpha));//m_Snow[i].Alpha
		}
	}
	m_pSprite->SetTransform(&MatrixOriginal);

	m_pSprite->End();
}