#include"Common.h"
#include "Include.h"
#include "Subtitle.h"


/*
** Function:	 Subtitle()
** Purpose: Subtitle���캯��
*/
Subtitle::Subtitle()
{
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fAlphaValue = 0.0f;
	m_fMoveX = 0.0f;
	m_fMoveXDelta = 0.0f;
	m_bAlphaFlag = false;
	m_bMoveX = false;
	m_bMoveDirect = false;
	m_bKeySelectFlashState = false;
	m_nKeySelectFlashCount = 0;
	m_nKeySelectFlashTimesCount = 0;
}

/*
** Function:	 ~Subtitle()
** Purpose: Subtitle��������
*/
Subtitle::~Subtitle()
{
}

/*
** Function:	 Subtitle()
** Purpose: Subtitle���캯��(����+1)
** Para: float X			//SubtitleX����
** Para: float Y			//SubtitleY����
** Para: float Alpha		//SubtitleAlpha��ֵ͸����ֵ
*/
Subtitle::Subtitle(float X, float Y, float Width, float Height, float Alpha)
{
	m_fX = X;
	m_fY = Y;
	m_fWidth = Width;
	m_fHeight = Height;
	m_fAlphaValue = Alpha;
	m_fMoveX = 0.0f;
	m_fMoveXDelta = 0.0f;
	m_bAlphaFlag = false;
	m_bMoveX = false;
	m_bMoveDirect = false;
	m_bKeySelectFlashState = false;
	m_nKeySelectFlashCount = 0;
	m_nKeySelectFlashTimesCount = 0;
}

/*
** Function:	 Subtitle()
** Purpose: Subtitle���캯��(����+2)
** Para: float X			//SubtitleX����
** Para: float Y			//SubtitleY����
** Para: float MoveX	//SubtitleX��ʼ�ƶ�����
** Para: float Alpha		//SubtitleAlpha��ֵ͸����ֵ
** Para: bool Flag		//SubtitleAlphaѭ����־��ֵ
*/
Subtitle::Subtitle(float X, float Y, float Width, float Height, float MoveX, float Alpha, bool Flag)
{
	m_fX = X;
	m_fY = Y;
	m_fWidth = Width;
	m_fHeight = Height;
	m_fAlphaValue = Alpha;
	m_fMoveX = MoveX;
	m_fMoveXDelta = 0.0f;
	m_bAlphaFlag = Flag;
	m_bMoveX = false;
	m_bMoveDirect = false;
	m_bKeySelectFlashState = false;
	m_nKeySelectFlashCount = 0;
	m_nKeySelectFlashTimesCount = 0;
}

/*
** Function:	 AddAlpha()
** Purpose: SubtitleAlpha����Alpha͸���Ⱥ���(����)
** Para: float fAlpha		//Alphaͨ��͸����
*/
void Subtitle::AddAlpha(float fAlpha)
{
	m_fAlphaValue += fAlpha;
}

/*
** Function:	 SetAlpha()
** Purpose: SubtitleAlpha����Alpha͸���Ⱥ���
** Para: float fAlpha		//Alphaͨ��͸����
*/
void Subtitle::SetAlpha(float fAlpha)
{
	m_fAlphaValue = fAlpha;
}

/*
** Function:	 SetAlphaFlag()
** Purpose: SubtitleAlpha����Alpha͸���ȱ�־����
** Para: bool bAlphaFlag		//Alphaͨ��͸���ȱ�־
*/
void Subtitle::SetAlphaFlag(bool bAlphaFlag)
{
	m_bAlphaFlag = bAlphaFlag;
}

/*
** Function:	 SetMoveX()
** Purpose: SubtitleAlpha����MoveX��ֵ
** Para: float fMoveX				//MoveX��ֵ
*/
void Subtitle::SetMoveX(float fMoveX)
{
	m_fMoveX = fMoveX;
}

/*
** Function:	 SetMoveXStatus()
** Purpose: SubtitleAlpha����MoveX״̬
** Para: bool bMoveX				//MoveX״̬
*/
void Subtitle::SetMoveXStatus(bool bMoveX)
{
	m_bMoveX = bMoveX;
}

/*
** Function:	 SetFlashState()
** Purpose: Subtitle���ö�����Ⱦ״̬
** Para: bool bKeySelectFlashState		//������Ⱦ״̬
*/
void Subtitle::SetFlashState(bool bKeySelectFlashState)
{
	m_bKeySelectFlashState = bKeySelectFlashState;
}

