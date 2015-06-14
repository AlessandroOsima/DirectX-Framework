#pragma once
#include "../App.h"
#include "../../Graph/Geometry.h"

namespace App
{
	class Drawer : public AppCallbacks
	{
	public:

		Drawer();
		virtual void OnInit(Graph::Scene & scene) override;
		virtual void OnPreRender(Graph::Scene & scene) override;
		virtual void OnPostRender(Graph::Scene & scene) override;
		virtual void OnDeInit(Graph::Scene & scene) override;

	private:
		Graph::Geometry * geometry;
		float rotAmount = 0;
		bool side = true;
	};

}