#include "Win32Platform.h"
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <assert.h>

LRESULT CALLBACK WindowsMessages(
	_In_  HWND hwnd,
	_In_  UINT uMsg,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	);

//FIXME: use a better solution
//needed for WindowsMessage to signal app end 
bool runApp = true;

Win32Platform::Win32Platform() : endApp(false)
{
}

void Win32Platform::Init(const WindowData & windowData, std::unique_ptr<App::AppCallbacks> & app)
{

	assert(app);

	userApp = std::move(app);

	InitWindow(windowData);
	renderer.Init(windowData, hWnd);
	scene.Init(&renderer);

	userApp->OnInit(scene);
}

int Win32Platform::RunLoop()
{
	while (runApp && !endApp)
	{

		MSG message;

		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		userApp->OnPreRender(scene);
		scene.Render();
		userApp->OnPreRender(scene);
	}

	return 0;
}

void Win32Platform::InitWindow(const WindowData & windowData)
{

	WNDCLASSEX wnd;

	ZeroMemory(&wnd, sizeof(wnd));

	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.style = CS_VREDRAW | CS_HREDRAW;
	wnd.lpfnWndProc = WindowsMessages;
	wnd.hInstance = GetModuleHandle(NULL);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wnd.lpszClassName = L"WindowClassDirectX";

	RegisterClassEx(&wnd);

	RECT clientRect; //{ 0, 0, windowData.width, windowData.height };
    clientRect.bottom = windowData.height;
    clientRect.left = 0;
    clientRect.right = windowData.width;
    clientRect.top = 0;

	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

	std::wstring wstring = std::wstring(windowData.title.begin(), windowData.title.end());

	hWnd = CreateWindowEx(NULL, L"WindowClassDirectX", wstring.c_str(), WS_OVERLAPPEDWINDOW, windowData.x, windowData.y, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, NULL, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
}


Win32Platform::~Win32Platform()
{
	userApp->OnDeInit(scene);
	renderer.DeInit();
}

LRESULT CALLBACK WindowsMessages(
	_In_  HWND hwnd,
	_In_  UINT uMsg,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	)
{

	switch (uMsg)
	{

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		runApp = false;
		return 0;
	}


	default:
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}