/*
** Function:	 ResetMoveX()
** Purpose: SubtitleAlpha����MoveX��ر���
*/
void Subtitle::ResetMoveX()
{
	m_bMoveX = false;
	m_bMoveDirect = false;
	m_fMoveXDelta = 0.0f;
}

/*
** Function:	 ResetSelectFlash()
** Purpose: SubtitleAlpha����SelectFlash��ر���
*/
void Subtitle::ResetSelectFlash()
{
	m_bKeySelectFlashState = false;
	m_nKeySelectFlashCount = 0;
	m_nKeySelectFlashTimesCount = 0;
}

/*
** Function:	 GetAlpha()
** Purpose: SubtitleAlpha��ȡAlpha͸���Ⱥ���
** Return: float fAlpha		//Alphaͨ��͸����
*/
float Subtitle::GetAlpha() const
{
	return m_fAlphaValue;
}

/*
** Function:	 GetMoveX()
** Purpose: SubtitleAlpha��ȡX������������
** Return: float fMoveX		//X��������
*/
float Subtitle::GetMoveX() const
{
	return m_fMoveX;
}

/*
** Function:	 GetMoveXDelta()
** Purpose: SubtitleAlpha��ȡX���������仯�ʺ���
** Return: float fMoveXDelta		//X��������
*/
float Subtitle::GetMoveXDelta() const
{
	return m_fMoveXDelta;
}

/*
** Function:	 GetVertexX()
** Purpose: Subtitle��ȡ����X����
** Para: VertexName Vertex	//����λ��
** Return: float X					//����X����
*/
float Subtitle::GetVertexX(VertexName Vertex) const
{
	float VertexX = 0.0f;

	switch(Vertex)
	{
	case Vertex_TopLeft:
	case Vertex_BottomLeft:
		VertexX = m_fX;
		break;
	case Vertex_TopRight:
	case Vertex_BottomRight:
		VertexX = m_fX + m_fWidth;
		break;
	default:
		break;
	}

	return VertexX;
}

/*
** Function:	 GetVertexY()
** Purpose: Subtitle��ȡ����Y����
** Para: VertexName Vertex	//����λ��
** Return: float Y					//����Y����
*/
float Subtitle::GetVertexY(VertexName Vertex) const
{
	float VertexY = 0.0f;

	switch(Vertex)
	{
	case Vertex_TopLeft:
	case Vertex_TopRight:
		VertexY = m_fY;
		break;
	case Vertex_BottomLeft:
	case Vertex_BottomRight:
		VertexY = m_fY + m_fHeight;
		break;
	default:
		break;
	}

	return VertexY;
}

/*
** Function:	 GetMoveXStatus()
** Purpose: SubtitleAlpha��ȡXλ��״̬
** Return: bool bMoveX		//Xλ�����״̬
*/
bool Subtitle::GetMoveXStatus() const
{
	return m_bMoveX;
}

/*
** Function:	 GetMoveXFlashState()
** Purpose: SubtitleAlpha��ȡ��˸��Ⱦ״̬
** Return: bool bState		//��˸״̬
*/
bool Subtitle::GetMoveXFlashState() const
{
	return m_bKeySelectFlashState;
}

/*
** Function:	 GetMoveXFlashCount()
** Purpose: SubtitleAlpha��ȡ��˸����
** Return: int nCount			//��˸����
*/
int Subtitle::GetMoveXFlashCount() const
{
	return m_nKeySelectFlashTimesCount;
}

/*
** Function:	 AlphaIncrease()
** Purpose: SubtitleAlphaͨ�������仯����
** Para: float fAlphaMax	//Alphaͨ�����ֵ
** Para: float fAlphaStep	//Alphaͨ�������仯����
*/
void Subtitle::AlphaIncrease(float fAlphaMax, float fAlphaStep)
{
	m_fAlphaValue += fAlphaStep;//Alphaͨ��͸���ȵ���
	if(m_fAlphaValue >= fAlphaMax)
	{
		m_fAlphaValue = fAlphaMax;//Alphaͨ��͸��������
	}
}

/*
** Function:	 AlphaDecrease()
** Purpose: SubtitleAlphaͨ���ݼ��仯����
** Para: float fAlphaMin		//Alphaͨ����Сֵ
** Para: float fAlphaStep	//Alphaͨ���ݼ��仯����
*/
void Subtitle::AlphaDecrease(float fAlphaMin, float fAlphaStep)
{
	m_fAlphaValue += -fAlphaStep;//Alphaͨ��͸���ȵݼ�
	if(m_fAlphaValue <= fAlphaMin)
	{
		m_fAlphaValue = fAlphaMin;//Alphaͨ��͸��������
	}
}

