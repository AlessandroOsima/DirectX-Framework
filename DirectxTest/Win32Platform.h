#pragma once

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

//DirectX related headers
#include <d3d11.h>
#include <dxgi.h>
#include <string>
//#include <d3dx11.h>
//#include <d3dx10.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dx11.lib")
//#pragma comment (lib, "d3dx10.lib")

struct WindowData
{
	int x;
	int y;
	int width;
	int height;
	std::string title;
	bool fullscreen;
};

class Win32Platform
{
public:
	Win32Platform();

	void Init(const WindowData & windowData);
	int RunLoop();
	void QuitApp() { endApp = true; }

	~Win32Platform();

private:
	void InitWindow(const WindowData & windowData);
	void InitRenderer(const WindowData & windowData);
	void DeInitRenderer();
	void Render();
	bool endApp;
	HWND hWnd;

	//DirectX vars
	IDXGISwapChain *swapchain;            
	ID3D11Device *dev;                     
	ID3D11DeviceContext *devcon; 
	ID3D11RenderTargetView * backbuffer;
};
