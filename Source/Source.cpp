#include "Windows\Win32Platform.h"
#include "App\Drawer\Drawer.h"
#include "Platform\Platform.h"

#include <memory>
namespace WinDataConfigs
{
	WindowData fullscreen1440x900 = { 0, 0, 1440, 900, "Directx App", true };
	WindowData windowed1440x900 = { 0, 0, 1440, 900, "Directx App", false };
	WindowData windowed800x600 = { 100, 100, 800, 600, "Directx App", false };
}

Win32Platform ptf;

Platform * GCurrentPlatform = &ptf;

int WINAPI  WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	ptf.Init(WinDataConfigs::windowed800x600, std::unique_ptr<App::AppCallbacks>(new App::Drawer));

	int returnStatus = ptf.RunLoop();

	return returnStatus;
}
