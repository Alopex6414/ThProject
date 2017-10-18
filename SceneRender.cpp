#include"Common.h"
#include "Include.h"
#include "SceneRender.h"

/*
** Function:	 SceneSig_Init()
** Purpose: Sig�ȴ�������Դ��ʼ��
*/
static void SceneSig_Init()
{
	g_pSubSig = new Subtitle(0.0f, 0.0f, 640.0f, 480.0f, 0.0f);//Sig����ҳ���ʼ��
	g_pSubLoad = new Subtitle(415.0f, 375.0f, 128.0f, 32.0f, 0.0f, 0.0f, false);//"��Ů����..."��ʼ��
	g_pSubLoadEn = new Subtitle(462.0f, 392.0f, 128.0f, 32.0f, 20.0f, 0.0f, true);//"Loading..."��ʼ��
}

/*
** Function:	 SceneSig_Update(float fTime)
** Purpose: Sig�ȴ���������
** Comment: �Ϻ�����˿������/"��Ů����..."
** Para: float fTime						//��ǰ��Ϸ��ʱ��
** Para: float fDeltaTime			//��ǰ��Ϸ����֡ʱ���
*/
void SceneSig_Update(float fTime, float fDeltaTime)
{
	if(fTime < 0.1f)//0.0f~0.1f(��Ϸ��ʼʱ��0.0s~0.1sʱ���ֺ�ɫ��Ļ,Alphaͨ��ֵΪ0.0f)
	{
		g_pSubSig->SetAlpha(0.0f);//(0.0s~0.1s Sig͸����AlphaΪ0.0f)
	}
	else if(fTime < 0.5f)//0.1f~0.5f(��Ϸ��ʼʱ��0.1s~0.5sʱSig��������,Alphaͨ��ֵ����)
	{
		g_pSubSig->AlphaIncrease(1.0f, 0.05f);//(0.1s~0.5s Sig͸����Alpha����0.05f,����20֡,ʱ��0.33s)
		g_pSubLoad->AlphaIncrease(1.0f, 0.1f);//(0.1s~0.5s "��Ů����..."͸����Alpha����0.1f,����10֡,ʱ��0.167s)
	}
	else//(��Ϸ��ʼʱ��0.5s~5.0sʱ,����Alphaͨ��ֵΪ1.0f)
	{
		g_pSubSig->SetAlpha(1.0f);//(0.5s~5.0s Sig͸����AlphaΪ1.0f)
		g_pSubLoad->AlphaCircle(0.5f, 1.0f, 0.02f);//"��Ů����..."͸����Alpha����0.02f,�仯��Χ0.5f->1.0f,����25֡,����仯
		g_pSubLoadEn->AlphaCircle(0.5f, 1.0f, 0.025f);//"loading..."͸����Alpha����0.025f,�仯��Χ1.0f->0.5f,����20֡,����仯
		g_pSubLoadEn->MoveXDecrease(0.0f, 1.0f);//"loading..."ƫ��λ��,��ʼXƫ��20,ƫ��X����ݼ�("loading..."�����ƶ�����"<-")
		g_pSnow->Update(fDeltaTime);//Snowѩ������ϵͳ...
	}

	#pragma region ���ö��㻺��
	//���ö��㻺������
	Vertex*pVertices = NULL;
	g_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	//Sig��������������
	pVertices[0] = Vertex(0.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubSig->GetAlpha()), 0.0f, 0.0f);
	pVertices[1] = Vertex(640.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubSig->GetAlpha()), (640.0f / 1024.0f), 0.0f);
	pVertices[2] = Vertex(640.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubSig->GetAlpha()), (640.0f / 1024.0f), (480.0f / 512.0f));
	pVertices[3] = Vertex(0.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubSig->GetAlpha()), 0.0f, (480.0f / 512.0f));

	//"��Ů����..."����������
	pVertices[4] = Vertex(415.0f, 375.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoad->GetAlpha()), 0.0f, 0.0f);
	pVertices[5] = Vertex(543.0f, 375.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoad->GetAlpha()), 1.0f, 0.0f);
	pVertices[6] = Vertex(543.0f, 407.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoad->GetAlpha()), 1.0f, 0.25f);
	pVertices[7] = Vertex(415.0f, 407.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoad->GetAlpha()), 0.0f, 0.25f);

	//"loading..."����������
	pVertices[8] = Vertex((462.0f + g_pSubLoadEn->GetMoveX()), 392.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoadEn->GetAlpha()), 0.0f, 0.25f);
	pVertices[9] = Vertex((590.0f + g_pSubLoadEn->GetMoveX()), 392.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoadEn->GetAlpha()), 1.0f, 0.25f);
	pVertices[10] = Vertex((590.0f + g_pSubLoadEn->GetMoveX()), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoadEn->GetAlpha()), 1.0f, 0.5f);
	pVertices[11] = Vertex((462.0f + g_pSubLoadEn->GetMoveX()), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoadEn->GetAlpha()), 0.0f, 0.5f);

	g_pVertexBuffer->Unlock();
	#pragma endregion

	#pragma region ������������
	//����������������
	WORD*pIndices = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);

	//Sig����������������
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	//"��Ů����..."������������
	pIndices[6] = 4;
	pIndices[7] = 5;
	pIndices[8] = 6;

	pIndices[9] = 4;
	pIndices[10] = 6;
	pIndices[11] = 7;

	//"loading..."������������
	pIndices[12] = 8;
	pIndices[13] = 9;
	pIndices[14] = 10;

	pIndices[15] = 8;
	pIndices[16] = 10;
	pIndices[17] = 11;

	g_pIndexBuffer->Unlock();
	#pragma endregion
}

/*
** Function:	 SceneSig_Render(float fTime)
** Purpose: Sig�ȴ�������Ⱦ
** Para: float fTime						//��ǰ��Ϸ��ʱ��
*/
void SceneSig_Render(float fTime)
{
	g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//����Alphaͨ�����
	g_pD3D9Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//Alpha�������Ϊͨ������
	g_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//Alpha���Ĭ��Դ�ں�����
	g_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//Alpha���Ĭ��Ŀ���ں�����

	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//Alpha�������ʽ
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);//Alpha���������Դ:��������ɫ//D3DTA_DIFFUSE//D3DTA_TEXTURE
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);//Alpha���������Դ:������ɫ

	g_pD3D9Device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex));//���ö�����������Դ
	g_pD3D9Device->SetFVF(D3DFVF_VERTEX);//���ö���ṹ��
	g_pD3D9Device->SetIndices(g_pIndexBuffer);//����������������Դ
	g_pD3D9Device->SetTexture(0, g_pTexture);//���ñ�������(����)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);//����ͼ��
	if(fTime >= 0.5f) g_pSnow->Render();//0.5s��ʼѩ��������Ⱦ
	g_pD3D9Device->SetTexture(0, g_pTextureLoading);//���ñ�������(�ȴ�)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 8, 0, 4);//����ͼ��
	g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//�ر�Alphaͨ�����
}

