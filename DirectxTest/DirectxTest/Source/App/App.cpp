#include "App.h"


namespace App
{
	void AppCallbacks::OnInit(Graph::Scene & scene)
	{
		Graph::Geometry gm4;

		gm4.addVertex(Math::Vertex(Math::Vector3f(-40, 40, 0), Math::Color(1.f, 0.f, 0.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(-40, -40, 0), Math::Color(0.f, 0.f, 1.f, 1.f)));

		gm4.addVertex(Math::Vertex(Math::Vector3f(-40, 40, 0), Math::Color(1.f, 0.f, 0.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, 40, 0), Math::Color(0.f, 0.f, 1.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f)));

		gm4.addVertex(Math::Vertex(Math::Vector3f(-40, -40, 40), Math::Color(0.f, 0.f, 1.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 40), Math::Color(0.f, 1.f, 0.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(-40, 40, 40), Math::Color(1.f, 0.f, 0.f, 1.f)));

		gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 40), Math::Color(0.f, 1.f, 0.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, 40, 40), Math::Color(0.f, 0.f, 1.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(-40, 40, 40), Math::Color(1.f, 0.f, 0.f, 1.f)));

		gm4.addVertex(Math::Vertex(Math::Vector3f(40, 40, 0), Math::Color(0.f, 0.f, 1.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, 40, 40), Math::Color(0.f, 1.f, 0.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f)));

		gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, 40, 40), Math::Color(0.f, 1.f, 0.f, 1.f)));
		gm4.addVertex(Math::Vertex(Math::Vector3f(40, -40, 40), Math::Color(1, 0.f, 0.f, 1.f)));

		gm4.translate(Math::Vector3f(0, 0, 500));
		gm4.rotate(Math::Vector3f(0, 0, 0));

		scene.AddGeometry(gm4);
	}
	void AppCallbacks::OnPreRender(Graph::Scene & scene)
	{

	}
	void AppCallbacks::OnPostRender(Graph::Scene & scene)
	{

	}
	void AppCallbacks::OnDeInit(Graph::Scene & scene)
	{

	}
}