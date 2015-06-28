#include "Drawer.h"
#include <memory>
#include "../../Graph/Geometry.h"

namespace App
{
	Drawer::Drawer() : rotAmount(0), side(true)
	{}

	void Drawer::OnInit(Graph::Scene & scene)
	{

        std::unique_ptr<Graph::Geometry> gm1(new Graph::Geometry);

		gm1->SetPrimitiveTopology(Graph::PrimitiveTopology::TriangleList);

		gm1->addVertex(Math::Vertex(Math::Vector3f(-40, 40, 0), Math::Color(1.f, 0.f, 0.f, 1.f), Math::Vector3f(0, 0, -1)));
		gm1->addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f), Math::Vector3f(0, 0, -1)));
		gm1->addVertex(Math::Vertex(Math::Vector3f(-40, -40, 0), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0, 0, -1)));
		gm1->addVertex(Math::Vertex(Math::Vector3f(40, 40, 0), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0, 0, -1)));


        //Using vs 2012
        const int size = 6;
        const int indices[] = {
            0,1,2, //front
            3,1,0, 
        };

        std::vector<int> indicesVecor;

        indicesVecor.assign(indices, indices + size);

		gm1->setIndices(indicesVecor);

        //gm1->translate(Math::Vector3f(0, 0, 500));

		gm1->rotate(Math::Vector3f(0, 0, 0));

		geometry = scene.AddGeometry(std::move(gm1));

        /*
		std::unique_ptr<Graph::Geometry> gm4(new Graph::Geometry);

        gm4->SetPrimitiveTopology(Graph::PrimitiveTopology::TriangleList);

		gm4->addVertex(Math::Vertex(Math::Vector3f(-40, 40, -40), Math::Color(1.f, 0.f, 0.f, 1.f), Math::Vector3f(0,0,0)));
		gm4->addVertex(Math::Vertex(Math::Vector3f(40, -40, -40), Math::Color(0.f, 1.f, 0.f, 1.f), Math::Vector3f(0,0,0)));
		gm4->addVertex(Math::Vertex(Math::Vector3f(-40, -40, -40), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0,0,0)));
		gm4->addVertex(Math::Vertex(Math::Vector3f(40, 40, -40), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0,0,0)));
           
		gm4->addVertex(Math::Vertex(Math::Vector3f(-40, 40, 40), Math::Color(1.f, 0.f, 0.f, 1.f), Math::Vector3f(0,0,0)));
		gm4->addVertex(Math::Vertex(Math::Vector3f(40, -40, 40), Math::Color(0.f, 1.f, 0.f, 1.f), Math::Vector3f(0,0,0)));
		gm4->addVertex(Math::Vertex(Math::Vector3f(-40, -40, 40), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0,0,0)));
		gm4->addVertex(Math::Vertex(Math::Vector3f(40, 40, 40), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0,0,0)));

        //Using vs 2012
        const int size = 36;
        const int indices[] = {
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
        };

        std::vector<int> indicesVecor;

        indicesVecor.assign(indices, indices + size);

		gm4->setIndices(indicesVecor);

		gm4->translate(Math::Vector3f(0, 0, 500));

		geometry = scene.AddGeometry(std::move(gm4));

        
        std::unique_ptr<Graph::Geometry> gm3(new Graph::Geometry);

        gm3->SetPrimitiveTopology(Graph::PrimitiveTopology::TriangleList);
        gm3->setVertices(geometry->getVertices());
        gm3->setIndices(geometry->getIndices());
        
        gm3->translate(Math::Vector3f(0,125,500));

        otherGeometry = scene.AddGeometry(std::move(gm3));

         std::unique_ptr<Graph::Geometry> gm5(new Graph::Geometry);

         gm5->SetPrimitiveTopology(Graph::PrimitiveTopology::LineList);

         gm5->addVertex(Math::Vertex(Math::Vector3f(50, 50, 0), Math::Color(1.f, 0.f, 0.f, 1.f)));
         gm5->addVertex(Math::Vertex(Math::Vector3f(-50, -50, 0), Math::Color(0.f, 1.f, 1.f, 0.f)));

         gm5->addIndex(0);
         gm5->addIndex(1);

         gm5->translate(Math::Vector3f(0, -100, 500));

         scene.AddGeometry(std::move(gm5));

         */

	}
	void Drawer::OnPreRender(Graph::Scene & scene)
	{
        if(!geometry)
        {
            return;
        }

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

		//geometry->rotate(Math::Vector3f(0,rotAmount,0));
        //otherGeometry->rotate(Math::Vector3f(rotAmount,0,0));
	}
	void Drawer::OnPostRender(Graph::Scene & scene)
	{

	}
	void Drawer::OnDeInit(Graph::Scene & scene)
	{

	}
}