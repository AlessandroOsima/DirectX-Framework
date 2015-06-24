#include "Drawer.h"


namespace App
{
	Drawer::Drawer()
	{}

	void Drawer::OnInit(Graph::Scene & scene)
	{
		Graph::Geometry gm4;

		gm4.addVertex(Math::Vertex(Math::Vector3f(-40, 40, -40), Math::Color(1.f, 0.f, 0.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, -40), Math::Color(0.f, 1.f, 0.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(-40, -40, -40), Math::Color(0.f, 0.f, 1.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, 40, -40), Math::Color(0.f, 0.f, 1.f, 1.f)));

		gm4.addVertex(Math::Vertex(Math::Vector3f(-40, 40, 40), Math::Color(1.f, 0.f, 0.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 40), Math::Color(0.f, 1.f, 0.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(-40, -40, 40), Math::Color(0.f, 0.f, 1.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, 40, 40), Math::Color(0.f, 0.f, 1.f, 1.f)));


		gm4.setIndices({
			0,1,2, //front
			3,1,0, 

			5,7,4, //back
			4,6,5,

			7,5,1, //right
			1,3,7,

			4,7,3, //top
			4,3,0,

			0,6,4, //left
			0,2,6,

			2,5,6, //bottom
			1,5,2
		});

		gm4.translate(Math::Vector3f(0, 0, 500));
		//gm4.rotate(Math::Vector3f(0, 10, 0));

		scene.AddGeometry(gm4);

		geometry = &(scene.GetGeometries()[scene.GetGeometries().size() - 1]);
	}
	void Drawer::OnPreRender(Graph::Scene & scene)
	{
		const float increment = 0.005f;

		if (side)
		{
			rotAmount += increment;
		}
		else
		{
			rotAmount -= increment;
		}

		if (rotAmount > 360)
		{
			side = false;
		}

		if (rotAmount < 0)
		{
			side = true;
		}

		geometry->rotate(Math::Vector3f(0,rotAmount,0));
	}
	void Drawer::OnPostRender(Graph::Scene & scene)
	{

	}
	void Drawer::OnDeInit(Graph::Scene & scene)
	{

	}
}