/*
** Function:	 SceneSig_Release()
** Purpose: Sig�ȴ�������Դ�ͷ�
*/
void SceneSig_Release()
{
	//�ͷ�Sig�������
	SAFE_DELETE(g_pSubSig);//�ͷ�Sig
	SAFE_DELETE(g_pSubLoad);//�ͷ�"��Ů����..."
	SAFE_DELETE(g_pSubLoadEn);//�ͷ�"Loading..."
}

/*
** Function:	 SceneTitle_Init()
** Purpose: Title��������Դ��ʼ��
*/
static void SceneTitle_Init()
{
	g_pSubTitle = new Subtitle(0.0f, 0.0f, 640.0f, 480.0f, 0.0f);//Title����ҳ���ʼ��(���������)
	g_pSubVersion = new Subtitle(304.0f, 431.0f, 64.0f, 16.0f, 0.0f);//Version�汾�ų�ʼ��(Ver1.00a)
	g_pSubGameOptionSelect = new Subtitle(540.0f, 256.0f, 128.0f, 32.0f, 0.0f, 0.0f, true);//GameOptionSelect��Ϸѡ����(��ǰ���������Ϸѡ��)
	g_pSubArrayGameOptions = new Subtitle[8]//GameOptions��Ϸѡ��(��������Ϸѡ��)
	{
		Subtitle(540.0f, 256.0f, 128.0f, 32.0f, 540.0f, 0.0f, true),//GameStartѡ��
		Subtitle(556.0f, 280.0f, 128.0f, 32.0f, 556.0f, 0.0f, true),//ExtraStartѡ��
		Subtitle(572.0f, 304.0f, 128.0f, 32.0f, 572.0f, 0.0f, true),//PracticeStartѡ��
		Subtitle(588.0f, 328.0f, 128.0f, 32.0f, 588.0f, 0.0f, true),//Replayѡ��
		Subtitle(604.0f, 352.0f, 128.0f, 32.0f, 604.0f, 0.0f, true),//PlayerDataѡ��
		Subtitle(620.0f, 376.0f, 128.0f, 32.0f, 620.0f, 0.0f, true),//MusicRoomѡ��
		Subtitle(636.0f, 400.0f, 128.0f, 32.0f, 636.0f, 0.0f, true),//Optionѡ��
		Subtitle(652.0f, 424.0f, 128.0f, 32.0f, 652.0f, 0.0f, true),//Quitѡ��
	};
}

