#pragma once
#include <string>
#include "Directx.h"

class ShaderSet
{
public:
	ShaderSet();
	
	bool LoadFromFile(const std::string & vertextShaderPath, const std::string & vertexShaderMainFunction, const std::string & pixelShaderPath, const std::string & pixelShaderMainFunction, ID3D11Device * dev);
	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC desc[], int numElements, ID3D11Device * dev);
	void SetInDeviceContext(ID3D11DeviceContext * devcon);

	inline ID3D11InputLayout * GetInputLayout() { return inputLayout; };

	~ShaderSet();
private:
	ID3D10Blob * VSBlob;
	ID3D10Blob * PSBlob;

	ID3D11VertexShader * vertexShader;    // the vertex shader
	ID3D11PixelShader * pixelShader;     // the pixel shader
	ID3D11InputLayout * inputLayout;
};

