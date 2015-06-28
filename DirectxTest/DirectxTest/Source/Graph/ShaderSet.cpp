#include "ShaderSet.h"

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <cassert>
#include "..\Windows\WinUtils.h"

ShaderSet::ShaderSet()
{

}

bool ShaderSet::LoadFromFile(const std::wstring & vertextShaderPath, const std::string & vertexShaderMainFunction, const std::wstring & pixelShaderPath, const std::string & pixelShaderMainFunction, ID3D11Device * dev)
{
	assert(dev);

	ID3D10Blob  * Errors;

	HRESULT res = D3DCompileFromFile(vertextShaderPath.c_str(), 0, 0, vertexShaderMainFunction.c_str(), "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PREFER_FLOW_CONTROL, 0, &VSBlob, &Errors);
	if (res != S_OK || Errors != nullptr)
	{
		WinUtils::PrintErrorMessage(res, "Error loading VS from file : ","Error");
		
		if (Errors != nullptr)
		{
			char * compileError = static_cast<char*>(Errors->GetBufferPointer());
			WinUtils::PrintErrorMessageString(compileError, "Error loading VS from file : ", "Error");

			Errors->Release();
		}

		return false;
	}

	res = D3DCompileFromFile(pixelShaderPath.c_str(), 0, 0, pixelShaderMainFunction.c_str(), "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PREFER_FLOW_CONTROL, 0, &PSBlob, &Errors);
	if (res != S_OK || Errors != nullptr)
	{
		WinUtils::PrintErrorMessage(res, "Error loading PS from file : ", "Error");

		if (Errors != nullptr)
		{
			char * compileError = static_cast<char*>(Errors->GetBufferPointer());
			WinUtils::PrintErrorMessageString(compileError, "Error loading VS from file : ", "Error");

			Errors->Release();
		}

		return false;
	}

	res = dev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &vertexShader);
	if (res != S_OK)
	{
		WinUtils::PrintErrorMessage(res, "Error creating VS : ", "Error");
		return false;
	}

	res = dev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pixelShader);
	if (res != S_OK)
	{
		WinUtils::PrintErrorMessage(res, "Error creating PS : ", "Error");
		return false;
	}

	return true;
}

void ShaderSet::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC desc[], int numElements, ID3D11Device * dev)
{
	assert(dev);

	dev->CreateInputLayout(desc, numElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &inputLayout);
}

void ShaderSet::SetInDeviceContext(ID3D11DeviceContext * devcon)
{
	assert(devcon);

	devcon->VSSetShader(vertexShader, 0, 0);
	devcon->PSSetShader(pixelShader, 0, 0);
}

ShaderSet::~ShaderSet()
{

}
