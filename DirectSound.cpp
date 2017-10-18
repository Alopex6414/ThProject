#include"Common.h"
#include "Include.h"
#include "DirectSound.h"


/*
** Function:	 DirectSound()
** Purpose: DirectSound构造函数
*/
DirectSound::DirectSound()
{
	m_pDirectSound = NULL;//IDirectSound8接口对象指针NULL
	m_pDirectSoundBuffer = NULL;//IDirectSoundBuffer接口对象指针NULL
	m_bDirectSoundStart = false;//DirectSound音乐未播放
}

/*
** Function:	 ~DirectSound()
** Purpose: DirectSound析构函数
*/
DirectSound::~DirectSound()
{
	SAFE_RELEASE(m_pDirectSoundBuffer);//IDirectSoundBuffer释放
	SAFE_RELEASE(m_pDirectSound);//IDirectSound释放
}

/*
** Function:	 DirectSound::Initialize(HWND hWnd, DWORD SoundCoopFlags)
** Purpose: DirectSound初始化
** Para: HWND hWnd(窗口句柄)
** Para: DWORD SoundCoopFlags(Sound协作级别)
** Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
*/
HRESULT DirectSound::Initialize(HWND hWnd, DWORD SoundCoopFlags)
{
	HRESULT Result;

	//创建IDirectSound8接口对象
	Result = DirectSoundCreate8(NULL, &m_pDirectSound, NULL);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//设置DirectSound协作级别
	Result = m_pDirectSound->SetCooperativeLevel(hWnd, SoundCoopFlags);//DirectSound协作级别:正常模式

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	return S_OK;
}

/*
** Function:	 DirectSound::LoadWave(LPDIRECTSOUND8 lpDirectSound, LPSTR lpzFileNamePath)
** Purpose: DirectSound加载Wave音乐
** Para: LPWSTR lpszFileName(Wave音乐文件名称)
** Return: HRESULT(加载状态:成功:S_OK,失败:E_FAIL)
*/
HRESULT DirectSound::LoadWave(LPWSTR lpszFileName)
{
	WAVEFORMATEX*pWaveFormatEx;
	HMMIO hWave;
	MMCKINFO Parent;
	MMCKINFO Child;
	MMRESULT MMResult;
	HRESULT Result;
	UCHAR*pSoundBuffer;
	UCHAR*AudioPtr1 = NULL;
	UCHAR*AudioPtr2 = NULL;
	DWORD AudioLength1 = 0;
	DWORD AudioLength2 = 0;

	SAFE_RELEASE(m_pDirectSoundBuffer);

	pWaveFormatEx = new WAVEFORMATEX();
	ZeroMemory(&Parent, sizeof(Parent));
	ZeroMemory(&Child, sizeof(Child));

	hWave = mmioOpen(lpszFileName, NULL, MMIO_READ | MMIO_ALLOCBUF);

	if(hWave == NULL)
	{
		return E_FAIL;//FAIL
	}

	Parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	MMResult = mmioDescend(hWave, &Parent, NULL, MMIO_FINDRIFF);

	if(MMResult)
	{
		mmioClose(hWave, NULL);
		return E_FAIL;//FAIL
	}

	Child.ckid = mmioFOURCC('f', 'm', 't', ' ');

	MMResult = mmioDescend(hWave, &Child, &Parent, NULL);

	if(MMResult)
	{
		mmioClose(hWave, NULL);
		return E_FAIL;//FAIL
	}

	if(mmioRead(hWave, (char*)pWaveFormatEx, sizeof(WAVEFORMATEX)) != sizeof(WAVEFORMATEX))
	{
		mmioClose(hWave, NULL);
		return E_FAIL;//FAIL
	}

	if(pWaveFormatEx->wFormatTag != WAVE_FORMAT_PCM)
	{
		mmioClose(hWave, NULL);
		return E_FAIL;//FAIL
	}

	MMResult = mmioAscend(hWave, &Child, NULL);

	if(MMResult)
	{
		mmioClose(hWave, NULL);
		return E_FAIL;//FAIL
	}

	Child.ckid = mmioFOURCC('d', 'a', 't', 'a');

	MMResult = mmioDescend(hWave, &Child, &Parent, MMIO_FINDCHUNK);

	if(MMResult)
	{
		mmioClose(hWave, NULL);
		return E_FAIL;//FAIL
	}

	pSoundBuffer = (UCHAR*)malloc(Child.cksize);
	mmioRead(hWave, (char*)pSoundBuffer, Child.cksize);
	mmioClose(hWave, NULL);

	WAVEFORMATEX*pWaveFormatExDevice;
	
	pWaveFormatExDevice = new WAVEFORMATEX();
	ZeroMemory(pWaveFormatExDevice, sizeof(WAVEFORMATEX));
	memset(pWaveFormatExDevice, 0, sizeof(WAVEFORMATEX));
	pWaveFormatExDevice->wFormatTag = pWaveFormatEx->wFormatTag;
	pWaveFormatExDevice->nChannels = pWaveFormatEx->nChannels;
	pWaveFormatExDevice->nSamplesPerSec = pWaveFormatEx->nSamplesPerSec;
	pWaveFormatExDevice->nBlockAlign = pWaveFormatEx->nBlockAlign;
	pWaveFormatExDevice->nAvgBytesPerSec = pWaveFormatEx->nAvgBytesPerSec;
	pWaveFormatExDevice->wBitsPerSample = pWaveFormatEx->wBitsPerSample;
	pWaveFormatExDevice->cbSize = pWaveFormatEx->cbSize;

	ZeroMemory(&m_DSBufferDesc, sizeof(m_DSBufferDesc));
	m_DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	m_DSBufferDesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_STATIC | DSBCAPS_LOCDEFER;
	m_DSBufferDesc.dwBufferBytes = Child.cksize;
	m_DSBufferDesc.lpwfxFormat = pWaveFormatExDevice;

	Result = m_pDirectSound->CreateSoundBuffer(&m_DSBufferDesc, &m_pDirectSoundBuffer, NULL);

	if(Result)
	{
		SAFE_DELETE(pSoundBuffer);
		return E_FAIL;//FAIL
	}

	m_pDirectSoundBuffer->Lock(0, Child.cksize, (void**)&AudioPtr1, &AudioLength1, (void**)&AudioPtr2, &AudioLength2, DSBLOCK_FROMWRITECURSOR);
	memcpy(AudioPtr1, pSoundBuffer, AudioLength1);
	memcpy(AudioPtr2, (pSoundBuffer + AudioLength1), AudioLength2);
	m_pDirectSoundBuffer->Unlock(AudioPtr1, AudioLength1, AudioPtr2, AudioLength2);

	SAFE_DELETE(pSoundBuffer);

	return S_OK;
}

