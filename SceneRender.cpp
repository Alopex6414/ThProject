#include"Common.h"
#include "Include.h"
#include "SceneRender.h"

/*
** Function:	 SceneSig_Init()
** Purpose: Sig等待界面资源初始化
*/
static void SceneSig_Init()
{
	g_pSubSig = new Subtitle(0.0f, 0.0f, 640.0f, 480.0f, 0.0f);//Sig背景页面初始化
	g_pSubLoad = new Subtitle(415.0f, 375.0f, 128.0f, 32.0f, 0.0f, 0.0f, false);//"少女祈祷中..."初始化
	g_pSubLoadEn = new Subtitle(462.0f, 392.0f, 128.0f, 32.0f, 20.0f, 0.0f, true);//"Loading..."初始化
}

/*
** Function:	 SceneSig_Update(float fTime)
** Purpose: Sig等待界面设置
** Comment: 上海爱丽丝幻乐团/"少女祈祷中..."
** Para: float fTime						//当前游戏总时间
** Para: float fDeltaTime			//当前游戏相邻帧时间差
*/
void SceneSig_Update(float fTime, float fDeltaTime)
{
	if(fTime < 0.1f)//0.0f~0.1f(游戏开始时间0.0s~0.1s时保持黑色屏幕,Alpha通道值为0.0f)
	{
		g_pSubSig->SetAlpha(0.0f);//(0.0s~0.1s Sig透明度Alpha为0.0f)
	}
	else if(fTime < 0.5f)//0.1f~0.5f(游戏开始时间0.1s~0.5s时Sig背景渐变,Alpha通道值增加)
	{
		g_pSubSig->AlphaIncrease(1.0f, 0.05f);//(0.1s~0.5s Sig透明度Alpha增速0.05f,持续20帧,时间0.33s)
		g_pSubLoad->AlphaIncrease(1.0f, 0.1f);//(0.1s~0.5s "少女祈祷中..."透明度Alpha增速0.1f,持续10帧,时间0.167s)
	}
	else//(游戏开始时间0.5s~5.0s时,背景Alpha通道值为1.0f)
	{
		g_pSubSig->SetAlpha(1.0f);//(0.5s~5.0s Sig透明度Alpha为1.0f)
		g_pSubLoad->AlphaCircle(0.5f, 1.0f, 0.02f);//"少女祈祷中..."透明度Alpha增速0.02f,变化范围0.5f->1.0f,持续25帧,交替变化
		g_pSubLoadEn->AlphaCircle(0.5f, 1.0f, 0.025f);//"loading..."透明度Alpha减速0.025f,变化范围1.0f->0.5f,持续20帧,交替变化
		g_pSubLoadEn->MoveXDecrease(0.0f, 1.0f);//"loading..."偏移位置,初始X偏移20,偏移X坐标递减("loading..."向左移动动作"<-")
		g_pSnow->Update(fDeltaTime);//Snow雪花粒子系统...
	}

	#pragma region 设置顶点缓存
	//设置顶点缓存数据
	Vertex*pVertices = NULL;
	g_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	//Sig背景纹理顶点数组
	pVertices[0] = Vertex(0.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubSig->GetAlpha()), 0.0f, 0.0f);
	pVertices[1] = Vertex(640.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubSig->GetAlpha()), (640.0f / 1024.0f), 0.0f);
	pVertices[2] = Vertex(640.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubSig->GetAlpha()), (640.0f / 1024.0f), (480.0f / 512.0f));
	pVertices[3] = Vertex(0.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubSig->GetAlpha()), 0.0f, (480.0f / 512.0f));

	//"少女祈祷中..."纹理顶点数组
	pVertices[4] = Vertex(415.0f, 375.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoad->GetAlpha()), 0.0f, 0.0f);
	pVertices[5] = Vertex(543.0f, 375.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoad->GetAlpha()), 1.0f, 0.0f);
	pVertices[6] = Vertex(543.0f, 407.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoad->GetAlpha()), 1.0f, 0.25f);
	pVertices[7] = Vertex(415.0f, 407.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoad->GetAlpha()), 0.0f, 0.25f);

	//"loading..."纹理顶点数组
	pVertices[8] = Vertex((462.0f + g_pSubLoadEn->GetMoveX()), 392.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoadEn->GetAlpha()), 0.0f, 0.25f);
	pVertices[9] = Vertex((590.0f + g_pSubLoadEn->GetMoveX()), 392.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoadEn->GetAlpha()), 1.0f, 0.25f);
	pVertices[10] = Vertex((590.0f + g_pSubLoadEn->GetMoveX()), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoadEn->GetAlpha()), 1.0f, 0.5f);
	pVertices[11] = Vertex((462.0f + g_pSubLoadEn->GetMoveX()), 424.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubLoadEn->GetAlpha()), 0.0f, 0.5f);

	g_pVertexBuffer->Unlock();
	#pragma endregion

	#pragma region 设置索引缓存
	//设置索引缓存数据
	WORD*pIndices = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);

	//Sig背景纹理索引数组
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	//"少女祈祷中..."纹理索引数组
	pIndices[6] = 4;
	pIndices[7] = 5;
	pIndices[8] = 6;

	pIndices[9] = 4;
	pIndices[10] = 6;
	pIndices[11] = 7;

	//"loading..."纹理索引数组
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
** Purpose: Sig等待界面渲染
** Para: float fTime						//当前游戏总时间
*/
void SceneSig_Render(float fTime)
{
	g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//开启Alpha通道混合
	g_pD3D9Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//Alpha混合设置为通道互补
	g_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//Alpha混合默认源融合因子
	g_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//Alpha混合默认目标融合因子

	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//Alpha混合纹理方式
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);//Alpha混合纹理来源:漫反射颜色//D3DTA_DIFFUSE//D3DTA_TEXTURE
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);//Alpha混合纹理来源:纹理颜色

	g_pD3D9Device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex));//设置顶点数组数据源
	g_pD3D9Device->SetFVF(D3DFVF_VERTEX);//设置顶点结构体
	g_pD3D9Device->SetIndices(g_pIndexBuffer);//设置索引数组数据源
	g_pD3D9Device->SetTexture(0, g_pTexture);//设置表面纹理(标题)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);//绘制图形
	if(fTime >= 0.5f) g_pSnow->Render();//0.5s后开始雪花粒子渲染
	g_pD3D9Device->SetTexture(0, g_pTextureLoading);//设置表面纹理(等待)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 8, 0, 4);//绘制图形
	g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//关闭Alpha通道混合
}

