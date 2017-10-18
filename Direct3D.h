#pragma once

#ifndef __DIRECT3D_H_
#define __DIRECT3D_H_

#include "Common.h"
#include "Include.h"

//Enum Declaration
enum GameOptions
{
	GameStart = 0,
	ExtraStart = 1,
	PracticeStart = 2,
	Replay = 3,
	PlayerData = 4,
	MusicRoom = 5,
	Option = 6,
	Quit = 7,
};

//Struct Declaration
struct Vertex
{
	float X, Y, Z, RHW;
	DWORD DIFFUSE;
	float U, V;
	Vertex(float x, float y, float z, float rhw, DWORD Diffuse, float u, float v) :X(x), Y(y), Z(z), RHW(rhw), DIFFUSE(Diffuse), U(u), V(v) {}
};
#define D3DFVF_VERTEX	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)//未变换顶点坐标,反射光线,纹理坐标信息

//Variable Declaration
extern LPDIRECT3DDEVICE9 g_pD3D9Device;
extern LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;
extern LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer;

extern LPDIRECT3DTEXTURE9 g_pTexture;
extern LPDIRECT3DTEXTURE9 g_pTextureLoading;
extern LPDIRECT3DTEXTURE9 g_pTextureTitle;
extern LPDIRECT3DTEXTURE9 g_pTextureLogo;
extern LPDIRECT3DTEXTURE9 g_pTextureVersion;
extern LPDIRECT3DTEXTURE9 g_pTextureTitleText;

extern LPD3DXFONT g_pFontFPS;

class Subtitle;
class Particle;
class Animation;
class DirectInput;
class DirectSound;

extern DirectInput*g_pDirectInput;
extern DirectSound*g_pDirectSound;
extern DirectSound*g_pDirectSoundEffect;
extern Animation*g_pAnimation;
extern Particle*g_pSnow;
extern Subtitle*g_pSubSig;
extern Subtitle*g_pSubLoad;
extern Subtitle*g_pSubLoadEn;
extern Subtitle*g_pSubTitle;
extern Subtitle*g_pSubVersion;
extern Subtitle*g_pSubGameOptionSelect;
extern Subtitle*g_pSubArrayGameOptions;
extern Subtitle*g_pSubOptionConfigSelect;
extern Subtitle*g_pSubArrayOptionsConfig;

extern int g_nTitleFlagNow;
extern int g_nKeySelectState;
extern int g_nKeyNumber;
extern int g_nKeyNumber_Option;
extern bool g_bKeySelectFlashState;
extern bool g_bKeySelectTitleChange;

//Function Declaration
extern HRESULT Direct3D_Init(HWND hWnd);
extern HRESULT Direct3D_SetUp(void);
extern HRESULT DirectInput_Init(HWND hWnd, HINSTANCE hInstance);
extern HRESULT DirectSound_Init(HWND hWnd);
extern void Direct3D_Update(HWND hWnd, float fDeltaTime);
extern void Direct3D_Render(HWND hWnd, float fDeltaTime);
extern void Direct3D_ClearUp(void);
extern float Direct3D_FPS(void);
extern void MatrixSet(void);

#endif // !__DIRECT3D_H_