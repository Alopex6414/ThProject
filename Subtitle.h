#pragma once

#ifndef __SUBTITLE_H_
#define __SUBTITLE_H_

#include "Common.h"

enum VertexName
{
	Vertex_TopLeft = 0,				//顶点位置:左上角
	Vertex_TopRight = 1,			//顶点位置:右上角
	Vertex_BottomLeft = 2,		//顶点位置:左下角
	Vertex_BottomRight = 3,		//顶点位置:右下角
};

class Subtitle
{
private:
	float m_fX;//SubtitleX坐标
	float m_fY;//SubtitleY坐标
	float m_fWidth;//Subtitle宽度
	float m_fHeight;//Subtitle高度
	float m_fMoveX;//SubtitleX坐标移动量
	float m_fMoveXDelta;//SubtitleX坐标移动变化量
	bool m_bMoveX;//SubtitleX坐标循环标志
	bool m_bMoveDirect;//SubtitleX坐标移动方向
	float m_fAlphaValue;//SubtitleAlpha值
	bool m_bAlphaFlag;//SubtitleAlpha值循环标志
	bool m_bKeySelectFlashState;//Subtitle选中动画状态
	int m_nKeySelectFlashCount;//Subtitle选中动画时间计数
	int m_nKeySelectFlashTimesCount;//Subtitle选中动画帧计数

public:
	Subtitle();
	~Subtitle();
	Subtitle(float X, float Y, float Width, float Height, float Alpha);
	Subtitle(float X, float Y, float Width, float Height, float MoveX, float Alpha, bool Flag);
	void AddAlpha(float fAlpha);
	void SetAlpha(float fAlpha);
	void SetAlphaFlag(bool bAlphaFlag);
	void SetMoveX(float fMoveX);
	void SetMoveXStatus(bool bMoveX);
	void SetFlashState(bool bKeySelectFlashState);
	void ResetMoveX();
	void ResetSelectFlash();
	float GetAlpha() const;
	float GetMoveX() const;
	float GetMoveXDelta() const;
	float GetVertexX(VertexName Vertex) const;
	float GetVertexY(VertexName Vertex) const;
	bool GetMoveXStatus() const;
	bool GetMoveXFlashState() const;
	int GetMoveXFlashCount() const;
	void AlphaIncrease(float fAlphaMax, float fAlphaStep);
	void AlphaDecrease(float fAlphaMin, float fAlphaStep);
	void AlphaCircle(float fAlphaMin, float fAlphaMax, float fAlphaStep);
	void MoveXIncrease(float fMoveXMax, float fMoveXStep);
	void MoveXDecrease(float fMoveXMin, float fMoveXStep);
	void MoveXPendulum();
	void MoveXSelect();
};

#endif // !__SUBTITLE_H_