/*
** Function:	 SceneTitle_Update()
** Purpose: Title�������������
** Comment: ��������Ⱦģʽ
** Para: HWND hWnd					//���ھ��
** Para: float fDeltaTime			//��ǰ��Ϸ����֡ʱ���
*/
void SceneTitle_Update(HWND hWnd, float fDeltaTime)
{
	static bool bKeySelect = false;		//Key��������
	static bool bKeySelectX = false;		//'X'��������
	static bool bKeySelectZ = false;		//'Z'��������
	static float fKeyPressTime = 0.0f;	//Key��������ʱ�����

	g_pDirectSound->PlayMusic();//���ű���BGM����

	if(g_nTitleFlagNow < 5)//���⶯��("�|�������")
	{
		g_pSubTitle->SetAlpha(0.0f);//Title���ⱳ��Alpha͸����0.0f(0%)
		g_pSubVersion->SetAlpha(0.0f);//Version�汾��Alpha͸����0.0f(0%)
		g_pSubGameOptionSelect->SetAlpha(0.5f);//��Ϸѡ����Alpha͸����0.5f(50%)(���ɼ�)
	}
	else if(g_nTitleFlagNow == 5)//���ⱳ���������
	{
		//���ⱳ���汾�ż���
		g_pSubTitle->AlphaIncrease(1.0f, 0.02f);//Title����͸����Alpha����0.02f,����50֡,0.83s
		if(g_pSubTitle->GetAlpha() >= 0.8f)
		{
			g_pSubVersion->AlphaIncrease(1.0f, 0.1f);//"V1.00a"͸����Alpha����0.1f,����10֡
		}

		//��Ϸѡ��˵�����(λ��)
		for(int i = 0; i < 8; i++)
		{
			g_pSubArrayGameOptions[i].MoveXDecrease(476.0f, 8.0f);//��Ϸѡ��˵�����8.0f�ٶ��ƶ�
		}

		//��Ϸѡ��˵�����(͸����)
		for(int i = 0; i < 8; i++)
		{
			if(g_pSubArrayGameOptions[i].GetAlpha() == 0.0f)//��������AlphaֵΪ0.0f
			{
				g_pSubArrayGameOptions[i].AddAlpha(0.1f);//AlphaֵΪ0.0f֮ǰ������0.1f
				break;
			}
			else
			{
				g_pSubArrayGameOptions[i].AlphaIncrease(1.0f, 0.1f);//Alphaֵȫ������0.1f
			}
		}

		//��Ϸ��ǰѡ����Alphaֵѭ���仯
		g_pSubGameOptionSelect->AlphaCircle(0.5f, 1.0f, 0.025f);//GameOptionSelect��Ϸѡ����Alpha͸���ȱ仯
	}
	else if(g_nTitleFlagNow == 6)//���������������(DirectInput�豸������Ӧ)(����KeyBoard)
	{
		g_pDirectInput->GetDeviceState();//��ȡ����״̬(KeyBoard)
		if(bKeySelect == false)//KeyBoard����δ����
		{
			#pragma region KeyPress��ⷽ���"��"״̬
			if(g_pDirectInput->IsKeyDown(DIK_UP))//��ⷽ���"��"״̬
			{
				bKeySelect = true;//KeyBoard�����Ѱ���
				g_nKeyNumber--;
				if(g_nKeyNumber < 0)
				{
					g_nKeyNumber = 7;
				}
				g_pSubArrayGameOptions[g_nKeyNumber].ResetMoveX();
				g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_select00.wav");//DirectSound���ز�����Чse_select00.wav
			}
			#pragma endregion

			#pragma region KeyPress��ⷽ���"��"״̬
			if(g_pDirectInput->IsKeyDown(DIK_DOWN))//��ⷽ���"��"״̬
			{
				bKeySelect = true;//KeyBoard�����Ѱ���
				g_nKeyNumber++;
				if(g_nKeyNumber > 7)
				{
					g_nKeyNumber = 0;
				}
				g_pSubArrayGameOptions[g_nKeyNumber].ResetMoveX();
				g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_select00.wav");//DirectSound���ز�����Чse_select00.wav
			}
			#pragma endregion

			#pragma region KeyPress���"Z"��״̬
			if(g_pDirectInput->IsKeyDown(DIK_Z))//���"Z"��״̬
			{
				if(bKeySelectZ == false)//'Z'��δ����
				{
					bKeySelect = true;//KeyBoard�����Ѱ���
					g_pSubArrayGameOptions[g_nKeyNumber].ResetMoveX();
					g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_ok00.wav");//DirectSound���ز�����Чse_ok00.wav
				}
				bKeySelectZ = true;//'Z'���Ѱ���
			}
			else
			{
				bKeySelectZ = false;//'Z'��δ����
			}
			#pragma endregion

			#pragma region KeyPress���"X"��/"Esc"��״̬
			if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//���"X"��/"Esc"��״̬
			{
				if(bKeySelectX == false)
				{
					bKeySelect = true;//KeyBoard�����Ѱ���
					if(g_nKeyNumber != 7)
					{
						g_nKeyNumber = 7;
						g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_cancel00.wav");//DirectSound���ز�����Чse_cancel00.wav
					}
					else
					{
						g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_cancel00.wav");//DirectSound���ز�����Чse_cancel00.wav
						Sleep(500);//�߳�Sleep500ms
						DestroyWindow(hWnd);//���ٴ���
					}
					g_pSubArrayGameOptions[g_nKeyNumber].ResetMoveX();
				}
				bKeySelectX = true;//'X'���Ѱ���
			}
			else
			{
				bKeySelectX = false;//'X'��δ����
			}
			#pragma endregion

			//��Ϸ��ǰѡ����Alphaֵѭ���仯
			g_pSubGameOptionSelect->AlphaCircle(0.5f, 1.0f, 0.025f);//GameOptionSelect��Ϸѡ����Alpha͸���ȱ仯
		}
		else//KeyBoard�����Ѱ���
		{
			#pragma region ��'Z'�����´���
			if(bKeySelectZ == false)//KeyPress�������·�'Z'��
			{
				fKeyPressTime += fDeltaTime;//�������ʱ����
				if(fKeyPressTime > 0.12f && g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXStatus() == true)
				{
					fKeyPressTime = 0.0f;//����ʱ������
					g_pSubArrayGameOptions[g_nKeyNumber].SetMoveXStatus(false);//MoveX�ƶ���־���
					bKeySelect = false;//KeyBoard����δ����
				}

				g_pSubArrayGameOptions[g_nKeyNumber].MoveXPendulum();//��Ϸѡ������˶�һ��
			}
			#pragma endregion

			#pragma region 'Z'�����´���
			else//KeyPress��������'Z'��
			{
				g_pSubArrayGameOptions[g_nKeyNumber].MoveXSelect();//��Ϸѡ������˸
				if(g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXFlashCount() >= 9)//9֡
				{
					//��Ϸѡ��˵��ƶ�����
					for(int i = 0; i < 8; i++)
					{
						if(i != g_nKeyNumber)//δѡ�е���Ϸѡ��˵��ƶ�
						{
							g_pSubArrayGameOptions[i].MoveXIncrease(640.0f, 4.0f);//δѡ����Ϸѡ��˵�����4.0f�ٶ��ƶ�
							g_pSubArrayGameOptions[i].AlphaDecrease(0.0f, 0.05f);//δѡ����Ϸѡ��˵�Alphaͨ��ֵ��0.05f�ݼ�
						}
						else//��ѡ�е���Ϸѡ��˵��ƶ�
						{
							if(g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXFlashCount() >= 13)//13֡
							{
								g_pSubArrayGameOptions[i].MoveXIncrease(640.0f, 8.0f);//��ѡ����Ϸѡ��˵�����8.0f�ٶ��ƶ�
								g_pSubArrayGameOptions[i].AlphaDecrease(0.0f, 0.075f);//��ѡ����Ϸѡ��˵�Alphaͨ��ֵ��0.075f�ݼ�
							}
						}
					}

					//��Ϸѡ��˵���ֹ��Ⱦ
					if(g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXFlashCount() >= 13)//13֡
					{
						g_pSubArrayGameOptions[g_nKeyNumber].SetFlashState(false);//��ֹ��Ⱦ(��˸)
					}

					//��Ϸѡ��˵�����
					if(g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXFlashCount() >= 20)//20֡
					{
						fKeyPressTime = 0.0f;//����ʱ������
						g_pSubArrayGameOptions[g_nKeyNumber].ResetSelectFlash();//���ѡ�б�־
						bKeySelect = false;//KeyBoard����δ����

						g_nKeySelectState = 1;//ת�����������

						switch(g_nKeyNumber)
						{
						case 0://GameStart(Key����������)
							break;
						case 1://ExtraStart(Key����������)
							break;
						case 2://PracticeStart(Key����������)
							break;
						case 3://Replay(Key����������)
							break;
						case 4://PlayerData(Key����������)
							break;
						case 5://MusicRoom(Key����������)
							break;
						case 6://Option(Key����������)
							//g_nKeyNumber_Option = 0;//�Ӳ˵�ѡ��
							g_bKeySelectTitleChange = false;
							for(int i = 0; i < 5; i++)//��ʼ��ѡ�����ò˵�(�ָ�)
							{
								g_pSubArrayOptionsConfig[i].SetMoveX(540.0f + i * 16.0f);//ѡ�������ַ��ָ���ʼ״̬
								g_pSubArrayOptionsConfig[i].SetAlpha(0.0f);//ѡ������Alpha͸���ȱ仯
							}
							break;
						case 7://Quit(ExitGame�˳���Ϸ)
							Sleep(500);//�߳�Sleep500ms
							DestroyWindow(hWnd);//���ٴ���
							break;
						default:
							break;
						}
					}
				}
			}
			#pragma endregion

			g_bKeySelectFlashState = g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXFlashState();
		}
	}

	#pragma region ���ö��㻺��
	//���ö��㻺������
	Vertex*pVertices = NULL;
	g_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	//Title00��������������
	pVertices[0] = Vertex(0.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, 0.0f);
	pVertices[1] = Vertex(640.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), 0.0f);
	pVertices[2] = Vertex(640.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), (480.0f / 512.0f));
	pVertices[3] = Vertex(0.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, (480.0f / 512.0f));

	//"Copyright"����������
	pVertices[4] = Vertex(22.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), 0.0f);
	pVertices[5] = Vertex(227.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), 0.0f);
	pVertices[6] = Vertex(227.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), (16.0f / 256.0f));
	pVertices[7] = Vertex(22.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), (16.0f / 256.0f));

	//"Ver1.00a"����������
	pVertices[8] = Vertex(304.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 0.0f);
	pVertices[9] = Vertex(368.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 0.0f);
	pVertices[10] = Vertex(368.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 1.0f);
	pVertices[11] = Vertex(304.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 1.0f);

	//��Ϸѡ��˵���Ⱦ
	for(int i = 0; i < 8; i++)
	{
		pVertices[12 + i * 4] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (128.0f / 512.0f), (i * 32.0f / 512.0f));
		pVertices[12 + i * 4 + 1] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + 128 + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (256.0f / 512.0f), (i * 32.0f / 512.0f));
		pVertices[12 + i * 4 + 2] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + 128 + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (256.0f / 512.0f), ((i + 1) * 32.0f / 512.0f));
		pVertices[12 + i * 4 + 3] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (128.0f / 512.0f), ((i + 1) * 32.0f / 512.0f));
	}

	//��Ϸѡ��ѡ������Ⱦ
	if(bKeySelectZ == false)//'Z'��δ����
	{
		pVertices[44] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
		pVertices[45] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
		pVertices[46] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
		pVertices[47] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
	}
	else//'Z'���Ѱ���
	{
		pVertices[44] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
		pVertices[45] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
		pVertices[46] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
		pVertices[47] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
	}

	g_pVertexBuffer->Unlock();
	#pragma endregion

	#pragma region ������������
	//����������������
	WORD*pIndices = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);

	//Title00����������������
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	//"Copyright"������������
	pIndices[6] = 4;
	pIndices[7] = 5;
	pIndices[8] = 6;

	pIndices[9] = 4;
	pIndices[10] = 6;
	pIndices[11] = 7;

	//"Ver1.00a"������������
	pIndices[12] = 8;
	pIndices[13] = 9;
	pIndices[14] = 10;

	pIndices[15] = 8;
	pIndices[16] = 10;
	pIndices[17] = 11;

	//"GameStart"������������
	pIndices[18] = 12;
	pIndices[19] = 13;
	pIndices[20] = 14;

	pIndices[21] = 12;
	pIndices[22] = 14;
	pIndices[23] = 15;

	//"ExtraStart"������������
	pIndices[24] = 16;
	pIndices[25] = 17;
	pIndices[26] = 18;

	pIndices[27] = 16;
	pIndices[28] = 18;
	pIndices[29] = 19;

	//"PracticeStart"������������
	pIndices[30] = 20;
	pIndices[31] = 21;
	pIndices[32] = 22;

	pIndices[33] = 20;
	pIndices[34] = 22;
	pIndices[35] = 23;

	//"Replay"������������
	pIndices[36] = 24;
	pIndices[37] = 25;
	pIndices[38] = 26;

	pIndices[39] = 24;
	pIndices[40] = 26;
	pIndices[41] = 27;

	//"PlayerData"������������
	pIndices[42] = 28;
	pIndices[43] = 29;
	pIndices[44] = 30;

	pIndices[45] = 28;
	pIndices[46] = 30;
	pIndices[47] = 31;

	//"MusicRoom"������������
	pIndices[48] = 32;
	pIndices[49] = 33;
	pIndices[50] = 34;

	pIndices[51] = 32;
	pIndices[52] = 34;
	pIndices[53] = 35;

	//"Option"������������
	pIndices[54] = 36;
	pIndices[55] = 37;
	pIndices[56] = 38;

	pIndices[57] = 36;
	pIndices[58] = 38;
	pIndices[59] = 39;

	//"Quit"������������
	pIndices[60] = 40;
	pIndices[61] = 41;
	pIndices[62] = 42;

	pIndices[63] = 40;
	pIndices[64] = 42;
	pIndices[65] = 43;

	//��Ϸѡ��ѡ��������������
	pIndices[66] = 44;
	pIndices[67] = 45;
	pIndices[68] = 46;

	pIndices[69] = 44;
	pIndices[70] = 46;
	pIndices[71] = 47;

	g_pIndexBuffer->Unlock();
	#pragma endregion
}