/*
** Function:	 DirectSound::PlayWave()
** Purpose: DirectSound播放Wave音乐
*/
void DirectSound::PlayWave()
{
	m_pDirectSoundBuffer->Play(NULL, NULL, DSBPLAY_LOOPING);
}

/*
** Function:	 DirectSound::PlayWave(PlayState DS_PlayState)(重载+1)
** Purpose: DirectSound播放Wave音乐
** Para: PlayState DS_PlayState(播放状态)
*/
void DirectSound::PlayWave(PlayState DS_PlayState)
{
	if(DS_PlayState == PlayState_Loop)
	{
		m_pDirectSoundBuffer->Play(NULL, NULL, DSBPLAY_LOOPING);
	}
	else if(DS_PlayState == PlayState_Once)
	{
		m_pDirectSoundBuffer->Play(NULL, NULL, NULL);
	}
	else
	{
		m_pDirectSoundBuffer->Play(NULL, NULL, NULL);
	}
}

/*
** Function:	 DirectSound::PlayMusic()
** Purpose: DirectSound播放游戏音乐(循环)
*/
void DirectSound::PlayMusic()
{
	if(m_bDirectSoundStart == false)
	{
		m_bDirectSoundStart = true;
		m_pDirectSoundBuffer->Play(NULL, NULL, DSBPLAY_LOOPING);
	}
}

/*
** Function:	 DirectSound::PlaySoundEffect(LPWSTR lpszFileName)
** Purpose: DirectSound播放游戏音效(单次)
*/
void DirectSound::PlaySoundEffect(LPWSTR lpszFileName)
{
	LoadWave(lpszFileName);
	m_pDirectSoundBuffer->Play(NULL, NULL, NULL);
}
