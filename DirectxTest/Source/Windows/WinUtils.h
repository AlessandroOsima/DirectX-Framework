#pragma  once
#include <windows.h>
#include <string>

namespace WinUtils
{
	std::wstring GetCStringFromHR(HRESULT res);
	void PrintErrorMessage(HRESULT res, const std::string & addToErrorCaption, const std::string & title);
	void PrintErrorMessageString(std::string msg, const std::string & addToErrorCaption, const std::string & title);
}