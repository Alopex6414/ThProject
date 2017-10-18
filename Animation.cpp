#include"Common.h"
#include "Include.h"
#include "Animation.h"


/*
** Function:	 Animation()
** Purpose: Animation构造函数
*/
Animation::Animation()
{
	m_pD3D9Device = NULL;
	m_pTextureTitle = NULL;
	m_pSprite = NULL;
}

/*
** Function:	 ~Animation()
** Purpose: Animation析构函数
*/
Animation::~Animation()
{
	SAFE_RELEASE(m_pTextureTitle);
	SAFE_RELEASE(m_pSprite);
}

/*
** Function:	 Animation()
** Purpose: Animation构造函数(重载+1)
*/
Animation::Animation(LPDIRECT3DDEVICE9 pD3D9Device)
{
	m_pD3D9Device = pD3D9Device;
	m_pTextureTitle = NULL;
	m_pSprite = NULL;
}

/*
** Function:	 Initialize(LPCWSTR lpSrcFile)
** Purpose: Animation初始化函数
** Para: LPCWSTR lpSrcFile(纹理文件路径)
** Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
*/
HRESULT Animation::Initialize(LPCWSTR lpSrcFile)
{
	HRESULT Result;

	//Animation加载纹理(标题)
	Result = D3DXCreateTextureFromFile(m_pD3D9Device, lpSrcFile, &m_pTextureTitle);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//Animation初始化Sprite
	Result = D3DXCreateSprite(m_pD3D9Device, &m_pSprite);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	return S_OK;
}

/*
** Function:	 UpdateTitle(float fDeltaTime)
** Purpose: Animation标题动画更新
** Para: float fDeltaTime(时间间隙)
*/
void Animation::UpdateTitle(float fDeltaTime)
{

}

