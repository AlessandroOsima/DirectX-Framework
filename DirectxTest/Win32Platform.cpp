#include "Win32Platform.h"

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

void Win32Platform::Init(const WindowData & windowData)
{
	InitWindow(windowData);
	InitRenderer(windowData);
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

		Render();
	}

	return 0;
}

void Win32Platform::Render()
{
	float color[] = { 0.1f, 0.5f, 0.4f, 1.0f };
	devcon->ClearRenderTargetView(backbuffer, color);

	swapchain->Present(0, 0);
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

	RECT clientRect{ 0, 0, windowData.width, windowData.height };

	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

	std::wstring wstring = std::wstring(windowData.title.begin(), windowData.title.end());

	hWnd = CreateWindowEx(NULL, L"WindowClassDirectX", wstring.c_str(), WS_OVERLAPPEDWINDOW, windowData.x, windowData.y, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, NULL, NULL, GetModuleHandle(NULL), NULL);


	ShowWindow(hWnd, SW_SHOWDEFAULT);
	
}

void Win32Platform::InitRenderer(const WindowData & windowData)
{

	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = windowData.width;
	swapChainDesc.BufferDesc.Height = windowData.height;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = !windowData.fullscreen;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	D3D11CreateDeviceAndSwapChain(
		NULL, 
		D3D_DRIVER_TYPE_HARDWARE, 
		NULL, 
		NULL, 
		NULL, 
		0, 
		D3D11_SDK_VERSION,
		&swapChainDesc, 
		&swapchain, 
		&dev, 
		NULL, 
		&devcon);

	ID3D11Texture2D * pBackbuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackbuffer);
	
	dev->CreateRenderTargetView(pBackbuffer, NULL, &backbuffer);
	pBackbuffer->Release();

	devcon->OMSetRenderTargets(0, &backbuffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)windowData.width;
	viewport.Height = (float)windowData.height;

	devcon->RSSetViewports(1, &viewport);
}

void Win32Platform::DeInitRenderer()
{
	swapchain->SetFullscreenState(FALSE, NULL);

	backbuffer->Release();
	devcon->Release();
	dev->Release();
	swapchain->Release();
}

Win32Platform::~Win32Platform()
{
	DeInitRenderer();
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