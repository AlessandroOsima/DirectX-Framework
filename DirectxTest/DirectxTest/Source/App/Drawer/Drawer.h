#pragma once
#include "../App.h"

namespace Graph
{
    class Geometry;
}

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
        Graph::Geometry * otherGeometry;
		float rotAmount;
		bool side;
	};

}