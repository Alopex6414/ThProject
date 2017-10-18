#include"Common.h"
#include "Include.h"
#include "Subtitle.h"


/*
** Function:	 Subtitle()
** Purpose: Subtitle构造函数
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
** Purpose: Subtitle析构函数
*/
Subtitle::~Subtitle()
{
}

/*
** Function:	 Subtitle()
** Purpose: Subtitle构造函数(重载+1)
** Para: float X			//SubtitleX坐标
** Para: float Y			//SubtitleY坐标
** Para: float Alpha		//SubtitleAlpha初值透明度值
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
** Purpose: Subtitle构造函数(重载+2)
** Para: float X			//SubtitleX坐标
** Para: float Y			//SubtitleY坐标
** Para: float MoveX	//SubtitleX初始移动坐标
** Para: float Alpha		//SubtitleAlpha初值透明度值
** Para: bool Flag		//SubtitleAlpha循环标志初值
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
** Purpose: SubtitleAlpha设置Alpha透明度函数(递增)
** Para: float fAlpha		//Alpha通道透明度
*/
void Subtitle::AddAlpha(float fAlpha)
{
	m_fAlphaValue += fAlpha;
}

/*
** Function:	 SetAlpha()
** Purpose: SubtitleAlpha设置Alpha透明度函数
** Para: float fAlpha		//Alpha通道透明度
*/
void Subtitle::SetAlpha(float fAlpha)
{
	m_fAlphaValue = fAlpha;
}

/*
** Function:	 SetAlphaFlag()
** Purpose: SubtitleAlpha设置Alpha透明度标志函数
** Para: bool bAlphaFlag		//Alpha通道透明度标志
*/
void Subtitle::SetAlphaFlag(bool bAlphaFlag)
{
	m_bAlphaFlag = bAlphaFlag;
}

/*
** Function:	 SetMoveX()
** Purpose: SubtitleAlpha设置MoveX数值
** Para: float fMoveX				//MoveX数值
*/
void Subtitle::SetMoveX(float fMoveX)
{
	m_fMoveX = fMoveX;
}

/*
** Function:	 SetMoveXStatus()
** Purpose: SubtitleAlpha设置MoveX状态
** Para: bool bMoveX				//MoveX状态
*/
void Subtitle::SetMoveXStatus(bool bMoveX)
{
	m_bMoveX = bMoveX;
}

/*
** Function:	 SetFlashState()
** Purpose: Subtitle设置动画渲染状态
** Para: bool bKeySelectFlashState		//动画渲染状态
*/
void Subtitle::SetFlashState(bool bKeySelectFlashState)
{
	m_bKeySelectFlashState = bKeySelectFlashState;
}

/*
** Function:	 ResetMoveX()
** Purpose: SubtitleAlpha重置MoveX相关变量
*/
void Subtitle::ResetMoveX()
{
	m_bMoveX = false;
	m_bMoveDirect = false;
	m_fMoveXDelta = 0.0f;
}

/*
** Function:	 ResetSelectFlash()
** Purpose: SubtitleAlpha重置SelectFlash相关变量
*/
void Subtitle::ResetSelectFlash()
{
	m_bKeySelectFlashState = false;
	m_nKeySelectFlashCount = 0;
	m_nKeySelectFlashTimesCount = 0;
}

/*
** Function:	 GetAlpha()
** Purpose: SubtitleAlpha获取Alpha透明度函数
** Return: float fAlpha		//Alpha通道透明度
*/
float Subtitle::GetAlpha() const
{
	return m_fAlphaValue;
}

/*
** Function:	 GetMoveX()
** Purpose: SubtitleAlpha获取X坐标增量函数
** Return: float fMoveX		//X坐标增量
*/
float Subtitle::GetMoveX() const
{
	return m_fMoveX;
}

/*
** Function:	 GetMoveXDelta()
** Purpose: SubtitleAlpha获取X坐标增量变化率函数
** Return: float fMoveXDelta		//X坐标增量
*/
float Subtitle::GetMoveXDelta() const
{
	return m_fMoveXDelta;
}

/*
** Function:	 GetVertexX()
** Purpose: Subtitle获取顶点X坐标
** Para: VertexName Vertex	//顶点位置
** Return: float X					//顶点X坐标
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
** Purpose: Subtitle获取顶点Y坐标
** Para: VertexName Vertex	//顶点位置
** Return: float Y					//顶点Y坐标
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
** Purpose: SubtitleAlpha获取X位移状态
** Return: bool bMoveX		//X位移完成状态
*/
bool Subtitle::GetMoveXStatus() const
{
	return m_bMoveX;
}