/*
** Function:	 SceneTitle_Render()
** Purpose: Title�����������Ⱦ
** Comment: ��������Ⱦģʽ
** Para: float fDeltaTime			//��ǰ��Ϸ����֡ʱ���
*/
void SceneTitle_Render(float fDeltaTime)
{
	g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//����Alphaͨ�����
	g_pD3D9Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//Alpha�������Ϊͨ������
	g_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//Alpha���Ĭ��Դ�ں�����
	g_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//Alpha���Ĭ��Ŀ���ں�����

	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//Alpha�������ʽ
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);//Alpha���������Դ:��������ɫ//D3DTA_DIFFUSE//D3DTA_TEXTURE
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);//Alpha���������Դ:������ɫ
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);//Color�������ʽ
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//Color���ѡ��������ɫ

	//���Ʊ���
	g_pD3D9Device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex));//���ö�����������Դ
	g_pD3D9Device->SetFVF(D3DFVF_VERTEX);//���ö���ṹ��
	g_pD3D9Device->SetIndices(g_pIndexBuffer);//����������������Դ
	g_pD3D9Device->SetTexture(0, g_pTextureTitle);//���ñ�������(����)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);//����ͼ��

	//��������
	g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//Color�������TFACTOR��ɫ��ɫ
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);//Color�������ʽ
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);//Color���ѡ��������ɫ
	g_pD3D9Device->SetTexture(0, g_pTextureLogo);//���ñ�������(LOGO��־)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 4, 0, 2);//����ͼ��
	g_pD3D9Device->SetTexture(0, g_pTextureVersion);//���ñ�������(�汾��Ver1.00a)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8, 0, 4, 0, 2);//����ͼ��

	//������Ϸѡ��
	g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 0.65f, 0.65f, 1.0f));//Color�������TFACTOR��ɫǳ�ۺ�ɫ
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//Color�������ʽ(���)
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//Color�������
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);//Color���TFACTOR��ɫ
	g_pD3D9Device->SetTexture(0, g_pTextureTitleText);//���ñ�������(��Ϸ�˵�ѡ��)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 12, 0, 32, 0, 16);//����ͼ��
	if(g_bKeySelectFlashState == false)//��Ϸѡ������Ⱦģʽ
	{
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);//Color�������ʽ
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//Color�������
	}
	else
	{
		g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));//Color�������TFACTOR��ɫ����ɫ
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//Color�������(���)
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//Color�������
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);//Color���TFACTOR��ɫ
	}
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 44, 0, 4, 0, 2);//����ͼ��

	//���ƾ��鶯��
	g_nTitleFlagNow = g_pAnimation->RenderTitle(fDeltaTime);//"�|����둵�"Sprite����
	
	//�ر�Alpha���
	g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//�ر�Alpha���
}

/*
** Function:	 SceneTitle_Release()
** Purpose: Title��������Դ�ͷ�
*/
void SceneTitle_Release()
{
	SAFE_DELETE(g_pSubTitle);//�ͷ�Title����ָ��
	SAFE_DELETE(g_pSubVersion);//�ͷ�Version�汾��ָ��
	SAFE_DELETE(g_pSubGameOptionSelect);//�ͷ�GameOptionSelect��Ϸѡ����
	SAFE_DELETE_ARRAY(g_pSubArrayGameOptions);//�ͷ�GameOptions��Ϸѡ��
}

/*
** Function:	 SceneOption_Init()
** Purpose: Title���������(Option����)��Դ��ʼ��
*/
static void SceneOption_Init()
{
	g_pSubTitle = new Subtitle(0.0f, 0.0f, 640.0f, 480.0f, 0.0f);//Title����ҳ���ʼ��(���������)
	g_pSubVersion = new Subtitle(304.0f, 431.0f, 64.0f, 16.0f, 0.0f);//Version�汾�ų�ʼ��(Ver1.00a)
	g_pSubOptionConfigSelect = new Subtitle(540.0f, 304.0f, 128.0f, 32.0f, 540.0f, 0.5f, true);//OptionConfigѡ�����ʼ��
	g_pSubArrayOptionsConfig = new Subtitle[5]//OptionsConfigѡ�����ó�ʼ��(Option����)
	{
		Subtitle(540.0f, 304.0f, 128.0f, 32.0f, 540.0f, 0.0f, true),//BGM Volumeѡ��
		Subtitle(556.0f, 328.0f, 128.0f, 32.0f, 556.0f, 0.0f, true),//SE Volumeѡ��
		Subtitle(572.0f, 352.0f, 128.0f, 32.0f, 572.0f, 0.0f, true),//Key Configѡ��
		Subtitle(588.0f, 376.0f, 128.0f, 32.0f, 588.0f, 0.0f, true),//Defaultѡ��
		Subtitle(604.0f, 400.0f, 128.0f, 32.0f, 604.0f, 0.0f, true),//Quitѡ��
	};
}

