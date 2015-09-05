#pragma once 

#include "../App/App.h"
#include "../Graph/Scene.h"
#include "../Resources/ShaderSetManager.h"

class Platform
{
public:
	virtual Resources::ShaderSetManager & GetShaderSetManager() = 0;
	virtual Graph::Scene & GetScene() = 0;
};

extern Platform * GCurrentPlatform;