#pragma once

#include "WinUtils.h"
#include "..\Graph\DirectxRenderer.h"
#include "..\Common.h"

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

//DirectX related headers
#include "..\Graph\Directx.h"
#include <string>

#include "../App/App.h"
#include "../Graph/Scene.h"

class Win32Platform
{
public:
	Win32Platform();

	void Init(const WindowData & windowData, App::AppCallbacks * userApp);
	int RunLoop();
	void QuitApp() { endApp = true; }

	~Win32Platform();

private:
	void InitWindow(const WindowData & windowData);
	bool endApp;
	HWND hWnd;
	App::AppCallbacks * userApp;
	Graph::Scene scene;

	DirectxRenderer renderer;

};