/*
** Function:	 SceneSig_Release()
** Purpose: Sig等待界面资源释放
*/
void SceneSig_Release()
{
	//释放Sig界面对象
	SAFE_DELETE(g_pSubSig);//释放Sig
	SAFE_DELETE(g_pSubLoad);//释放"少女祈祷中..."
	SAFE_DELETE(g_pSubLoadEn);//释放"Loading..."
}

/*
** Function:	 SceneTitle_Init()
** Purpose: Title主界面资源初始化
*/
static void SceneTitle_Init()
{
	g_pSubTitle = new Subtitle(0.0f, 0.0f, 640.0f, 480.0f, 0.0f);//Title背景页面初始化(主标题界面)
	g_pSubVersion = new Subtitle(304.0f, 431.0f, 64.0f, 16.0f, 0.0f);//Version版本号初始化(Ver1.00a)
	g_pSubGameOptionSelect = new Subtitle(540.0f, 256.0f, 128.0f, 32.0f, 0.0f, 0.0f, true);//GameOptionSelect游戏选中项(当前光标所在游戏选项)
	g_pSubArrayGameOptions = new Subtitle[8]//GameOptions游戏选项(主界面游戏选项)
	{
		Subtitle(540.0f, 256.0f, 128.0f, 32.0f, 540.0f, 0.0f, true),//GameStart选项
		Subtitle(556.0f, 280.0f, 128.0f, 32.0f, 556.0f, 0.0f, true),//ExtraStart选项
		Subtitle(572.0f, 304.0f, 128.0f, 32.0f, 572.0f, 0.0f, true),//PracticeStart选项
		Subtitle(588.0f, 328.0f, 128.0f, 32.0f, 588.0f, 0.0f, true),//Replay选项
		Subtitle(604.0f, 352.0f, 128.0f, 32.0f, 604.0f, 0.0f, true),//PlayerData选项
		Subtitle(620.0f, 376.0f, 128.0f, 32.0f, 620.0f, 0.0f, true),//MusicRoom选项
		Subtitle(636.0f, 400.0f, 128.0f, 32.0f, 636.0f, 0.0f, true),//Option选项
		Subtitle(652.0f, 424.0f, 128.0f, 32.0f, 652.0f, 0.0f, true),//Quit选项
	};
}

