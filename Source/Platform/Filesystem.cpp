#include "../Windows/WinUtils.h"
#include <tchar.h>

namespace Filesystem
{
	//windows only implementation, change when new platforms will be added 
	std::string GetCurrentWorkingDirectory()
	{
		DWORD currentDirectorySize = GetCurrentDirectory(0, nullptr);

		int currentDirStringLenght = currentDirectorySize;

		TCHAR * currentDirectoryBuffer = new TCHAR[currentDirStringLenght];

		GetCurrentDirectory(currentDirectorySize, currentDirectoryBuffer);

		std::string fullPath = std::string(currentDirectoryBuffer);

		delete[] currentDirectoryBuffer;

		return fullPath;
	}
}


