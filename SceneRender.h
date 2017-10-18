#pragma once

#ifndef __SCENERENDER_H_
#define __SCENERENDER_H_

#include "Common.h"

//Variable Declaration

//Function Declaration
extern void SceneSig_Update(float fTime, float fDeltaTime);
extern void SceneSig_Render(float fTime);
extern void SceneSig_Release();
extern void SceneTitle_Update(HWND hWnd, float fDeltaTime);
extern void SceneTitle_Render(float fDeltaTime);
extern void SceneTitle_Release();
extern void SceneOption_Update(float fDeltaTime);
extern void SceneOption_Render(float fDeltaTime);

#endif // !__SCENERENDER_H_

