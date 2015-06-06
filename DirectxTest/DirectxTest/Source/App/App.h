#pragma once
#include "../Graph/Scene.h"
namespace App
{
	class AppCallbacks
	{
	public:

		void OnInit(Graph::Scene & scene);
		void OnPreRender(Graph::Scene & scene);
		void OnPostRender(Graph::Scene & scene);
		void OnDeInit(Graph::Scene & scene);
	};
}