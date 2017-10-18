#pragma once

#ifndef __DIRECTSOUND_H_
#define __DIRECTSOUND_H_

#include "Common.h"

enum PlayState
{
	PlayState_Loop = 0,//ѭ������
	PlayState_Once = 1,//���β���
};

class DirectSound
{
private:
	LPDIRECTSOUND8 m_pDirectSound;//IDirectSound8�ӿڶ���ָ��
	LPDIRECTSOUNDBUFFER m_pDirectSoundBuffer;//IDirectSoundBuffer�ӿڶ���ָ��
	DSBUFFERDESC m_DSBufferDesc;//Wave��������
	bool m_bDirectSoundStart;//Wave���ֲ��ű�־

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