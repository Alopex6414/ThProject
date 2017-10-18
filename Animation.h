#pragma once

#ifndef __ANIMATION_H_
#define __ANIMATION_H_

#include "Common.h"

class Animation
{
private:
	LPDIRECT3DDEVICE9 m_pD3D9Device;
	LPDIRECT3DTEXTURE9 m_pTextureTitle;
	LPD3DXSPRITE m_pSprite;

public:
	Animation();
	~Animation();
	Animation(LPDIRECT3DDEVICE9 pD3D9Device);
	HRESULT Initialize(LPCWSTR lpSrcFile);
	void UpdateTitle(float fDeltaTime);
	int RenderTitle(float fDeltaTime);
};

#endif // !__ANIMATION_H_