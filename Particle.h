#pragma once

#ifndef __PARTICLE_H_
#define __PARTICLE_H_

#include "Common.h"

//Macro Definition
#define PARTICLE_NUMBER	25

//Struct
struct SnowParticle
{
	float X, Y;
	float Scale;
	float Rotate;
	float Alpha;
	float FallSpeed;
	float MoveSpeed;
	int MoveDirect;
	float RotateSpeed;
	int RotateDirect;
};

class Particle
{
private:
	LPDIRECT3DDEVICE9 m_pD3D9Device;
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPD3DXSPRITE m_pSprite;
	SnowParticle m_Snow[PARTICLE_NUMBER];
	int m_nPosX;
	int m_nPosY;
	int m_nWidth;
	int m_nHeight;

public:
	Particle();
	~Particle();
	Particle(LPDIRECT3DDEVICE9 pD3D9Device, int X, int Y, int Width, int Height);
	HRESULT Initialize(LPCWSTR lpSrcFile);
	void Update(float fDeltaTime);
	void Render(void);
};

#endif // !__PARTICLE_H_