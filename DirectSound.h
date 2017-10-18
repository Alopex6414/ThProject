#pragma once

#ifndef __DIRECTSOUND_H_
#define __DIRECTSOUND_H_

#include "Common.h"

enum PlayState
{
	PlayState_Loop = 0,//循环播放
	PlayState_Once = 1,//单次播放
};

class DirectSound
{
private:
	LPDIRECTSOUND8 m_pDirectSound;//IDirectSound8接口对象指针
	LPDIRECTSOUNDBUFFER m_pDirectSoundBuffer;//IDirectSoundBuffer接口对象指针
	DSBUFFERDESC m_DSBufferDesc;//Wave缓存描述
	bool m_bDirectSoundStart;//Wave音乐播放标志

public:
	DirectSound();
	~DirectSound();
	HRESULT Initialize(HWND hWnd, DWORD SoundCoopFlags);
	HRESULT LoadWave(LPWSTR lpszFileName);
	void PlayWave();
	void PlayWave(PlayState DS_PlayState);
	void PlayMusic();
	void PlaySoundEffect(LPWSTR lpszFileName);
};

#endif // !__DIRECTSOUND_H_