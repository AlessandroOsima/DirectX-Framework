#pragma once
#include "../Graph/ShaderSet.h"
#include "Resources.h"
#include <memory.h>

namespace Resources
{
	/*
	Very simple shader set manager. For now all shaders are registered and loaded at Init and unloaded at DeInit. 
	
	To ease shaders definition a shader set is loaded from a .sif (shader information file) file.
	A .sif file contains a list of all the shaders to load for a shader set, its format is:

	stages //the beginning of a file

	[stagename]->  //Can be v,h,d,g,p,c for the vertex, hull, domain, geometry and compute shaders
	[filename] //The name of the file the shader is stored in
	[main function name] //The name of the main function of the shader
	[profile] //The profile the shader must be compiled with
	[flags] //The flags the shader must be compiled with

	Example :

	stages

	v->
	shaders.hlsl
	VShader
	vs_5_0
	3077

	p->
	shaders.hlsl
	PShader
	ps_5_0
	3077

	For the file to be loaded correctly at least the pixel and vertex shaders must be defined
	Note that all .sif and .hlsl files must be under the /Data/Shaders directory to be loaded
	*/

	class ShaderSetManager
	{
	public:
		ShaderSetManager();

		//Register all the builtin shaders
		void RegisterShaders(Graph::DirectxRenderer * renderer);

		//Load shader info from a .sif file
		std::unique_ptr<Shaders::ShadersInfo> GenerateShadersInfoFromFile(const std::string & info);

		//Load all registered shaders, currently called on init
		void Load(Graph::DirectxRenderer * renderer);
		//Unload all registered shaders, currently called on DeInit
		void Unload();

		void Init(Graph::DirectxRenderer * renderer);
		void DeInit();

		//Get a loaded shader
		Graph::ShaderSet * Get(Shaders::SHADERS_IDX idx);

		~ShaderSetManager();

	private:
		std::vector<std::unique_ptr<Graph::ShaderSet>> shaders;
		std::string fullPath;
	};
}

