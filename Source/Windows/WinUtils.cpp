#include "WinUtils.h"
#include <comdef.h>



std::string WinUtils::GetCStringFromHR(HRESULT res)
{
	_com_error err(res);


	return std::string(err.ErrorMessage());
}

void WinUtils::PrintErrorMessage(HRESULT res, const std::string & addToErrorCaption, const std::string & title)
{
	std::string errorMessage = std::string(addToErrorCaption.begin(),addToErrorCaption.end());

	errorMessage += GetCStringFromHR(res);
	MessageBox(NULL, std::string(errorMessage.begin(), errorMessage.end()).c_str(), std::string(title.begin(), title.end()).c_str(), MB_OK);
	
	//delete [] errorString;
}

void WinUtils::PrintErrorMessageString(std::string msg, const std::string & addToErrorCaption, const std::string & title)
{
	std::string fullMessage = addToErrorCaption + msg;

	std::string errorMessage = std::string(fullMessage.begin(), fullMessage.end());

	MessageBox(NULL, errorMessage.c_str(), std::string(title.begin(), title.end()).c_str(), MB_OK);

}
