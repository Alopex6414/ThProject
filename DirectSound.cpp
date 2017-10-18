#include"Common.h"
#include "Include.h"
#include "DirectSound.h"


/*
** Function:	 DirectSound()
** Purpose: DirectSound���캯��
*/
DirectSound::DirectSound()
{
	m_pDirectSound = NULL;//IDirectSound8�ӿڶ���ָ��NULL
	m_pDirectSoundBuffer = NULL;//IDirectSoundBuffer�ӿڶ���ָ��NULL
	m_bDirectSoundStart = false;//DirectSound����δ����
}

/*
** Function:	 ~DirectSound()
** Purpose: DirectSound��������
*/
DirectSound::~DirectSound()
{
	SAFE_RELEASE(m_pDirectSoundBuffer);//IDirectSoundBuffer�ͷ�
	SAFE_RELEASE(m_pDirectSound);//IDirectSound�ͷ�
}

/*
** Function:	 DirectSound::Initialize(HWND hWnd, DWORD SoundCoopFlags)
** Purpose: DirectSound��ʼ��
** Para: HWND hWnd(���ھ��)
** Para: DWORD SoundCoopFlags(SoundЭ������)
** Return: HRESULT(��ʼ��״̬:�ɹ�:S_OK,ʧ��:E_FAIL)
*/
HRESULT DirectSound::Initialize(HWND hWnd, DWORD SoundCoopFlags)
{
	HRESULT Result;

	//����IDirectSound8�ӿڶ���
	Result = DirectSoundCreate8(NULL, &m_pDirectSound, NULL);

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	//����DirectSoundЭ������
	Result = m_pDirectSound->SetCooperativeLevel(hWnd, SoundCoopFlags);//DirectSoundЭ������:����ģʽ

	if(FAILED(Result))
	{
		return E_FAIL;//FAIL
	}

	return S_OK;
}

/*
** Function:	 DirectSound::LoadWave(LPDIRECTSOUND8 lpDirectSound, LPSTR lpzFileNamePath)
** Purpose: DirectSound����Wave����
** Para: LPWSTR lpszFileName(Wave�����ļ�����)
** Return: HRESULT(����״̬:�ɹ�:S_OK,ʧ��:E_FAIL)
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
** Purpose: DirectSound����Wave����
*/
void DirectSound::PlayWave()
{
	m_pDirectSoundBuffer->Play(NULL, NULL, DSBPLAY_LOOPING);
}

/*
** Function:	 DirectSound::PlayWave(PlayState DS_PlayState)(����+1)
** Purpose: DirectSound����Wave����
** Para: PlayState DS_PlayState(����״̬)
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
** Purpose: DirectSound������Ϸ����(ѭ��)
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
** Purpose: DirectSound������Ϸ��Ч(����)
*/
void DirectSound::PlaySoundEffect(LPWSTR lpszFileName)
{
	LoadWave(lpszFileName);
	m_pDirectSoundBuffer->Play(NULL, NULL, NULL);
}
