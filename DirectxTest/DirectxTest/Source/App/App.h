#pragma once
#include "../Graph/Scene.h"
namespace App
{
	class AppCallbacks
	{
	public:

		virtual void OnInit(Graph::Scene & scene) = 0;
		virtual void OnPreRender(Graph::Scene & scene) = 0;
		virtual void OnPostRender(Graph::Scene & scene) = 0;
		virtual void OnDeInit(Graph::Scene & scene) = 0;
		virtual ~AppCallbacks() {}
	};
}