/*
** Function:	 SceneTitle_Update()
** Purpose: Title主标题界面设置
** Comment: 主界面渲染模式
** Para: HWND hWnd					//窗口句柄
** Para: float fDeltaTime			//当前游戏相邻帧时间差
*/
void SceneTitle_Update(HWND hWnd, float fDeltaTime)
{
	static bool bKeySelect = false;		//Key按键按下
	static bool bKeySelectX = false;		//'X'按键按下
	static bool bKeySelectZ = false;		//'Z'按键按下
	static float fKeyPressTime = 0.0f;	//Key按键按下时间计数

	g_pDirectSound->PlayMusic();//播放标题BGM音乐

	if(g_nTitleFlagNow < 5)//标题动画("東方地灵殿")
	{
		g_pSubTitle->SetAlpha(0.0f);//Title标题背景Alpha透明度0.0f(0%)
		g_pSubVersion->SetAlpha(0.0f);//Version版本号Alpha透明度0.0f(0%)
		g_pSubGameOptionSelect->SetAlpha(0.5f);//游戏选中项Alpha透明度0.5f(50%)(不可见)
	}
	else if(g_nTitleFlagNow == 5)//标题背景纹理加载
	{
		//标题背景版本号加载
		g_pSubTitle->AlphaIncrease(1.0f, 0.02f);//Title标题透明度Alpha增速0.02f,持续50帧,0.83s
		if(g_pSubTitle->GetAlpha() >= 0.8f)
		{
			g_pSubVersion->AlphaIncrease(1.0f, 0.1f);//"V1.00a"透明度Alpha增速0.1f,持续10帧
		}

		//游戏选项菜单加载(位置)
		for(int i = 0; i < 8; i++)
		{
			g_pSubArrayGameOptions[i].MoveXDecrease(476.0f, 8.0f);//游戏选项菜单向左8.0f速度移动
		}

		//游戏选项菜单加载(透明度)
		for(int i = 0; i < 8; i++)
		{
			if(g_pSubArrayGameOptions[i].GetAlpha() == 0.0f)//数组中有Alpha值为0.0f
			{
				g_pSubArrayGameOptions[i].AddAlpha(0.1f);//Alpha值为0.0f之前都增加0.1f
				break;
			}
			else
			{
				g_pSubArrayGameOptions[i].AlphaIncrease(1.0f, 0.1f);//Alpha值全部增加0.1f
			}
		}

		//游戏当前选中项Alpha值循环变化
		g_pSubGameOptionSelect->AlphaCircle(0.5f, 1.0f, 0.025f);//GameOptionSelect游戏选中项Alpha透明度变化
	}
	else if(g_nTitleFlagNow == 6)//主标题界面加载完成(DirectInput设备交互响应)(键盘KeyBoard)
	{
		g_pDirectInput->GetDeviceState();//获取按键状态(KeyBoard)
		if(bKeySelect == false)//KeyBoard按键未按下
		{
			#pragma region KeyPress检测方向键"↑"状态
			if(g_pDirectInput->IsKeyDown(DIK_UP))//检测方向键"↑"状态
			{
				bKeySelect = true;//KeyBoard按键已按下
				g_nKeyNumber--;
				if(g_nKeyNumber < 0)
				{
					g_nKeyNumber = 7;
				}
				g_pSubArrayGameOptions[g_nKeyNumber].ResetMoveX();
				g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_select00.wav");//DirectSound加载播放音效se_select00.wav
			}
			#pragma endregion

			#pragma region KeyPress检测方向键"↓"状态
			if(g_pDirectInput->IsKeyDown(DIK_DOWN))//检测方向键"↓"状态
			{
				bKeySelect = true;//KeyBoard按键已按下
				g_nKeyNumber++;
				if(g_nKeyNumber > 7)
				{
					g_nKeyNumber = 0;
				}
				g_pSubArrayGameOptions[g_nKeyNumber].ResetMoveX();
				g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_select00.wav");//DirectSound加载播放音效se_select00.wav
			}
			#pragma endregion

			#pragma region KeyPress检测"Z"键状态
			if(g_pDirectInput->IsKeyDown(DIK_Z))//检测"Z"键状态
			{
				if(bKeySelectZ == false)//'Z'键未按下
				{
					bKeySelect = true;//KeyBoard按键已按下
					g_pSubArrayGameOptions[g_nKeyNumber].ResetMoveX();
					g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_ok00.wav");//DirectSound加载播放音效se_ok00.wav
				}
				bKeySelectZ = true;//'Z'键已按下
			}
			else
			{
				bKeySelectZ = false;//'Z'键未按下
			}
			#pragma endregion

			#pragma region KeyPress检测"X"键/"Esc"键状态
			if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//检测"X"键/"Esc"键状态
			{
				if(bKeySelectX == false)
				{
					bKeySelect = true;//KeyBoard按键已按下
					if(g_nKeyNumber != 7)
					{
						g_nKeyNumber = 7;
						g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载播放音效se_cancel00.wav
					}
					else
					{
						g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载播放音效se_cancel00.wav
						Sleep(500);//线程Sleep500ms
						DestroyWindow(hWnd);//销毁窗口
					}
					g_pSubArrayGameOptions[g_nKeyNumber].ResetMoveX();
				}
				bKeySelectX = true;//'X'键已按下
			}
			else
			{
				bKeySelectX = false;//'X'键未按下
			}
			#pragma endregion

			//游戏当前选中项Alpha值循环变化
			g_pSubGameOptionSelect->AlphaCircle(0.5f, 1.0f, 0.025f);//GameOptionSelect游戏选中项Alpha透明度变化
		}
		else//KeyBoard按键已按下
		{
			#pragma region 非'Z'键按下处理
			if(bKeySelectZ == false)//KeyPress按键按下非'Z'键
			{
				fKeyPressTime += fDeltaTime;//计算叠加时间间隔
				if(fKeyPressTime > 0.12f && g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXStatus() == true)
				{
					fKeyPressTime = 0.0f;//计算时间清零
					g_pSubArrayGameOptions[g_nKeyNumber].SetMoveXStatus(false);//MoveX移动标志清空
					bKeySelect = false;//KeyBoard按键未按下
				}

				g_pSubArrayGameOptions[g_nKeyNumber].MoveXPendulum();//游戏选中项单摆运动一次
			}
			#pragma endregion

			#pragma region 'Z'键按下处理
			else//KeyPress按键按下'Z'键
			{
				g_pSubArrayGameOptions[g_nKeyNumber].MoveXSelect();//游戏选中项闪烁
				if(g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXFlashCount() >= 9)//9帧
				{
					//游戏选项菜单移动淡出
					for(int i = 0; i < 8; i++)
					{
						if(i != g_nKeyNumber)//未选中的游戏选项菜单移动
						{
							g_pSubArrayGameOptions[i].MoveXIncrease(640.0f, 4.0f);//未选中游戏选项菜单向右4.0f速度移动
							g_pSubArrayGameOptions[i].AlphaDecrease(0.0f, 0.05f);//未选中游戏选项菜单Alpha通道值以0.05f递减
						}
						else//已选中的游戏选项菜单移动
						{
							if(g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXFlashCount() >= 13)//13帧
							{
								g_pSubArrayGameOptions[i].MoveXIncrease(640.0f, 8.0f);//已选中游戏选项菜单向右8.0f速度移动
								g_pSubArrayGameOptions[i].AlphaDecrease(0.0f, 0.075f);//已选中游戏选项菜单Alpha通道值以0.075f递减
							}
						}
					}

					//游戏选项菜单禁止渲染
					if(g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXFlashCount() >= 13)//13帧
					{
						g_pSubArrayGameOptions[g_nKeyNumber].SetFlashState(false);//禁止渲染(闪烁)
					}

					//游戏选项菜单消除
					if(g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXFlashCount() >= 20)//20帧
					{
						fKeyPressTime = 0.0f;//计算时间清零
						g_pSubArrayGameOptions[g_nKeyNumber].ResetSelectFlash();//清除选中标志
						bKeySelect = false;//KeyBoard按键未按下

						g_nKeySelectState = 1;//转到副标题界面

						switch(g_nKeyNumber)
						{
						case 0://GameStart(Key按键被按下)
							break;
						case 1://ExtraStart(Key按键被按下)
							break;
						case 2://PracticeStart(Key按键被按下)
							break;
						case 3://Replay(Key按键被按下)
							break;
						case 4://PlayerData(Key按键被按下)
							break;
						case 5://MusicRoom(Key按键被按下)
							break;
						case 6://Option(Key按键被按下)
							//g_nKeyNumber_Option = 0;//子菜单选项
							g_bKeySelectTitleChange = false;
							for(int i = 0; i < 5; i++)//初始化选项设置菜单(恢复)
							{
								g_pSubArrayOptionsConfig[i].SetMoveX(540.0f + i * 16.0f);//选项设置字符恢复初始状态
								g_pSubArrayOptionsConfig[i].SetAlpha(0.0f);//选项设置Alpha透明度变化
							}
							break;
						case 7://Quit(ExitGame退出游戏)
							Sleep(500);//线程Sleep500ms
							DestroyWindow(hWnd);//销毁窗口
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

	#pragma region 设置顶点缓存
	//设置顶点缓存数据
	Vertex*pVertices = NULL;
	g_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	//Title00背景纹理顶点数组
	pVertices[0] = Vertex(0.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, 0.0f);
	pVertices[1] = Vertex(640.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), 0.0f);
	pVertices[2] = Vertex(640.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), (480.0f / 512.0f));
	pVertices[3] = Vertex(0.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, (480.0f / 512.0f));

	//"Copyright"纹理顶点数组
	pVertices[4] = Vertex(22.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), 0.0f);
	pVertices[5] = Vertex(227.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), 0.0f);
	pVertices[6] = Vertex(227.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), (16.0f / 256.0f));
	pVertices[7] = Vertex(22.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), (16.0f / 256.0f));

	//"Ver1.00a"纹理顶点数组
	pVertices[8] = Vertex(304.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 0.0f);
	pVertices[9] = Vertex(368.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 0.0f);
	pVertices[10] = Vertex(368.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 1.0f);
	pVertices[11] = Vertex(304.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 1.0f);

	//游戏选项菜单渲染
	for(int i = 0; i < 8; i++)
	{
		pVertices[12 + i * 4] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (128.0f / 512.0f), (i * 32.0f / 512.0f));
		pVertices[12 + i * 4 + 1] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + 128 + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (256.0f / 512.0f), (i * 32.0f / 512.0f));
		pVertices[12 + i * 4 + 2] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + 128 + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (256.0f / 512.0f), ((i + 1) * 32.0f / 512.0f));
		pVertices[12 + i * 4 + 3] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (128.0f / 512.0f), ((i + 1) * 32.0f / 512.0f));
	}

	//游戏选项选中项渲染
	if(bKeySelectZ == false)//'Z'键未按下
	{
		pVertices[44] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
		pVertices[45] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
		pVertices[46] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
		pVertices[47] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
	}
	else//'Z'键已按下
	{
		pVertices[44] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
		pVertices[45] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
		pVertices[46] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
		pVertices[47] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
	}

	g_pVertexBuffer->Unlock();
	#pragma endregion

	#pragma region 设置索引缓存
	//设置索引缓存数据
	WORD*pIndices = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);

	//Title00背景纹理索引数组
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	//"Copyright"纹理索引数组
	pIndices[6] = 4;
	pIndices[7] = 5;
	pIndices[8] = 6;

	pIndices[9] = 4;
	pIndices[10] = 6;
	pIndices[11] = 7;

	//"Ver1.00a"纹理索引数组
	pIndices[12] = 8;
	pIndices[13] = 9;
	pIndices[14] = 10;

	pIndices[15] = 8;
	pIndices[16] = 10;
	pIndices[17] = 11;

	//"GameStart"纹理索引数组
	pIndices[18] = 12;
	pIndices[19] = 13;
	pIndices[20] = 14;

	pIndices[21] = 12;
	pIndices[22] = 14;
	pIndices[23] = 15;

	//"ExtraStart"纹理索引数组
	pIndices[24] = 16;
	pIndices[25] = 17;
	pIndices[26] = 18;

	pIndices[27] = 16;
	pIndices[28] = 18;
	pIndices[29] = 19;

	//"PracticeStart"纹理索引数组
	pIndices[30] = 20;
	pIndices[31] = 21;
	pIndices[32] = 22;

	pIndices[33] = 20;
	pIndices[34] = 22;
	pIndices[35] = 23;

	//"Replay"纹理索引数组
	pIndices[36] = 24;
	pIndices[37] = 25;
	pIndices[38] = 26;

	pIndices[39] = 24;
	pIndices[40] = 26;
	pIndices[41] = 27;

	//"PlayerData"纹理索引数组
	pIndices[42] = 28;
	pIndices[43] = 29;
	pIndices[44] = 30;

	pIndices[45] = 28;
	pIndices[46] = 30;
	pIndices[47] = 31;

	//"MusicRoom"纹理索引数组
	pIndices[48] = 32;
	pIndices[49] = 33;
	pIndices[50] = 34;

	pIndices[51] = 32;
	pIndices[52] = 34;
	pIndices[53] = 35;

	//"Option"纹理索引数组
	pIndices[54] = 36;
	pIndices[55] = 37;
	pIndices[56] = 38;

	pIndices[57] = 36;
	pIndices[58] = 38;
	pIndices[59] = 39;

	//"Quit"纹理索引数组
	pIndices[60] = 40;
	pIndices[61] = 41;
	pIndices[62] = 42;

	pIndices[63] = 40;
	pIndices[64] = 42;
	pIndices[65] = 43;

	//游戏选项选中纹理索引数组
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
** Purpose: Title主标题界面渲染
** Comment: 主界面渲染模式
** Para: float fDeltaTime			//当前游戏相邻帧时间差
*/
void SceneTitle_Render(float fDeltaTime)
{
	g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//开启Alpha通道混合
	g_pD3D9Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//Alpha混合设置为通道互补
	g_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//Alpha混合默认源融合因子
	g_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//Alpha混合默认目标融合因子

	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//Alpha混合纹理方式
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);//Alpha混合纹理来源:漫反射颜色//D3DTA_DIFFUSE//D3DTA_TEXTURE
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);//Alpha混合纹理来源:纹理颜色
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);//Color混合纹理方式
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//Color混合选择纹理颜色

	//绘制标题
	g_pD3D9Device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex));//设置顶点数组数据源
	g_pD3D9Device->SetFVF(D3DFVF_VERTEX);//设置顶点结构体
	g_pD3D9Device->SetIndices(g_pIndexBuffer);//设置索引数组数据源
	g_pD3D9Device->SetTexture(0, g_pTextureTitle);//设置表面纹理(标题)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);//绘制图形

	//绘制文字
	g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//Color混合设置TFACTOR颜色白色
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);//Color混合纹理方式
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);//Color混合选择纹理颜色
	g_pD3D9Device->SetTexture(0, g_pTextureLogo);//设置表面纹理(LOGO标志)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 4, 0, 2);//绘制图形
	g_pD3D9Device->SetTexture(0, g_pTextureVersion);//设置表面纹理(版本号Ver1.00a)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8, 0, 4, 0, 2);//绘制图形

	//绘制游戏选项
	g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 0.65f, 0.65f, 1.0f));//Color混合设置TFACTOR颜色浅粉红色
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//Color混合纹理方式(相乘)
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//Color混合纹理
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);//Color混合TFACTOR颜色
	g_pD3D9Device->SetTexture(0, g_pTextureTitleText);//设置表面纹理(游戏菜单选项)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 12, 0, 32, 0, 16);//绘制图形
	if(g_bKeySelectFlashState == false)//游戏选中项渲染模式
	{
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);//Color混合纹理方式
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//Color混合纹理
	}
	else
	{
		g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));//Color混合设置TFACTOR颜色淡灰色
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//Color混合纹理(相乘)
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//Color混合纹理
		g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);//Color混合TFACTOR颜色
	}
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 44, 0, 4, 0, 2);//绘制图形

	//绘制精灵动画
	g_nTitleFlagNow = g_pAnimation->RenderTitle(fDeltaTime);//"東方地霊殿"Sprite动画
	
	//关闭Alpha混合
	g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//关闭Alpha混合
}

