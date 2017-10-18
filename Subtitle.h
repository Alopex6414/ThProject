#pragma once

#ifndef __SUBTITLE_H_
#define __SUBTITLE_H_

#include "Common.h"

enum VertexName
{
	Vertex_TopLeft = 0,				//����λ��:���Ͻ�
	Vertex_TopRight = 1,			//����λ��:���Ͻ�
	Vertex_BottomLeft = 2,		//����λ��:���½�
	Vertex_BottomRight = 3,		//����λ��:���½�
};

class Subtitle
{
private:
	float m_fX;//SubtitleX����
	float m_fY;//SubtitleY����
	float m_fWidth;//Subtitle���
	float m_fHeight;//Subtitle�߶�
	float m_fMoveX;//SubtitleX�����ƶ���
	float m_fMoveXDelta;//SubtitleX�����ƶ��仯��
	bool m_bMoveX;//SubtitleX����ѭ����־
	bool m_bMoveDirect;//SubtitleX�����ƶ�����
	float m_fAlphaValue;//SubtitleAlphaֵ
	bool m_bAlphaFlag;//SubtitleAlphaֵѭ����־
	bool m_bKeySelectFlashState;//Subtitleѡ�ж���״̬
	int m_nKeySelectFlashCount;//Subtitleѡ�ж���ʱ�����
	int m_nKeySelectFlashTimesCount;//Subtitleѡ�ж���֡����

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