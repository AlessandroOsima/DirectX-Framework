#pragma once

#include "WinUtils.h"
#include "..\Graph\DirectxRenderer.h"
#include "..\Common.h"

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <memory>

//DirectX related headers
#include "..\Graph\Directx.h"
#include <string>

#include "../App/App.h"
#include "../Graph/Scene.h"

class Win32Platform
{
public:
	Win32Platform();

	void Init(const WindowData & windowData, std::unique_ptr<App::AppCallbacks> & app);
	int RunLoop();
	void QuitApp() { endApp = true; }

	~Win32Platform();

private:
	void InitWindow(const WindowData & windowData);
	bool endApp;
	HWND hWnd;
	std::unique_ptr<App::AppCallbacks> userApp;
	Graph::Scene scene;

	Graph::DirectxRenderer renderer;

};