/*
** Function:	 RenderTitle(float fDeltaTime)
** Purpose: Animation标题动画渲染
** Para: float fDeltaTime(时间间隙)
*/
int Animation::RenderTitle(float fDeltaTime)
{
	RECT Rect;
	D3DXMATRIX MatrixOriginal;
	D3DXMATRIX MatrixNow, MatrixScale, MatrixRotate, MatrixTranslate, MatrixInvrsT;
	D3DXMATRIX MatrixRotateX;
	static int FlagNow = 0;
	static int FlagCount = 0;
	static float Rotate = -D3DX_PI * 5.0f / 6.0f;
	static float Scale = 0.5f;
	static float Rotate1 = -D3DX_PI * 5.0f / 6.0f;
	static float Scale1 = 0.5f;
	static float Rotate2 = -D3DX_PI * 5.0f / 6.0f;
	static float Scale2 = 0.5f;
	static float Rotate3 = -D3DX_PI * 5.0f / 6.0f;
	static float Scale3 = 0.5f;
	static float Rotate4 = -D3DX_PI * 5.0f / 6.0f;
	static float Scale4 = 0.5f;
	static float Rotate5 = 0.0f;
	static float RotateText = -D3DX_PI * 5.0f / 12.0f;
	static float ScaleTextW = 0.2f;
	static float ScaleTextH = 0.0f;
	static float RotateAngle = -D3DX_PI * 1.0f;
	static float RotateAngle1 = -D3DX_PI * 3.0f / 2.0f;
	static float RotateAngle2 = -D3DX_PI * 1.0f / 3.0f;
	static float RotateAngle3 = -D3DX_PI * 1.0f / 2.0f;
	static float RotateAngle4 = -D3DX_PI * 1.0f;
	float RotateX = 0.0f;
	float RotateX1 = 0.0f;
	float RotateX2 = 0.0f;
	float RotateX3 = 0.0f;
	float RotateX4 = 0.0f;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pSprite->GetTransform(&MatrixOriginal);

	if(FlagNow == 0)
	{
		if(Rotate < -D3DX_PI * 2.0f / 6.0f)
		{
			Rotate += D3DX_PI / 20.0f;
			Scale += 0.1f;
			if(Rotate > -D3DX_PI * 2.0f / 6.0f)
			{
				Rotate = -D3DX_PI * 2.0f / 6.0f;
			}
			if(Scale > 1.5f)
			{
				Scale = 1.5f;
			}
		}
		else if(Rotate < 0.0f)
		{
			Rotate += D3DX_PI / 30.0f;
			Scale -= 0.05f;
			if(Rotate > 0.0f)
			{
				Rotate = 0.0f;
			}
			if(Scale < 1.0f)
			{
				Scale = 1.0f;
			}
		}
		else
		{
			Rotate = 0.0f;
			Scale = 1.0f;
			FlagNow = 1;
		}
	}
	else if(FlagNow == 1)
	{
		if(Rotate1 < -D3DX_PI * 2.0f / 6.0f)
		{
			Rotate1 += D3DX_PI / 20.0f;
			Scale1 += 0.1f;
			if(Rotate1 > -D3DX_PI * 2.0f / 6.0f)
			{
				Rotate1 = -D3DX_PI * 2.0f / 6.0f;
			}
			if(Scale1 > 1.5f)
			{
				Scale1 = 1.5f;
			}
		}
		else if(Rotate1 < 0.0f)
		{
			Rotate1 += D3DX_PI / 30.0f;
			Scale1 -= 0.05f;
			if(Rotate1 > 0.0f)
			{
				Rotate1 = 0.0f;
			}
			if(Scale1 < 1.0f)
			{
				Scale1 = 1.0f;
			}
		}
		else
		{
			Rotate1 = 0.0f;
			Scale1 = 1.0f;
			FlagNow = 2;
		}
	}
	else if(FlagNow == 2)
	{
		if(Rotate2 < -D3DX_PI * 2.0f / 6.0f)
		{
			Rotate2 += D3DX_PI / 20.0f;
			Scale2 += 0.1f;
			if(Rotate2 > -D3DX_PI * 2.0f / 6.0f)
			{
				Rotate2 = -D3DX_PI * 2.0f / 6.0f;
			}
			if(Scale2 > 1.5f)
			{
				Scale2 = 1.5f;
			}
		}
		else if(Rotate2 < 0.0f)
		{
			Rotate2 += D3DX_PI / 30.0f;
			Scale2 -= 0.05f;
			if(Rotate2 > 0.0f)
			{
				Rotate2 = 0.0f;
			}
			if(Scale2 < 1.0f)
			{
				Scale2 = 1.0f;
			}
		}
		else
		{
			Rotate2 = 0.0f;
			Scale2 = 1.0f;
			FlagNow = 3;
		}
	}
	else if(FlagNow == 3)
	{
		if(Rotate3 < -D3DX_PI * 2.0f / 6.0f)
		{
			Rotate3 += D3DX_PI / 20.0f;
			Scale3 += 0.1f;
			if(Rotate3 > -D3DX_PI * 2.0f / 6.0f)
			{
				Rotate3 = -D3DX_PI * 2.0f / 6.0f;
			}
			if(Scale3 > 1.5f)
			{
				Scale3 = 1.5f;
			}
		}
		else if(Rotate3 < 0.0f)
		{
			Rotate3 += D3DX_PI / 30.0f;
			Scale3 -= 0.05f;
			if(Rotate3 > 0.0f)
			{
				Rotate3 = 0.0f;
			}
			if(Scale3 < 1.0f)
			{
				Scale3 = 1.0f;
			}
		}
		else
		{
			Rotate3 = 0.0f;
			Scale3 = 1.0f;
			FlagNow = 4;
		}
	}
	else if(FlagNow == 4)
	{
		if(Rotate4 < -D3DX_PI * 2.0f / 6.0f)
		{
			Rotate4 += D3DX_PI / 20.0f;
			Scale4 += 0.1f;
			if(Rotate4 > -D3DX_PI * 2.0f / 6.0f)
			{
				Rotate4 = -D3DX_PI * 2.0f / 6.0f;
			}
			if(Scale4 > 1.5f)
			{
				Scale4 = 1.5f;
			}
		}
		else if(Rotate4 < 0.0f)
		{
			Rotate4 += D3DX_PI / 30.0f;
			Scale4 -= 0.05f;
			if(Rotate4 > 0.0f)
			{
				Rotate4 = 0.0f;
			}
			if(Scale4 < 1.0f)
			{
				Scale4 = 1.0f;
			}
		}
		else
		{
			Rotate4 = 0.0f;
			Scale4 = 1.0f;
			FlagNow = 5;
		}
	}
	else if(FlagNow == 5)
	{
		FlagCount++;
		if(FlagCount > 50)
		{
			FlagCount = 50;
			FlagNow = 6;
		}

		Rotate5 += -D3DX_PI / 160.0f;
		if(Rotate5 < -D3DX_PI / 8.0f)
		{
			Rotate5 = -D3DX_PI / 8.0f;
		}

		ScaleTextW += 0.04f;
		if(ScaleTextW > 1.0f)
		{
			ScaleTextW = 1.0f;
		}

		if(ScaleTextW == 1.0f)
		{
			RotateText += D3DX_PI / 30.0f;
			if(RotateText > 0.0f)
			{
				RotateText = 0.0f;
			}
		}
		else
		{
			RotateText += D3DX_PI / 240.0f;
			if(RotateText > -D3DX_PI / 3.0f)
			{
				RotateText = -D3DX_PI / 3.0f;
			}
		}

		ScaleTextH = cosf(RotateText);
		if(ScaleTextH < 0.0f)
		{
			ScaleTextH = 0.0f;
		}
		else if(ScaleTextH > 1.0f)
		{
			ScaleTextH = 1.0f;
		}
	}
	else if(FlagNow == 6)
	{
		RotateAngle += fDeltaTime*0.6f;
		if(RotateAngle > D3DX_PI*1.0f)
		{
			RotateAngle = -D3DX_PI*1.0f;
		}
		RotateX = 5.0f*cosf(RotateAngle) + 5.0f;

		RotateAngle1 += fDeltaTime*0.6f;
		if(RotateAngle1 > D3DX_PI*1.0f / 2.0f)
		{
			RotateAngle1 = -D3DX_PI*3.0f / 2.0f;
		}
		RotateX1 = 5.0f*cosf(RotateAngle1);

		RotateAngle2 += fDeltaTime*0.8f;
		if(RotateAngle2 > D3DX_PI*5.0f / 3.0f)
		{
			RotateAngle2 = -D3DX_PI*1.0f / 3.0f;
		}
		RotateX2 = -6.0f*cosf(RotateAngle2) + 3.0f;

		RotateAngle3 += fDeltaTime*0.8f;
		if(RotateAngle3 > D3DX_PI*3.0f / 2.0f)
		{
			RotateAngle3 = -D3DX_PI*1.0f / 2.0f;
		}
		RotateX3 = 5.0f*cosf(RotateAngle3);

		RotateAngle4 += fDeltaTime*0.5f;
		if(RotateAngle4 > D3DX_PI*1.0f)
		{
			RotateAngle4 = -D3DX_PI*1.0f;
		}
		RotateX4 = 5.0f*cosf(RotateAngle4) + 5.0f;

	}

	//标题动画绘制
	if(FlagNow >= 0)//状态0
	{
		SetRect(&Rect, 9, 3, 131, 128);
		m_pSprite->SetTransform(&MatrixOriginal);
		MatrixNow = MatrixOriginal;
		D3DXMatrixScaling(&MatrixScale, Scale, Scale, 1.0f);
		D3DXMatrixRotationZ(&MatrixRotate, Rotate);
		D3DXMatrixTranslation(&MatrixTranslate, (280.0f + RotateX), 71.0f, 0.0f);
		D3DXMatrixInverse(&MatrixInvrsT, NULL, &MatrixTranslate);
		MatrixNow = MatrixNow*MatrixInvrsT*MatrixScale*MatrixRotate*MatrixTranslate;
		m_pSprite->SetTransform(&MatrixNow);
		m_pSprite->Draw(m_pTextureTitle, &Rect, &D3DXVECTOR3(64.0f, 61.0f, 0.0f), &D3DXVECTOR3((280.0f + RotateX), 71.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pSprite->SetTransform(&MatrixOriginal);
	}

	if(FlagNow >= 1)//状态1
	{
		SetRect(&Rect, 138, 5, 218, 87);
		m_pSprite->SetTransform(&MatrixOriginal);
		MatrixNow = MatrixOriginal;
		D3DXMatrixScaling(&MatrixScale, Scale1, Scale1, 1.0f);
		D3DXMatrixRotationZ(&MatrixRotate, Rotate1);
		D3DXMatrixTranslation(&MatrixTranslate, (340.0f + RotateX1), 135.0f, 0.0f);
		D3DXMatrixInverse(&MatrixInvrsT, NULL, &MatrixTranslate);
		MatrixNow = MatrixNow*MatrixInvrsT*MatrixScale*MatrixRotate*MatrixTranslate;
		m_pSprite->SetTransform(&MatrixNow);
		m_pSprite->Draw(m_pTextureTitle, &Rect, &D3DXVECTOR3(40.0f, 41.0f, 0.0f), &D3DXVECTOR3((340.0f + RotateX1), 135.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pSprite->SetTransform(&MatrixOriginal);
	}

	if(FlagNow >= 2)//状态2
	{
		SetRect(&Rect, 225, 0, 380, 128);
		m_pSprite->SetTransform(&MatrixOriginal);
		MatrixNow = MatrixOriginal;
		D3DXMatrixScaling(&MatrixScale, Scale2, Scale2, 1.0f);
		D3DXMatrixRotationZ(&MatrixRotate, Rotate2);
		D3DXMatrixTranslation(&MatrixTranslate, (320.0f + RotateX2), 219.0f, 0.0f);
		D3DXMatrixInverse(&MatrixInvrsT, NULL, &MatrixTranslate);
		MatrixNow = MatrixNow*MatrixInvrsT*MatrixScale*MatrixRotate*MatrixTranslate;
		m_pSprite->SetTransform(&MatrixNow);
		m_pSprite->Draw(m_pTextureTitle, &Rect, &D3DXVECTOR3(78.0f, 64.0f, 0.0f), &D3DXVECTOR3((320.0f + RotateX2), 219.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pSprite->SetTransform(&MatrixOriginal);
	}

	if(FlagNow >= 3)//状态3
	{
		SetRect(&Rect, 387, 0, 509, 122);
		m_pSprite->SetTransform(&MatrixOriginal);
		MatrixNow = MatrixOriginal;
		D3DXMatrixScaling(&MatrixScale, Scale3, Scale3, 1.0f);
		D3DXMatrixRotationZ(&MatrixRotate, Rotate3);
		D3DXMatrixTranslation(&MatrixTranslate, (405.0f + RotateX3), 311.0f, 0.0f);
		D3DXMatrixInverse(&MatrixInvrsT, NULL, &MatrixTranslate);
		MatrixNow = MatrixNow*MatrixInvrsT*MatrixScale*MatrixRotate*MatrixTranslate;
		m_pSprite->SetTransform(&MatrixNow);
		m_pSprite->Draw(m_pTextureTitle, &Rect, &D3DXVECTOR3(61.0f, 61.0f, 0.0f), &D3DXVECTOR3((405.0f + RotateX3), 311.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pSprite->SetTransform(&MatrixOriginal);
	}

	if(FlagNow >= 4)//状态4
	{
		SetRect(&Rect, 510, 2, 610, 90);
		m_pSprite->SetTransform(&MatrixOriginal);
		MatrixNow = MatrixOriginal;
		D3DXMatrixScaling(&MatrixScale, Scale4, Scale4, 1.0f);
		D3DXMatrixRotationZ(&MatrixRotate, Rotate4);
		D3DXMatrixTranslation(&MatrixTranslate, (368.0f + RotateX4), 391.0f, 0.0f);
		D3DXMatrixInverse(&MatrixInvrsT, NULL, &MatrixTranslate);
		MatrixNow = MatrixNow*MatrixInvrsT*MatrixScale*MatrixRotate*MatrixTranslate;
		m_pSprite->SetTransform(&MatrixNow);
		m_pSprite->Draw(m_pTextureTitle, &Rect, &D3DXVECTOR3(50.0f, 44.0f, 0.0f), &D3DXVECTOR3((368.0f + RotateX4), 391.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pSprite->SetTransform(&MatrixOriginal);
	}

	if(FlagNow >= 5)//状态5
	{
		SetRect(&Rect, 12, 145, 620, 179);
		m_pSprite->SetTransform(&MatrixOriginal);
		MatrixNow = MatrixOriginal;
		D3DXMatrixScaling(&MatrixScale, ScaleTextW, ScaleTextH, 1.0f);
		D3DXMatrixRotationZ(&MatrixRotate, Rotate5);//-D3DX_PI / 8.0f
		D3DXMatrixTranslation(&MatrixTranslate, 324.0f, 280.0f, 0.0f);
		D3DXMatrixInverse(&MatrixInvrsT, NULL, &MatrixTranslate);
		MatrixNow = MatrixNow*MatrixInvrsT*MatrixScale*MatrixRotate*MatrixTranslate;
		m_pSprite->SetTransform(&MatrixNow);
		m_pSprite->Draw(m_pTextureTitle, &Rect, &D3DXVECTOR3(304.0f, 34.0f, 0.0f), &D3DXVECTOR3(324.0f, 280.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pSprite->SetTransform(&MatrixOriginal);
	}

	m_pSprite->End();

	return FlagNow;
}