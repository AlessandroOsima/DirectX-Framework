#pragma once 

#include "../Graph/Lights/Light.h"

namespace Resources
{
	//Builtin shaders indicies and definitions. When it grows too much should be moved to another file. 
	namespace Shaders
	{

		//Used to know where the shader is located on the filesystem and what is its entry point
		//Could be loaded from a file
		struct ShaderTypeInfo
		{
			std::string fileName;
			std::string mainFunctionName;
			std::string profile;
			unsigned int compilationFlags;
		};

		struct ShadersInfo
		{
			std::unique_ptr<ShaderTypeInfo> vertexShaderInfo = nullptr;
			std::unique_ptr<ShaderTypeInfo> hullShaderInfo = nullptr;
			std::unique_ptr<ShaderTypeInfo> domainShaderInfo = nullptr;
			std::unique_ptr<ShaderTypeInfo> geometryShaderInfo = nullptr;
			std::unique_ptr<ShaderTypeInfo> pixelShaderInfo = nullptr;
			std::unique_ptr<ShaderTypeInfo> computeShaderInfo = nullptr;
		};

		//Builtin shaders for now initializing and loading more shaders at runtime is not needed. 
		enum class SHADERS_IDX : unsigned int
		{
			BLINN_PHONG_TEXTURE = 0,
			SIZE = 1,
		};

		enum SHADER_TYPES
		{
			VERTEX_SHADER = 0x1,
			HULL_SHADER = 0x2,
			DOMAIN_SHADER = 0x8,
			GEOMETRY_SHADER = 0xf,
			PIXEL_SHADER = 0x10,
			COMPUTE_SHADER = 0x20,
		};

		//Builtin shader info (constant buffers and more when needed). 
		namespace BLINN_PHONG_TEXTURE
		{
			struct vsConstantBuffer
			{
				Math::Matrix44 finalMatrix;
				Math::Matrix44 modelMatrix;
			};

			struct PerFramePSConstantBuffer
			{
				Math::Vector4f eye;
			};

			struct PerGeometryPSConstantBuffer
			{
				Math::Color ambientLight;
				Graph::DirectionalLightProperties  directionalLights[Graph::Constants::MAX_DIRECTIONAL_LIGHTS];
				Graph::PointLightProperties  pointLights[Graph::Constants::MAX_POINT_LIGHTS];
				Math::Vector4f activeLights;
			};
		}
	}
}