/*
** Function:	 SceneTitle_Release()
** Purpose: Title主界面资源释放
*/
void SceneTitle_Release()
{
	SAFE_DELETE(g_pSubTitle);//释放Title背景指针
	SAFE_DELETE(g_pSubVersion);//释放Version版本号指针
	SAFE_DELETE(g_pSubGameOptionSelect);//释放GameOptionSelect游戏选中项
	SAFE_DELETE_ARRAY(g_pSubArrayGameOptions);//释放GameOptions游戏选项
}

/*
** Function:	 SceneOption_Init()
** Purpose: Title副标题界面(Option界面)资源初始化
*/
static void SceneOption_Init()
{
	g_pSubTitle = new Subtitle(0.0f, 0.0f, 640.0f, 480.0f, 0.0f);//Title背景页面初始化(主标题界面)
	g_pSubVersion = new Subtitle(304.0f, 431.0f, 64.0f, 16.0f, 0.0f);//Version版本号初始化(Ver1.00a)
	g_pSubOptionConfigSelect = new Subtitle(540.0f, 304.0f, 128.0f, 32.0f, 540.0f, 0.5f, true);//OptionConfig选中项初始化
	g_pSubArrayOptionsConfig = new Subtitle[5]//OptionsConfig选项设置初始化(Option界面)
	{
		Subtitle(540.0f, 304.0f, 128.0f, 32.0f, 540.0f, 0.0f, true),//BGM Volume选项
		Subtitle(556.0f, 328.0f, 128.0f, 32.0f, 556.0f, 0.0f, true),//SE Volume选项
		Subtitle(572.0f, 352.0f, 128.0f, 32.0f, 572.0f, 0.0f, true),//Key Config选项
		Subtitle(588.0f, 376.0f, 128.0f, 32.0f, 588.0f, 0.0f, true),//Default选项
		Subtitle(604.0f, 400.0f, 128.0f, 32.0f, 604.0f, 0.0f, true),//Quit选项
	};
}