/*
** Function:	 SceneOption_Update()
** Purpose: Title���������(Option����)����
** Comment: Option������Ⱦģʽ(��Ϸѡ��)
** Para: float fDeltaTime			//��ǰ��Ϸ����֡ʱ���
*/
void SceneOption_Update(float fDeltaTime)
{
	static bool bKeySelect = false;		//Key��������
	static bool bKeySelectX = false;		//'X'��������
	static bool bKeySelectZ = false;		//'Z'��������
	static bool bKeyEnableFlag = false;	//Key����ʹ�ܱ�־(true:���԰���/false:��ֹ����)
	static bool bKeyBackSpace = false;	//Key������������������־
	static float fKeyPressTime = 0.0f;	//Key��������ʱ�����
	static int nKeySelectFlashTimesCount = 0;//Keyѡ�ж�������

	//����KeyBoard����(KeyBoard Process)
	#pragma region ����KeyBoard����
	g_pDirectInput->GetDeviceState();//��ȡ����״̬(KeyBoard)
	if(bKeySelect == false)//KeyBoard����δ����
	{
		//����KeyBoard��Ӧ
		#pragma region ����KeyBoardʹ�ܼ��
		bKeyEnableFlag = true;//����KeyBoardʹ��
		for(int i = 0; i < 5; i++)
		{
			if(g_pSubArrayOptionsConfig[i].GetMoveX() != 444.0f)//ѡ������δ�������
			{
				bKeyEnableFlag = false;//����KeyBoard��ֹ
				break;
			}
		}
		#pragma endregion

		//Optionѡ��˵�����
		#pragma region Optionѡ��˵�����
		//���ⱳ���汾�ż���
		g_pSubTitle->SetAlpha(1.0f);//Title���ⱳ��Alpha͸����1.0f(100%)
		g_pSubVersion->SetAlpha(1.0f);//Version�汾��Alpha͸����1.0f(100%)

		//ѡ�����ò˵�����(λ��)
		for(int i = 0; i < 5; i++)
		{
			g_pSubArrayOptionsConfig[i].MoveXDecrease(444.0f, 8.0f);//ѡ�����ò˵�����8.0f�ٶ��ƶ�
		}

		//ѡ�����ò˵�����(͸����)
		for(int i = 0; i < 5; i++)
		{
			if(g_pSubArrayOptionsConfig[i].GetAlpha() == 0.0f)//��������AlphaֵΪ0.0f
			{
				g_pSubArrayOptionsConfig[i].AddAlpha(0.1f);//AlphaֵΪ0.0f֮ǰ������0.1f
				break;
			}
			else
			{
				g_pSubArrayOptionsConfig[i].AlphaIncrease(1.0f, 0.1f);//Alphaֵȫ������0.1f
			}
		}

		//ѡ�����õ�ǰѡ����Alphaֵѭ���仯
		g_pSubOptionConfigSelect->AlphaCircle(0.5f, 1.0f, 0.025f);//OptionConfigSelectѡ������ѡ����Alpha͸���ȱ仯
		#pragma endregion

		if(bKeyEnableFlag == true)//����KeyBoardʹ��
		{
			#pragma region KeyPress��ⷽ���"��"״̬
			if(g_pDirectInput->IsKeyDown(DIK_UP))//��ⷽ���"��"״̬
			{
				bKeySelect = true;//KeyBoard�����Ѱ���
				g_nKeyNumber_Option--;
				if(g_nKeyNumber_Option < 0)
				{
					g_nKeyNumber_Option = 4;
				}
				g_pSubArrayOptionsConfig[g_nKeyNumber_Option].ResetMoveX();
				g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_select00.wav");//DirectSound���ز�����Чse_select00.wav
			}
			#pragma endregion

			#pragma region KeyPress��ⷽ���"��"״̬
			if(g_pDirectInput->IsKeyDown(DIK_DOWN))//��ⷽ���"��"״̬
			{
				bKeySelect = true;//KeyBoard�����Ѱ���
				g_nKeyNumber_Option++;
				if(g_nKeyNumber_Option > 4)
				{
					g_nKeyNumber_Option = 0;
				}
				g_pSubArrayOptionsConfig[g_nKeyNumber_Option].ResetMoveX();
				g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_select00.wav");//DirectSound���ز�����Чse_select00.wav
			}
			#pragma endregion

			#pragma region KeyPress���"Z"��״̬
			if(g_pDirectInput->IsKeyDown(DIK_Z))//���"Z"��״̬
			{
				if(bKeySelectZ == false)//'Z'��δ����
				{
					bKeySelect = true;//KeyBoard�����Ѱ���
					if(g_nKeyNumber_Option == 4)//Quit
					{
						bKeyBackSpace = true;//�˸��־��λ
						g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_cancel00.wav");//DirectSound���ز�����Чse_cancel00.wav
					}
					else
					{
						g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_ok00.wav");//DirectSound���ز�����Чse_ok00.wav
					}
					g_pSubArrayOptionsConfig[g_nKeyNumber_Option].ResetMoveX();
				}
				bKeySelectZ = true;//'Z'���Ѱ���
			}
			else
			{
				bKeySelectZ = false;//'Z'��δ����
			}
			#pragma endregion

			#pragma region KeyPress���"X"��/"Esc"��״̬
			if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//���"X"��/"Esc"��״̬
			{
				if(bKeySelectX == false)
				{
					bKeySelect = true;//KeyBoard�����Ѱ���
					if(g_nKeyNumber_Option != 4)
					{
						g_nKeyNumber_Option = 4;
					}
					else
					{
						bKeyBackSpace = true;//�˸��־��λ
					}
					g_pSubArrayOptionsConfig[g_nKeyNumber_Option].ResetMoveX();
					g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_cancel00.wav");//DirectSound���ز�����Чse_cancel00.wav
				}
				bKeySelectX = true;//'X'���Ѱ���
			}
			else
			{
				bKeySelectX = false;//'X'��δ����
			}
			#pragma endregion
		}
	}
	else//KeyBoard�����Ѱ���
	{
		#pragma region �˸��־δ��λ
		if(bKeyBackSpace == false)//�˸��־δ��λ
		{
			#pragma region 'Z'��δ����
			if(bKeySelect == false)//'Z'��δ����
			{
				fKeyPressTime += fDeltaTime;//�������ʱ����
				if(fKeyPressTime > 0.12f && g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXStatus() == true)
				{
					fKeyPressTime = 0.0f;//����ʱ������
					g_pSubArrayOptionsConfig[g_nKeyNumber_Option].SetMoveXStatus(false);//MoveX�ƶ���־���
					bKeySelect = false;//KeyBoard����δ����
				}

				g_pSubArrayOptionsConfig[g_nKeyNumber_Option].MoveXPendulum();//��Ϸѡ������˶�һ��
			}
			#pragma endregion

			#pragma region 'Z'���Ѱ���
			else//'Z'���Ѱ���
			{
				fKeyPressTime += fDeltaTime;//�������ʱ����
				if(fKeyPressTime > 0.12f && g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXStatus() == true)
				{
					fKeyPressTime = 0.0f;//����ʱ������
					g_pSubArrayOptionsConfig[g_nKeyNumber_Option].SetMoveXStatus(false);//MoveX�ƶ���־���
					bKeySelect = false;//KeyBoard����δ����
				}

				g_pSubArrayOptionsConfig[g_nKeyNumber_Option].MoveXPendulum();//��Ϸѡ������˶�һ��
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region �˸��־����λ
		else//�˸��־����λ
		{
			if(g_bKeySelectTitleChange == false)
			{
				g_pSubArrayOptionsConfig[g_nKeyNumber_Option].MoveXSelect();//��Ϸѡ������˸
				if(g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXFlashCount() >= 9)//9֡
				{
					//ѡ�����ò˵��ƶ�����
					for(int i = 0; i < 5; i++)
					{
						if(i != g_nKeyNumber_Option)//δѡ�е���Ϸѡ��˵��ƶ�
						{
							g_pSubArrayOptionsConfig[i].MoveXIncrease(640.0f, 4.0f);//δѡ����Ϸѡ��˵�����4.0f�ٶ��ƶ�
							g_pSubArrayOptionsConfig[i].AlphaDecrease(0.0f, 0.05f);//δѡ����Ϸѡ��˵�Alphaͨ��ֵ��0.05f�ݼ�
						}
						else//��ѡ�е���Ϸѡ��˵��ƶ�
						{
							if(g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXFlashCount() >= 13)//13֡
							{
								g_pSubArrayOptionsConfig[i].MoveXIncrease(640.0f, 8.0f);//��ѡ����Ϸѡ��˵�����8.0f�ٶ��ƶ�
								g_pSubArrayOptionsConfig[i].AlphaDecrease(0.0f, 0.075f);//��ѡ����Ϸѡ��˵�Alphaͨ��ֵ��0.075f�ݼ�
							}
						}
					}

					//ѡ�����ò˵���ֹ��Ⱦ
					if(g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXFlashCount() >= 13)//13֡
					{
						g_pSubArrayOptionsConfig[g_nKeyNumber_Option].SetFlashState(false);//��ֹ��Ⱦ(��˸)
					}

					//ѡ�����ò˵�����
					if(g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXFlashCount() >= 20)//20֡
					{
						fKeyPressTime = 0.0f;//����ʱ������
						g_pSubArrayOptionsConfig[g_nKeyNumber_Option].ResetSelectFlash();//���ѡ�б�־
						g_bKeySelectTitleChange = true;

						for(int i = 0; i < 8; i++)
						{
							g_pSubArrayGameOptions[i].SetMoveX(540.0f + i * 16.0f);
							g_pSubArrayGameOptions[i].SetAlpha(0.0f);
						}
					}
				}
				g_bKeySelectFlashState = g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXFlashState();
			}
			else
			{
				//��Ϸѡ��˵�����(λ��)
				for(int i = 0; i < 8; i++)
				{
					g_pSubArrayGameOptions[i].MoveXDecrease(476.0f, 8.0f);//ѡ�����ò˵�����8.0f�ٶ��ƶ�
				}

				//��Ϸѡ��˵�����(͸����)
				for(int i = 0; i < 8; i++)
				{
					if(g_pSubArrayGameOptions[i].GetAlpha() == 0.0f)//��������AlphaֵΪ0.0f
					{
						g_pSubArrayGameOptions[i].AddAlpha(0.0625f);//AlphaֵΪ0.0f֮ǰ������0.1f
						break;
					}
					else
					{
						g_pSubArrayGameOptions[i].AlphaIncrease(1.0f, 0.0625f);//Alphaֵȫ������0.1f
					}
				}

				nKeySelectFlashTimesCount++;
				if(nKeySelectFlashTimesCount >= 24)
				{
					nKeySelectFlashTimesCount = 0;
					fKeyPressTime = 0.0f;//����ʱ������
					bKeySelect = false;//KeyBoard����δ����
					bKeyBackSpace = false;//Key������������������־���
					g_nKeySelectState = 0;//ת�����������
				}
			}
		}
		#pragma endregion
	}
	#pragma endregion

	#pragma region ���ö��㻺��
	//���ö��㻺������
	Vertex*pVertices = NULL;
	g_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	//Title00��������������
	pVertices[0] = Vertex(0.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, 0.0f);
	pVertices[1] = Vertex(640.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), 0.0f);
	pVertices[2] = Vertex(640.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), (480.0f / 512.0f));
	pVertices[3] = Vertex(0.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, (480.0f / 512.0f));

	//"Copyright"����������
	pVertices[4] = Vertex(22.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), 0.0f);
	pVertices[5] = Vertex(227.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), 0.0f);
	pVertices[6] = Vertex(227.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), (16.0f / 256.0f));
	pVertices[7] = Vertex(22.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), (16.0f / 256.0f));

	//"Ver1.00a"����������
	pVertices[8] = Vertex(304.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 0.0f);
	pVertices[9] = Vertex(368.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 0.0f);
	pVertices[10] = Vertex(368.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 1.0f);
	pVertices[11] = Vertex(304.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 1.0f);

	if(g_bKeySelectTitleChange == false)
	{
		//ѡ�����ò˵���Ⱦ
		#pragma region "BGM Volume"����������
		//"BGM Volume"����������
		pVertices[12] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[0].GetAlpha()), (128.0f / 512.0f), (256.0f / 512.0f));
		pVertices[13] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + 128 + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[0].GetAlpha()), (256.0f / 512.0f), (256.0f / 512.0f));
		pVertices[14] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + 128 + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[0].GetAlpha()), (256.0f / 512.0f), (288.0f / 512.0f));
		pVertices[15] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[0].GetAlpha()), (128.0f / 512.0f), (288.0f / 512.0f));
		#pragma endregion

		#pragma region "SE Volume"����������
		//"SE Volume"����������
		pVertices[16] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[1].GetAlpha()), (128.0f / 512.0f), (288.0f / 512.0f));
		pVertices[17] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + 128 + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[1].GetAlpha()), (256.0f / 512.0f), (288.0f / 512.0f));
		pVertices[18] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + 128 + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[1].GetAlpha()), (256.0f / 512.0f), (320.0f / 512.0f));
		pVertices[19] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[1].GetAlpha()), (128.0f / 512.0f), (320.0f / 512.0f));
		#pragma endregion

		#pragma region "Key Config"����������
		//"Key Config"����������
		pVertices[20] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[2].GetAlpha()), (128.0f / 512.0f), (352.0f / 512.0f));
		pVertices[21] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + 128 + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[2].GetAlpha()), (256.0f / 512.0f), (352.0f / 512.0f));
		pVertices[22] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + 128 + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[2].GetAlpha()), (256.0f / 512.0f), (384.0f / 512.0f));
		pVertices[23] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[2].GetAlpha()), (128.0f / 512.0f), (384.0f / 512.0f));
		#pragma endregion

		#pragma region "Default"����������
		//"Default"����������
		pVertices[24] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[3].GetAlpha()), (128.0f / 512.0f), (320.0f / 512.0f));
		pVertices[25] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + 128 + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[3].GetAlpha()), (256.0f / 512.0f), (320.0f / 512.0f));
		pVertices[26] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + 128 + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[3].GetAlpha()), (256.0f / 512.0f), (352.0f / 512.0f));
		pVertices[27] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[3].GetAlpha()), (128.0f / 512.0f), (352.0f / 512.0f));
		#pragma endregion

		#pragma region "Quit"����������
		//"Quit"����������
		pVertices[28] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[4].GetAlpha()), (128.0f / 512.0f), (224.0f / 512.0f));
		pVertices[29] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + 128 + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[4].GetAlpha()), (256.0f / 512.0f), (224.0f / 512.0f));
		pVertices[30] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + 128 + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[4].GetAlpha()), (256.0f / 512.0f), (256.0f / 512.0f));
		pVertices[31] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[4].GetAlpha()), (128.0f / 512.0f), (256.0f / 512.0f));
		#pragma endregion

		#pragma region ѡ������ѡ������Ⱦ
		//ѡ������ѡ������Ⱦ
		if(bKeyBackSpace == false)//'BackSpace'��δ����
		{
			if(g_nKeyNumber_Option == 0)
			{
				pVertices[32] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[0].GetAlpha()), (0.0f / 512.0f), (256.0f / 512.0f));
				pVertices[33] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + 128 + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[0].GetAlpha()), (128.0f / 512.0f), (256.0f / 512.0f));
				pVertices[34] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + 128 + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[0].GetAlpha()), (128.0f / 512.0f), (288.0f / 512.0f));
				pVertices[35] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[0].GetAlpha()), (0.0f / 512.0f), (288.0f / 512.0f));
			}
			else if(g_nKeyNumber_Option == 1)
			{
				pVertices[32] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[1].GetAlpha()), (0.0f / 512.0f), (288.0f / 512.0f));
				pVertices[33] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + 128 + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[1].GetAlpha()), (128.0f / 512.0f), (288.0f / 512.0f));
				pVertices[34] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + 128 + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[1].GetAlpha()), (128.0f / 512.0f), (320.0f / 512.0f));
				pVertices[35] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[1].GetAlpha()), (0.0f / 512.0f), (320.0f / 512.0f));
			}
			else if(g_nKeyNumber_Option == 2)
			{
				pVertices[32] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[2].GetAlpha()), (0.0f / 512.0f), (352.0f / 512.0f));
				pVertices[33] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + 128 + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[2].GetAlpha()), (128.0f / 512.0f), (352.0f / 512.0f));
				pVertices[34] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + 128 + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[2].GetAlpha()), (128.0f / 512.0f), (384.0f / 512.0f));
				pVertices[35] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[2].GetAlpha()), (0.0f / 512.0f), (384.0f / 512.0f));
			}
			else if(g_nKeyNumber_Option == 3)
			{
				pVertices[32] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[3].GetAlpha()), (0.0f / 512.0f), (320.0f / 512.0f));
				pVertices[33] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + 128 + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[3].GetAlpha()), (128.0f / 512.0f), (320.0f / 512.0f));
				pVertices[34] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + 128 + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[3].GetAlpha()), (128.0f / 512.0f), (352.0f / 512.0f));
				pVertices[35] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[3].GetAlpha()), (0.0f / 512.0f), (352.0f / 512.0f));
			}
			else if(g_nKeyNumber_Option == 4)
			{
				pVertices[32] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[4].GetAlpha()), (0.0f / 512.0f), (224.0f / 512.0f));
				pVertices[33] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + 128 + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[4].GetAlpha()), (128.0f / 512.0f), (224.0f / 512.0f));
				pVertices[34] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + 128 + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[4].GetAlpha()), (128.0f / 512.0f), (256.0f / 512.0f));
				pVertices[35] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubOptionConfigSelect->GetAlpha() * g_pSubArrayOptionsConfig[4].GetAlpha()), (0.0f / 512.0f), (256.0f / 512.0f));
			}
		}
		else//'BackSpace'���Ѱ���
		{
			if(g_nKeyNumber_Option == 0)
			{
				pVertices[32] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[0].GetAlpha()), (0.0f / 512.0f), (256.0f / 512.0f));
				pVertices[33] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + 128 + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[0].GetAlpha()), (128.0f / 512.0f), (256.0f / 512.0f));
				pVertices[34] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + 128 + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[0].GetAlpha()), (128.0f / 512.0f), (288.0f / 512.0f));
				pVertices[35] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[0].GetAlpha()), (0.0f / 512.0f), (288.0f / 512.0f));
			}
			else if(g_nKeyNumber_Option == 1)
			{
				pVertices[32] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[1].GetAlpha()), (0.0f / 512.0f), (288.0f / 512.0f));
				pVertices[33] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + 128 + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[1].GetAlpha()), (128.0f / 512.0f), (288.0f / 512.0f));
				pVertices[34] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + 128 + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[1].GetAlpha()), (128.0f / 512.0f), (320.0f / 512.0f));
				pVertices[35] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[1].GetAlpha()), (0.0f / 512.0f), (320.0f / 512.0f));
			}
			else if(g_nKeyNumber_Option == 2)
			{
				pVertices[32] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[2].GetAlpha()), (0.0f / 512.0f), (352.0f / 512.0f));
				pVertices[33] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + 128 + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[2].GetAlpha()), (128.0f / 512.0f), (352.0f / 512.0f));
				pVertices[34] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + 128 + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[2].GetAlpha()), (128.0f / 512.0f), (384.0f / 512.0f));
				pVertices[35] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[2].GetAlpha()), (0.0f / 512.0f), (384.0f / 512.0f));
			}
			else if(g_nKeyNumber_Option == 3)
			{
				pVertices[32] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[3].GetAlpha()), (0.0f / 512.0f), (320.0f / 512.0f));
				pVertices[33] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + 128 + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[3].GetAlpha()), (128.0f / 512.0f), (320.0f / 512.0f));
				pVertices[34] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + 128 + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[3].GetAlpha()), (128.0f / 512.0f), (352.0f / 512.0f));
				pVertices[35] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[3].GetAlpha()), (0.0f / 512.0f), (352.0f / 512.0f));
			}
			else if(g_nKeyNumber_Option == 4)
			{
				pVertices[32] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[4].GetAlpha()), (0.0f / 512.0f), (224.0f / 512.0f));
				pVertices[33] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + 128 + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[4].GetAlpha()), (128.0f / 512.0f), (224.0f / 512.0f));
				pVertices[34] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + 128 + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[4].GetAlpha()), (128.0f / 512.0f), (256.0f / 512.0f));
				pVertices[35] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[4].GetAlpha()), (0.0f / 512.0f), (256.0f / 512.0f));
			}
		}
		#pragma endregion
	}
	else
	{
		//��Ϸѡ��˵���Ⱦ
		for(int i = 0; i < 8; i++)
		{
			pVertices[12 + i * 4] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (128.0f / 512.0f), (i * 32.0f / 512.0f));
			pVertices[12 + i * 4 + 1] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + 128 + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (256.0f / 512.0f), (i * 32.0f / 512.0f));
			pVertices[12 + i * 4 + 2] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + 128 + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (256.0f / 512.0f), ((i + 1) * 32.0f / 512.0f));
			pVertices[12 + i * 4 + 3] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (128.0f / 512.0f), ((i + 1) * 32.0f / 512.0f));
		}

		//��Ϸѡ��ѡ������Ⱦ
		if(bKeySelectZ == false)//'Z'��δ����
		{
			pVertices[44] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
			pVertices[45] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
			pVertices[46] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
			pVertices[47] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
		}
		else//'Z'���Ѱ���
		{
			pVertices[44] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
			pVertices[45] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
			pVertices[46] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
			pVertices[47] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
		}
	}

	g_pVertexBuffer->Unlock();
	#pragma endregion

	#pragma region ������������
	//����������������
	WORD*pIndices = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);

	//Title00����������������
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	//"Copyright"������������
	pIndices[6] = 4;
	pIndices[7] = 5;
	pIndices[8] = 6;

	pIndices[9] = 4;
	pIndices[10] = 6;
	pIndices[11] = 7;

	//"Ver1.00a"������������
	pIndices[12] = 8;
	pIndices[13] = 9;
	pIndices[14] = 10;

	pIndices[15] = 8;
	pIndices[16] = 10;
	pIndices[17] = 11;

	//"GameStart"������������
	pIndices[18] = 12;
	pIndices[19] = 13;
	pIndices[20] = 14;

	pIndices[21] = 12;
	pIndices[22] = 14;
	pIndices[23] = 15;

	//"ExtraStart"������������
	pIndices[24] = 16;
	pIndices[25] = 17;
	pIndices[26] = 18;

	pIndices[27] = 16;
	pIndices[28] = 18;
	pIndices[29] = 19;

	//"PracticeStart"������������
	pIndices[30] = 20;
	pIndices[31] = 21;
	pIndices[32] = 22;

	pIndices[33] = 20;
	pIndices[34] = 22;
	pIndices[35] = 23;

	//"Replay"������������
	pIndices[36] = 24;
	pIndices[37] = 25;
	pIndices[38] = 26;

	pIndices[39] = 24;
	pIndices[40] = 26;
	pIndices[41] = 27;

	//"PlayerData"������������
	pIndices[42] = 28;
	pIndices[43] = 29;
	pIndices[44] = 30;

	pIndices[45] = 28;
	pIndices[46] = 30;
	pIndices[47] = 31;

	//"MusicRoom"������������
	pIndices[48] = 32;
	pIndices[49] = 33;
	pIndices[50] = 34;

	pIndices[51] = 32;
	pIndices[52] = 34;
	pIndices[53] = 35;

	//"Option"������������
	pIndices[54] = 36;
	pIndices[55] = 37;
	pIndices[56] = 38;

	pIndices[57] = 36;
	pIndices[58] = 38;
	pIndices[59] = 39;

	//"Quit"������������
	pIndices[60] = 40;
	pIndices[61] = 41;
	pIndices[62] = 42;

	pIndices[63] = 40;
	pIndices[64] = 42;
	pIndices[65] = 43;

	//��Ϸѡ��ѡ��������������
	pIndices[66] = 44;
	pIndices[67] = 45;
	pIndices[68] = 46;

	pIndices[69] = 44;
	pIndices[70] = 46;
	pIndices[71] = 47;

	g_pIndexBuffer->Unlock();
	#pragma endregion

}

