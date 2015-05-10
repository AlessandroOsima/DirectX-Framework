#include "WinUtils.h"
#include <comdef.h>


std::wstring WinUtils::GetCStringFromHR(HRESULT res)
{
	_com_error err(res);


	return std::wstring(err.ErrorMessage());
}

void WinUtils::PrintErrorMessage(HRESULT res, const std::string & addToErrorCaption, const std::string & title)
{
	std::wstring errorMessage = std::wstring(addToErrorCaption.begin(),addToErrorCaption.end());

	errorMessage += GetCStringFromHR(res);
	MessageBox(NULL, std::wstring(errorMessage.begin(), errorMessage.end()).c_str(), std::wstring(title.begin(), title.end()).c_str(), MB_OK);
	
	//delete [] errorString;
}

void WinUtils::PrintErrorMessageString(std::string msg, const std::string & addToErrorCaption, const std::string & title)
{
	std::string fullMessage = addToErrorCaption + msg;

	std::wstring errorMessage = std::wstring(fullMessage.begin(), fullMessage.end());

	MessageBox(NULL, errorMessage.c_str(), std::wstring(title.begin(), title.end()).c_str(), MB_OK);

}