/*
** Function:	 GetMoveXFlashState()
** Purpose: SubtitleAlpha获取闪烁渲染状态
** Return: bool bState		//闪烁状态
*/
bool Subtitle::GetMoveXFlashState() const
{
	return m_bKeySelectFlashState;
}

/*
** Function:	 GetMoveXFlashCount()
** Purpose: SubtitleAlpha获取闪烁次数
** Return: int nCount			//闪烁次数
*/
int Subtitle::GetMoveXFlashCount() const
{
	return m_nKeySelectFlashTimesCount;
}

/*
** Function:	 AlphaIncrease()
** Purpose: SubtitleAlpha通道递增变化函数
** Para: float fAlphaMax	//Alpha通道最大值
** Para: float fAlphaStep	//Alpha通道递增变化步长
*/
void Subtitle::AlphaIncrease(float fAlphaMax, float fAlphaStep)
{
	m_fAlphaValue += fAlphaStep;//Alpha通道透明度递增
	if(m_fAlphaValue >= fAlphaMax)
	{
		m_fAlphaValue = fAlphaMax;//Alpha通道透明度限制
	}
}

/*
** Function:	 AlphaDecrease()
** Purpose: SubtitleAlpha通道递减变化函数
** Para: float fAlphaMin		//Alpha通道最小值
** Para: float fAlphaStep	//Alpha通道递减变化步长
*/
void Subtitle::AlphaDecrease(float fAlphaMin, float fAlphaStep)
{
	m_fAlphaValue += -fAlphaStep;//Alpha通道透明度递减
	if(m_fAlphaValue <= fAlphaMin)
	{
		m_fAlphaValue = fAlphaMin;//Alpha通道透明度限制
	}
}

/*
** Function:	 AlphaCircle()
** Purpose: SubtitleAlpha通道明暗交替变化函数
** Para: float fAlphaMin		//Alpha通道最小值
** Para: float fAlphaMax	//Alpha通道最大值
** Para: float fAlphaStep	//Alpha通道交替变化步长
*/
void Subtitle::AlphaCircle(float fAlphaMin, float fAlphaMax, float fAlphaStep)
{
	if(m_bAlphaFlag == false)//交替变化标志为False
	{
		m_fAlphaValue += -fAlphaStep;//Alpha通道透明度递减
		if(m_fAlphaValue <= fAlphaMin)//当Alpha通道透明度小于最小值时
		{
			m_fAlphaValue = fAlphaMin;//Alpha通道透明度等于最小值
			m_bAlphaFlag = true;//交替变化标志为True,开始递增
		}
	}
	else//交替变化标志为True
	{
		m_fAlphaValue += fAlphaStep;//Alpha通道透明度递增
		if(m_fAlphaValue >= fAlphaMax)//当Alpha通道透明度大于最大值时
		{
			m_fAlphaValue = fAlphaMax;//Alpha通道透明度等于最大值
			m_bAlphaFlag = false;//交替变化标志为False,开始递减
		}
	}
}

/*
** Function:	 MoveXIncrease()
** Purpose: SubtitleX坐标移动递增变化函数
** Para: float fMoveXMax	//X坐标移动最大值
** Para: float fMoveXStep	//X坐标移动步长
*/
void Subtitle::MoveXIncrease(float fMoveXMax, float fMoveXStep)
{
	m_fMoveX += fMoveXStep;//X坐标递增
	if(m_fMoveX >= fMoveXMax)
	{
		m_fMoveX = fMoveXMax;//X坐标限制
	}
}

/*
** Function:	 MoveXDecrease()
** Purpose: SubtitleX坐标移动递减变化函数
** Para: float fMoveXMin	//X坐标移动最小值
** Para: float fMoveXStep	//X坐标移动步长
*/
void Subtitle::MoveXDecrease(float fMoveXMin, float fMoveXStep)
{
	m_fMoveX += -fMoveXStep;//X坐标递减
	if(m_fMoveX <= fMoveXMin)
	{
		m_fMoveX = fMoveXMin;//X坐标限制
	}
}

/*
** Function:	 MoveXPendulum()
** Purpose: SubtitleX坐标移动单摆式变化
*/
void Subtitle::MoveXPendulum()
{
	if(m_bMoveX == false)//MoveX移动标志未置位
	{
		if(m_bMoveDirect == false)//MoveX向右移动(Right2->Left4->Right2)
		{
			m_fMoveXDelta += 2.0f;//Right:2
			if(m_fMoveXDelta >= 4.0f)
			{
				m_fMoveXDelta = 4.0f;
				m_bMoveDirect = true;
			}
			if(m_fMoveXDelta == 0.0f)//MoveX结束标志
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
** Purpose: Subtitle选中连续闪烁
*/
void Subtitle::MoveXSelect()
{
	if(m_bKeySelectFlashState == false)//Subtitle闪烁
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