/*
** Function:	 SceneOption_Render()
** Purpose: Title���������(Option����)��Ⱦ
** Comment: Option������Ⱦģʽ(��Ϸѡ��)
** Para: float fDeltaTime			//��ǰ��Ϸ����֡ʱ���
*/
void SceneOption_Render(float fDeltaTime)
{
	g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//����Alphaͨ�����
	g_pD3D9Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//Alpha�������Ϊͨ������
	g_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//Alpha���Ĭ��Դ�ں�����
	g_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//Alpha���Ĭ��Ŀ���ں�����

	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//Alpha�������ʽ
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);//Alpha���������Դ:��������ɫ//D3DTA_DIFFUSE//D3DTA_TEXTURE
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);//Alpha���������Դ:������ɫ
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);//Color�������ʽ
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//Color���ѡ��������ɫ

	//���Ʊ���
	g_pD3D9Device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex));//���ö�����������Դ
	g_pD3D9Device->SetFVF(D3DFVF_VERTEX);//���ö���ṹ��
	g_pD3D9Device->SetIndices(g_pIndexBuffer);//����������������Դ
	g_pD3D9Device->SetTexture(0, g_pTextureTitle);//���ñ�������(����)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);//����ͼ��

	//��������
	g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//Color�������TFACTOR��ɫ��ɫ
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);//Color�������ʽ
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);//Color���ѡ��������ɫ
	g_pD3D9Device->SetTexture(0, g_pTextureLogo);//���ñ�������(LOGO��־)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 4, 0, 2);//����ͼ��
	g_pD3D9Device->SetTexture(0, g_pTextureVersion);//���ñ�������(�汾��Ver1.00a)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8, 0, 4, 0, 2);//����ͼ��

	if(g_bKeySelectTitleChange == false)
	{
		//������Ϸѡ��
		g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 0.65f, 0.65f, 1.0f));
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		g_pD3D9Device->SetTexture(0, g_pTextureTitleText);
		g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 12, 0, 20, 0, 10);
		if(g_bKeySelectFlashState == false)
		{
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		}
		else
		{
			g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		}
		g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 32, 0, 4, 0, 2);
	}
	else
	{
		//������Ϸѡ��
		g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 0.65f, 0.65f, 1.0f));
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		g_pD3D9Device->SetTexture(0, g_pTextureTitleText);
		g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 12, 0, 32, 0, 16);
		if(g_bKeySelectFlashState == false)
		{
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		}
		else
		{
			g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		}
		g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 44, 0, 4, 0, 2);
	}

	//���ƾ��鶯��
	g_nTitleFlagNow = g_pAnimation->RenderTitle(fDeltaTime);//"�|����둵�"Sprite����

	//�ر�Alpha���
	g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//�ر�Alpha���
}