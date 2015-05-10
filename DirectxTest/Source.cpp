#include "Win32Platform.h"

namespace WinDataConfigs
{
	WindowData fullscreen1440x900 = { 0, 0, 1440, 900, "Directx App", true };
	WindowData windowed800x600 = { 100, 100, 800, 600, "Directx App", false };
}

int WINAPI  WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	Win32Platform ptf {};

	ptf.Init(WinDataConfigs::windowed800x600);

	int returnStatus = ptf.RunLoop();

	return returnStatus;
}