/*
** Function:	 AlphaCircle()
** Purpose: SubtitleAlphaͨ����������仯����
** Para: float fAlphaMin		//Alphaͨ����Сֵ
** Para: float fAlphaMax	//Alphaͨ�����ֵ
** Para: float fAlphaStep	//Alphaͨ������仯����
*/
void Subtitle::AlphaCircle(float fAlphaMin, float fAlphaMax, float fAlphaStep)
{
	if(m_bAlphaFlag == false)//����仯��־ΪFalse
	{
		m_fAlphaValue += -fAlphaStep;//Alphaͨ��͸���ȵݼ�
		if(m_fAlphaValue <= fAlphaMin)//��Alphaͨ��͸����С����Сֵʱ
		{
			m_fAlphaValue = fAlphaMin;//Alphaͨ��͸���ȵ�����Сֵ
			m_bAlphaFlag = true;//����仯��־ΪTrue,��ʼ����
		}
	}
	else//����仯��־ΪTrue
	{
		m_fAlphaValue += fAlphaStep;//Alphaͨ��͸���ȵ���
		if(m_fAlphaValue >= fAlphaMax)//��Alphaͨ��͸���ȴ������ֵʱ
		{
			m_fAlphaValue = fAlphaMax;//Alphaͨ��͸���ȵ������ֵ
			m_bAlphaFlag = false;//����仯��־ΪFalse,��ʼ�ݼ�
		}
	}
}

/*
** Function:	 MoveXIncrease()
** Purpose: SubtitleX�����ƶ������仯����
** Para: float fMoveXMax	//X�����ƶ����ֵ
** Para: float fMoveXStep	//X�����ƶ�����
*/
void Subtitle::MoveXIncrease(float fMoveXMax, float fMoveXStep)
{
	m_fMoveX += fMoveXStep;//X�������
	if(m_fMoveX >= fMoveXMax)
	{
		m_fMoveX = fMoveXMax;//X��������
	}
}

/*
** Function:	 MoveXDecrease()
** Purpose: SubtitleX�����ƶ��ݼ��仯����
** Para: float fMoveXMin	//X�����ƶ���Сֵ
** Para: float fMoveXStep	//X�����ƶ�����
*/
void Subtitle::MoveXDecrease(float fMoveXMin, float fMoveXStep)
{
	m_fMoveX += -fMoveXStep;//X����ݼ�
	if(m_fMoveX <= fMoveXMin)
	{
		m_fMoveX = fMoveXMin;//X��������
	}
}

/*
** Function:	 MoveXPendulum()
** Purpose: SubtitleX�����ƶ�����ʽ�仯
*/
void Subtitle::MoveXPendulum()
{
	if(m_bMoveX == false)//MoveX�ƶ���־δ��λ
	{
		if(m_bMoveDirect == false)//MoveX�����ƶ�(Right2->Left4->Right2)
		{
			m_fMoveXDelta += 2.0f;//Right:2
			if(m_fMoveXDelta >= 4.0f)
			{
				m_fMoveXDelta = 4.0f;
				m_bMoveDirect = true;
			}
			if(m_fMoveXDelta == 0.0f)//MoveX������־
			{
				m_fMoveXDelta = 0.0f;
				m_bMoveDirect = false;
				m_bMoveX = true;
			}
		}
		else
		{
			m_fMoveXDelta -= 2.0f;//Left:4
			if(m_fMoveXDelta <= -4.0f)
			{
				m_fMoveXDelta = -4.0f;
				m_bMoveDirect = false;
			}
		}
	}
}

/*
** Function:	 MoveXSelect()
** Purpose: Subtitleѡ��������˸
*/
void Subtitle::MoveXSelect()
{
	if(m_bKeySelectFlashState == false)//Subtitle��˸
	{
		m_nKeySelectFlashCount++;
		if(m_nKeySelectFlashCount >= 2)
		{
			m_nKeySelectFlashCount = 0;
			m_bKeySelectFlashState = true;
			m_nKeySelectFlashTimesCount++;
		}
	}
	else
	{
		m_nKeySelectFlashCount++;
		if(m_nKeySelectFlashCount >= 2)
		{
			m_nKeySelectFlashCount = 0;
			m_bKeySelectFlashState = false;
			m_nKeySelectFlashTimesCount++;
		}
	}
}