/*
** Function:	 SceneOption_Update()
** Purpose: Title副标题界面(Option界面)设置
** Comment: Option界面渲染模式(游戏选项)
** Para: float fDeltaTime			//当前游戏相邻帧时间差
*/
void SceneOption_Update(float fDeltaTime)
{
	static bool bKeySelect = false;		//Key按键按下
	static bool bKeySelectX = false;		//'X'按键按下
	static bool bKeySelectZ = false;		//'Z'按键按下
	static bool bKeyEnableFlag = false;	//Key按键使能标志(true:可以按键/false:禁止按键)
	static bool bKeyBackSpace = false;	//Key按键返回主标题界面标志
	static float fKeyPressTime = 0.0f;	//Key按键按下时间计数
	static int nKeySelectFlashTimesCount = 0;//Key选中动画计数

	//键盘KeyBoard处理(KeyBoard Process)
	#pragma region 键盘KeyBoard处理
	g_pDirectInput->GetDeviceState();//获取按键状态(KeyBoard)
	if(bKeySelect == false)//KeyBoard按键未按下
	{
		//键盘KeyBoard响应
		#pragma region 键盘KeyBoard使能检测
		bKeyEnableFlag = true;//键盘KeyBoard使能
		for(int i = 0; i < 5; i++)
		{
			if(g_pSubArrayOptionsConfig[i].GetMoveX() != 444.0f)//选项设置未加载完成
			{
				bKeyEnableFlag = false;//键盘KeyBoard禁止
				break;
			}
		}
		#pragma endregion

		//Option选项菜单动画
		#pragma region Option选项菜单动画
		//标题背景版本号加载
		g_pSubTitle->SetAlpha(1.0f);//Title标题背景Alpha透明度1.0f(100%)
		g_pSubVersion->SetAlpha(1.0f);//Version版本号Alpha透明度1.0f(100%)

		//选项设置菜单加载(位置)
		for(int i = 0; i < 5; i++)
		{
			g_pSubArrayOptionsConfig[i].MoveXDecrease(444.0f, 8.0f);//选项设置菜单向左8.0f速度移动
		}

		//选项设置菜单加载(透明度)
		for(int i = 0; i < 5; i++)
		{
			if(g_pSubArrayOptionsConfig[i].GetAlpha() == 0.0f)//数组中有Alpha值为0.0f
			{
				g_pSubArrayOptionsConfig[i].AddAlpha(0.1f);//Alpha值为0.0f之前都增加0.1f
				break;
			}
			else
			{
				g_pSubArrayOptionsConfig[i].AlphaIncrease(1.0f, 0.1f);//Alpha值全部增加0.1f
			}
		}

		//选项设置当前选中项Alpha值循环变化
		g_pSubOptionConfigSelect->AlphaCircle(0.5f, 1.0f, 0.025f);//OptionConfigSelect选项设置选中项Alpha透明度变化
		#pragma endregion

		if(bKeyEnableFlag == true)//键盘KeyBoard使能
		{
			#pragma region KeyPress检测方向键"↑"状态
			if(g_pDirectInput->IsKeyDown(DIK_UP))//检测方向键"↑"状态
			{
				bKeySelect = true;//KeyBoard按键已按下
				g_nKeyNumber_Option--;
				if(g_nKeyNumber_Option < 0)
				{
					g_nKeyNumber_Option = 4;
				}
				g_pSubArrayOptionsConfig[g_nKeyNumber_Option].ResetMoveX();
				g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_select00.wav");//DirectSound加载播放音效se_select00.wav
			}
			#pragma endregion

			#pragma region KeyPress检测方向键"↓"状态
			if(g_pDirectInput->IsKeyDown(DIK_DOWN))//检测方向键"↓"状态
			{
				bKeySelect = true;//KeyBoard按键已按下
				g_nKeyNumber_Option++;
				if(g_nKeyNumber_Option > 4)
				{
					g_nKeyNumber_Option = 0;
				}
				g_pSubArrayOptionsConfig[g_nKeyNumber_Option].ResetMoveX();
				g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_select00.wav");//DirectSound加载播放音效se_select00.wav
			}
			#pragma endregion

			#pragma region KeyPress检测"Z"键状态
			if(g_pDirectInput->IsKeyDown(DIK_Z))//检测"Z"键状态
			{
				if(bKeySelectZ == false)//'Z'键未按下
				{
					bKeySelect = true;//KeyBoard按键已按下
					if(g_nKeyNumber_Option == 4)//Quit
					{
						bKeyBackSpace = true;//退格标志置位
						g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载播放音效se_cancel00.wav
					}
					else
					{
						g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_ok00.wav");//DirectSound加载播放音效se_ok00.wav
					}
					g_pSubArrayOptionsConfig[g_nKeyNumber_Option].ResetMoveX();
				}
				bKeySelectZ = true;//'Z'键已按下
			}
			else
			{
				bKeySelectZ = false;//'Z'键未按下
			}
			#pragma endregion

			#pragma region KeyPress检测"X"键/"Esc"键状态
			if(g_pDirectInput->IsKeyDown(DIK_X) || g_pDirectInput->IsKeyDown(DIK_ESCAPE))//检测"X"键/"Esc"键状态
			{
				if(bKeySelectX == false)
				{
					bKeySelect = true;//KeyBoard按键已按下
					if(g_nKeyNumber_Option != 4)
					{
						g_nKeyNumber_Option = 4;
					}
					else
					{
						bKeyBackSpace = true;//退格标志置位
					}
					g_pSubArrayOptionsConfig[g_nKeyNumber_Option].ResetMoveX();
					g_pDirectSoundEffect->PlaySoundEffect(L"Resource\\sound\\se_cancel00.wav");//DirectSound加载播放音效se_cancel00.wav
				}
				bKeySelectX = true;//'X'键已按下
			}
			else
			{
				bKeySelectX = false;//'X'键未按下
			}
			#pragma endregion
		}
	}
	else//KeyBoard按键已按下
	{
		#pragma region 退格标志未置位
		if(bKeyBackSpace == false)//退格标志未置位
		{
			#pragma region 'Z'键未按下
			if(bKeySelect == false)//'Z'键未按下
			{
				fKeyPressTime += fDeltaTime;//计算叠加时间间隔
				if(fKeyPressTime > 0.12f && g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXStatus() == true)
				{
					fKeyPressTime = 0.0f;//计算时间清零
					g_pSubArrayOptionsConfig[g_nKeyNumber_Option].SetMoveXStatus(false);//MoveX移动标志清空
					bKeySelect = false;//KeyBoard按键未按下
				}

				g_pSubArrayOptionsConfig[g_nKeyNumber_Option].MoveXPendulum();//游戏选中项单摆运动一次
			}
			#pragma endregion

			#pragma region 'Z'键已按下
			else//'Z'键已按下
			{
				fKeyPressTime += fDeltaTime;//计算叠加时间间隔
				if(fKeyPressTime > 0.12f && g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXStatus() == true)
				{
					fKeyPressTime = 0.0f;//计算时间清零
					g_pSubArrayOptionsConfig[g_nKeyNumber_Option].SetMoveXStatus(false);//MoveX移动标志清空
					bKeySelect = false;//KeyBoard按键未按下
				}

				g_pSubArrayOptionsConfig[g_nKeyNumber_Option].MoveXPendulum();//游戏选中项单摆运动一次
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region 退格标志已置位
		else//退格标志已置位
		{
			if(g_bKeySelectTitleChange == false)
			{
				g_pSubArrayOptionsConfig[g_nKeyNumber_Option].MoveXSelect();//游戏选中项闪烁
				if(g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXFlashCount() >= 9)//9帧
				{
					//选项设置菜单移动淡出
					for(int i = 0; i < 5; i++)
					{
						if(i != g_nKeyNumber_Option)//未选中的游戏选项菜单移动
						{
							g_pSubArrayOptionsConfig[i].MoveXIncrease(640.0f, 4.0f);//未选中游戏选项菜单向右4.0f速度移动
							g_pSubArrayOptionsConfig[i].AlphaDecrease(0.0f, 0.05f);//未选中游戏选项菜单Alpha通道值以0.05f递减
						}
						else//已选中的游戏选项菜单移动
						{
							if(g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXFlashCount() >= 13)//13帧
							{
								g_pSubArrayOptionsConfig[i].MoveXIncrease(640.0f, 8.0f);//已选中游戏选项菜单向右8.0f速度移动
								g_pSubArrayOptionsConfig[i].AlphaDecrease(0.0f, 0.075f);//已选中游戏选项菜单Alpha通道值以0.075f递减
							}
						}
					}

					//选项设置菜单禁止渲染
					if(g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXFlashCount() >= 13)//13帧
					{
						g_pSubArrayOptionsConfig[g_nKeyNumber_Option].SetFlashState(false);//禁止渲染(闪烁)
					}

					//选项设置菜单消除
					if(g_pSubArrayOptionsConfig[g_nKeyNumber_Option].GetMoveXFlashCount() >= 20)//20帧
					{
						fKeyPressTime = 0.0f;//计算时间清零
						g_pSubArrayOptionsConfig[g_nKeyNumber_Option].ResetSelectFlash();//清除选中标志
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
				//游戏选项菜单加载(位置)
				for(int i = 0; i < 8; i++)
				{
					g_pSubArrayGameOptions[i].MoveXDecrease(476.0f, 8.0f);//选项设置菜单向左8.0f速度移动
				}

				//游戏选项菜单加载(透明度)
				for(int i = 0; i < 8; i++)
				{
					if(g_pSubArrayGameOptions[i].GetAlpha() == 0.0f)//数组中有Alpha值为0.0f
					{
						g_pSubArrayGameOptions[i].AddAlpha(0.0625f);//Alpha值为0.0f之前都增加0.1f
						break;
					}
					else
					{
						g_pSubArrayGameOptions[i].AlphaIncrease(1.0f, 0.0625f);//Alpha值全部增加0.1f
					}
				}

				nKeySelectFlashTimesCount++;
				if(nKeySelectFlashTimesCount >= 24)
				{
					nKeySelectFlashTimesCount = 0;
					fKeyPressTime = 0.0f;//计算时间清零
					bKeySelect = false;//KeyBoard按键未按下
					bKeyBackSpace = false;//Key按键返回主标题界面标志清空
					g_nKeySelectState = 0;//转到副标题界面
				}
			}
		}
		#pragma endregion
	}
	#pragma endregion

	#pragma region 设置顶点缓存
	//设置顶点缓存数据
	Vertex*pVertices = NULL;
	g_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	//Title00背景纹理顶点数组
	pVertices[0] = Vertex(0.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, 0.0f);
	pVertices[1] = Vertex(640.0f, 0.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), 0.0f);
	pVertices[2] = Vertex(640.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (640.0f / 1024.0f), (480.0f / 512.0f));
	pVertices[3] = Vertex(0.0f, 480.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), 0.0f, (480.0f / 512.0f));

	//"Copyright"纹理顶点数组
	pVertices[4] = Vertex(22.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), 0.0f);
	pVertices[5] = Vertex(227.0f, 460.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), 0.0f);
	pVertices[6] = Vertex(227.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (815.0f / 1024.0f), (16.0f / 256.0f));
	pVertices[7] = Vertex(22.0f, 476.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubTitle->GetAlpha()), (610.0f / 1024.0f), (16.0f / 256.0f));

	//"Ver1.00a"纹理顶点数组
	pVertices[8] = Vertex(304.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 0.0f);
	pVertices[9] = Vertex(368.0f, 431.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 0.0f);
	pVertices[10] = Vertex(368.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.5f, 1.0f);
	pVertices[11] = Vertex(304.0f, 447.0f, 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubVersion->GetAlpha()), 0.0f, 1.0f);

	if(g_bKeySelectTitleChange == false)
	{
		//选项设置菜单渲染
		#pragma region "BGM Volume"纹理顶点数组
		//"BGM Volume"纹理顶点数组
		pVertices[12] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[0].GetAlpha()), (128.0f / 512.0f), (256.0f / 512.0f));
		pVertices[13] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + 128 + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[0].GetAlpha()), (256.0f / 512.0f), (256.0f / 512.0f));
		pVertices[14] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + 128 + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[0].GetAlpha()), (256.0f / 512.0f), (288.0f / 512.0f));
		pVertices[15] = Vertex((g_pSubArrayOptionsConfig[0].GetMoveX() + g_pSubArrayOptionsConfig[0].GetMoveXDelta()), g_pSubArrayOptionsConfig[0].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[0].GetAlpha()), (128.0f / 512.0f), (288.0f / 512.0f));
		#pragma endregion

		#pragma region "SE Volume"纹理顶点数组
		//"SE Volume"纹理顶点数组
		pVertices[16] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[1].GetAlpha()), (128.0f / 512.0f), (288.0f / 512.0f));
		pVertices[17] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + 128 + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[1].GetAlpha()), (256.0f / 512.0f), (288.0f / 512.0f));
		pVertices[18] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + 128 + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[1].GetAlpha()), (256.0f / 512.0f), (320.0f / 512.0f));
		pVertices[19] = Vertex((g_pSubArrayOptionsConfig[1].GetMoveX() + g_pSubArrayOptionsConfig[1].GetMoveXDelta()), g_pSubArrayOptionsConfig[1].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[1].GetAlpha()), (128.0f / 512.0f), (320.0f / 512.0f));
		#pragma endregion

		#pragma region "Key Config"纹理顶点数组
		//"Key Config"纹理顶点数组
		pVertices[20] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[2].GetAlpha()), (128.0f / 512.0f), (352.0f / 512.0f));
		pVertices[21] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + 128 + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[2].GetAlpha()), (256.0f / 512.0f), (352.0f / 512.0f));
		pVertices[22] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + 128 + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[2].GetAlpha()), (256.0f / 512.0f), (384.0f / 512.0f));
		pVertices[23] = Vertex((g_pSubArrayOptionsConfig[2].GetMoveX() + g_pSubArrayOptionsConfig[2].GetMoveXDelta()), g_pSubArrayOptionsConfig[2].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[2].GetAlpha()), (128.0f / 512.0f), (384.0f / 512.0f));
		#pragma endregion

		#pragma region "Default"纹理顶点数组
		//"Default"纹理顶点数组
		pVertices[24] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[3].GetAlpha()), (128.0f / 512.0f), (320.0f / 512.0f));
		pVertices[25] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + 128 + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[3].GetAlpha()), (256.0f / 512.0f), (320.0f / 512.0f));
		pVertices[26] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + 128 + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[3].GetAlpha()), (256.0f / 512.0f), (352.0f / 512.0f));
		pVertices[27] = Vertex((g_pSubArrayOptionsConfig[3].GetMoveX() + g_pSubArrayOptionsConfig[3].GetMoveXDelta()), g_pSubArrayOptionsConfig[3].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[3].GetAlpha()), (128.0f / 512.0f), (352.0f / 512.0f));
		#pragma endregion

		#pragma region "Quit"纹理顶点数组
		//"Quit"纹理顶点数组
		pVertices[28] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[4].GetAlpha()), (128.0f / 512.0f), (224.0f / 512.0f));
		pVertices[29] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + 128 + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[4].GetAlpha()), (256.0f / 512.0f), (224.0f / 512.0f));
		pVertices[30] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + 128 + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[4].GetAlpha()), (256.0f / 512.0f), (256.0f / 512.0f));
		pVertices[31] = Vertex((g_pSubArrayOptionsConfig[4].GetMoveX() + g_pSubArrayOptionsConfig[4].GetMoveXDelta()), g_pSubArrayOptionsConfig[4].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayOptionsConfig[4].GetAlpha()), (128.0f / 512.0f), (256.0f / 512.0f));
		#pragma endregion

		#pragma region 选项设置选中项渲染
		//选项设置选中项渲染
		if(bKeyBackSpace == false)//'BackSpace'键未按下
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
		else//'BackSpace'键已按下
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
		//游戏选项菜单渲染
		for(int i = 0; i < 8; i++)
		{
			pVertices[12 + i * 4] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (128.0f / 512.0f), (i * 32.0f / 512.0f));
			pVertices[12 + i * 4 + 1] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + 128 + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (256.0f / 512.0f), (i * 32.0f / 512.0f));
			pVertices[12 + i * 4 + 2] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + 128 + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (256.0f / 512.0f), ((i + 1) * 32.0f / 512.0f));
			pVertices[12 + i * 4 + 3] = Vertex((g_pSubArrayGameOptions[i].GetMoveX() + g_pSubArrayGameOptions[i].GetMoveXDelta()), g_pSubArrayGameOptions[i].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[i].GetAlpha()), (128.0f / 512.0f), ((i + 1) * 32.0f / 512.0f));
		}

		//游戏选项选中项渲染
		if(bKeySelectZ == false)//'Z'键未按下
		{
			pVertices[44] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
			pVertices[45] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
			pVertices[46] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
			pVertices[47] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubGameOptionSelect->GetAlpha() * g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
		}
		else//'Z'键已按下
		{
			pVertices[44] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
			pVertices[45] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_TopRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), (g_nKeyNumber * 32.0f / 512.0f));
			pVertices[46] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + 128 + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomRight), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (128.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
			pVertices[47] = Vertex((g_pSubArrayGameOptions[g_nKeyNumber].GetMoveX() + g_pSubArrayGameOptions[g_nKeyNumber].GetMoveXDelta()), g_pSubArrayGameOptions[g_nKeyNumber].GetVertexY(Vertex_BottomLeft), 0.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, g_pSubArrayGameOptions[g_nKeyNumber].GetAlpha()), (0.0f / 512.0f), ((g_nKeyNumber + 1) * 32.0f / 512.0f));
		}
	}

	g_pVertexBuffer->Unlock();
	#pragma endregion

	#pragma region 设置索引缓存
	//设置索引缓存数据
	WORD*pIndices = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);

	//Title00背景纹理索引数组
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	//"Copyright"纹理索引数组
	pIndices[6] = 4;
	pIndices[7] = 5;
	pIndices[8] = 6;

	pIndices[9] = 4;
	pIndices[10] = 6;
	pIndices[11] = 7;

	//"Ver1.00a"纹理索引数组
	pIndices[12] = 8;
	pIndices[13] = 9;
	pIndices[14] = 10;

	pIndices[15] = 8;
	pIndices[16] = 10;
	pIndices[17] = 11;

	//"GameStart"纹理索引数组
	pIndices[18] = 12;
	pIndices[19] = 13;
	pIndices[20] = 14;

	pIndices[21] = 12;
	pIndices[22] = 14;
	pIndices[23] = 15;

	//"ExtraStart"纹理索引数组
	pIndices[24] = 16;
	pIndices[25] = 17;
	pIndices[26] = 18;

	pIndices[27] = 16;
	pIndices[28] = 18;
	pIndices[29] = 19;

	//"PracticeStart"纹理索引数组
	pIndices[30] = 20;
	pIndices[31] = 21;
	pIndices[32] = 22;

	pIndices[33] = 20;
	pIndices[34] = 22;
	pIndices[35] = 23;

	//"Replay"纹理索引数组
	pIndices[36] = 24;
	pIndices[37] = 25;
	pIndices[38] = 26;

	pIndices[39] = 24;
	pIndices[40] = 26;
	pIndices[41] = 27;

	//"PlayerData"纹理索引数组
	pIndices[42] = 28;
	pIndices[43] = 29;
	pIndices[44] = 30;

	pIndices[45] = 28;
	pIndices[46] = 30;
	pIndices[47] = 31;

	//"MusicRoom"纹理索引数组
	pIndices[48] = 32;
	pIndices[49] = 33;
	pIndices[50] = 34;

	pIndices[51] = 32;
	pIndices[52] = 34;
	pIndices[53] = 35;

	//"Option"纹理索引数组
	pIndices[54] = 36;
	pIndices[55] = 37;
	pIndices[56] = 38;

	pIndices[57] = 36;
	pIndices[58] = 38;
	pIndices[59] = 39;

	//"Quit"纹理索引数组
	pIndices[60] = 40;
	pIndices[61] = 41;
	pIndices[62] = 42;

	pIndices[63] = 40;
	pIndices[64] = 42;
	pIndices[65] = 43;

	//游戏选项选中纹理索引数组
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
** Purpose: Title副标题界面(Option界面)渲染
** Comment: Option界面渲染模式(游戏选项)
** Para: float fDeltaTime			//当前游戏相邻帧时间差
*/
void SceneOption_Render(float fDeltaTime)
{
	g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//开启Alpha通道混合
	g_pD3D9Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//Alpha混合设置为通道互补
	g_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//Alpha混合默认源融合因子
	g_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//Alpha混合默认目标融合因子

	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//Alpha混合纹理方式
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);//Alpha混合纹理来源:漫反射颜色//D3DTA_DIFFUSE//D3DTA_TEXTURE
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);//Alpha混合纹理来源:纹理颜色
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);//Color混合纹理方式
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//Color混合选择纹理颜色

	//绘制标题
	g_pD3D9Device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex));//设置顶点数组数据源
	g_pD3D9Device->SetFVF(D3DFVF_VERTEX);//设置顶点结构体
	g_pD3D9Device->SetIndices(g_pIndexBuffer);//设置索引数组数据源
	g_pD3D9Device->SetTexture(0, g_pTextureTitle);//设置表面纹理(标题)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);//绘制图形

	//绘制文字
	g_pD3D9Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//Color混合设置TFACTOR颜色白色
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);//Color混合纹理方式
	g_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);//Color混合选择纹理颜色
	g_pD3D9Device->SetTexture(0, g_pTextureLogo);//设置表面纹理(LOGO标志)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 4, 0, 2);//绘制图形
	g_pD3D9Device->SetTexture(0, g_pTextureVersion);//设置表面纹理(版本号Ver1.00a)
	g_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8, 0, 4, 0, 2);//绘制图形

	if(g_bKeySelectTitleChange == false)
	{
		//绘制游戏选项
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
		//绘制游戏选项
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

	//绘制精灵动画
	g_nTitleFlagNow = g_pAnimation->RenderTitle(fDeltaTime);//"東方地霊殿"Sprite动画

	//关闭Alpha混合
	g_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//关闭Alpha混合
}