                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    #include "Drawer.h"
#include <memory>
#include "../../Graph/Geometry.h"
#include "../../Graph/DirectxRenderer.h"
#include "../../Graph/Lights/Light.h"
#include "../../Resources/Resources.h"


namespace App
{
	Drawer::Drawer() : rotAmount(0), side(true)
	{}

	void Drawer::OnInit(Graph::Scene & scene)
	{
		scene.SetAmbientLight(Math::Color(0.2f, 0.2f, 0.2f, 1.f));

		Graph::PointLightProperties * pl1 = &scene.GetPointLights()[0];
		pl1->worldPosition = Math::Vector4f(-50, 50, 150, 0); 
		pl1->color = Math::Color(0.98f, 0.8f, 0.0f, 500);
		pl1->attenuation = {1, 0.0025f, 0.00065f, 0};


		Graph::PointLightProperties * pl2 = &scene.GetPointLights()[1];
		pl2->worldPosition = Math::Vector4f(50, 50, 150, 0);
		pl2->color = Math::Color(1.f, 0.f, 0.f, 200);
		pl2->attenuation = { 1, 0.0015f, 0.00065f, 0 };

		Graph::PointLightProperties * pl3 = &scene.GetPointLights()[2];
		pl3->worldPosition = Math::Vector4f(-50, -50, 150, 0);
		pl3->color = Math::Color(0.f, 0.f, 1.f, 300);
		pl3->attenuation = { 1, 0.0025f, 0.00065f, 0 };

		scene.SetActivePointLights(3);

        std::unique_ptr<Graph::Geometry> gm1(new Graph::Geometry);

		gm1->SetPrimitiveTopology(Graph::PrimitiveTopology::TriangleList);

		gm1->addVertex(Math::Vertex(Math::Vector3f(-40, 40, 0), Math::Color(1.f, 0.f, 0.f, 1.f), Math::Vector3f(0, 0, -1), {0,0}));
		gm1->addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f), Math::Vector3f(0, 0, -1), {1,1}));
		gm1->addVertex(Math::Vertex(Math::Vector3f(-40, -40, 0), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0, 0, -1), {0,1}));
		gm1->addVertex(Math::Vertex(Math::Vector3f(40, 40, 0), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0, 0, -1), {1,0}));

		std::unique_ptr<Resources::Texture2d> texture = std::make_unique<Resources::Texture2d>();
		texture->SetFilename("WoodDiffuseTest.dds");

		gm1->SetDiffuseTexture(std::move(texture));
		
		gm1->SetShaderSet(Resources::Shaders::SHADERS_IDX::BLINN_PHONG_TEXTURE);

        //Using vs 2012
        const int size = 6;
        const int indices[] = {
            0,1,2, //front
            3,1,0, 
        };

        std::vector<int> indicesVecor;

        indicesVecor.assign(indices, indices + size);

		gm1->setIndices(indicesVecor);

        gm1->translate(Math::Vector3f(-40, 0, 200));

        gm1->scale(Math::Vector3f(0.8f, 0.8f, 0.8f));

		gm1->rotate(Math::Vector3f(0, -30, 0));

		geometry = scene.AddGeometry(std::move(gm1));


		
		std::unique_ptr<Graph::Geometry> gm2(new Graph::Geometry);

		gm2->SetPrimitiveTopology(Graph::PrimitiveTopology::TriangleList);

		gm2->addVertex(Math::Vertex(Math::Vector3f(-40, 40, 0), Math::Color(1.f, 0.f, 0.f, 1.f), Math::Vector3f(0, 0, -1), { 0, 0 }));
		gm2->addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f), Math::Vector3f(0, 0, -1), { 1, 1 }));
		gm2->addVertex(Math::Vertex(Math::Vector3f(-40, -40, 0), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0, 0, -1), { 0, 1 }));
		gm2->addVertex(Math::Vertex(Math::Vector3f(40, 40, 0), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0, 0, -1), { 1, 0 }));

		gm2->setIndices(indicesVecor);

		gm2->translate(Math::Vector3f(40, 0, 200));

		gm2->scale(Math::Vector3f(0.8f, 0.8f, 0.8f));

		gm2->rotate(Math::Vector3f(0, 40, 0));

		std::unique_ptr<Resources::Texture2d> diffuseTextureGm2 = std::make_unique<Resources::Texture2d>();
		diffuseTextureGm2->SetFilename("MetalPaintedDiffuse.dds");

		gm2->SetShaderSet(Resources::Shaders::SHADERS_IDX::BLINN_PHONG_TEXTURE);

		gm2->SetDiffuseTexture(std::move(diffuseTextureGm2));

		scene.AddGeometry(std::move(gm2));
		

		/*

		std::unique_ptr<Graph::Geometry> gm3(new Graph::Geometry);

		gm3->SetPrimitiveTopology(Graph::PrimitiveTopology::TriangleList);

		gm3->addVertex(Math::Vertex(Math::Vector3f(-40, 40, 0), Math::Color(1.f, 0.f, 0.f, 1.f), Math::Vector3f(0, 0, -1)));
		gm3->addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f), Math::Vector3f(0, 0, -1)));
		gm3->addVertex(Math::Vertex(Math::Vector3f(-40, -40, 0), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0, 0, -1)));
		gm3->addVertex(Math::Vertex(Math::Vector3f(40, 40, 0), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0, 0, -1)));
		
		gm3->setIndices(indicesVecor);
		
		gm3->translate(Math::Vector3f(0, 0, 200));
		
		gm3->scale(Math::Vector3f(0.5f, 0.5f, 0.5f));
		
		gm3->rotate(Math::Vector3f(0, 0, 0));

		scene.AddGeometry(std::move(gm3));


		std::unique_ptr<Graph::Geometry> gm4(new Graph::Geometry);

		gm4->SetPrimitiveTopology(Graph::PrimitiveTopology::TriangleList);

		gm4->addVertex(Math::Vertex(Math::Vector3f(-40, 40, 0), Math::Color(1.f, 0.f, 0.f, 1.f), Math::Vector3f(0, 0, -1)));
		gm4->addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f), Math::Vector3f(0, 0, -1)));
		gm4->addVertex(Math::Vertex(Math::Vector3f(-40, -40, 0), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0, 0, -1)));
		gm4->addVertex(Math::Vertex(Math::Vector3f(40, 40, 0), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0, 0, -1)));

		gm4->setIndices(indicesVecor);

		gm4->translate(Math::Vector3f(50, 0, 200));

		gm4->scale(Math::Vector3f(0.5f, 0.5f, 0.5f));

		gm4->rotate(Math::Vector3f(0, 0, 0));


		scene.AddGeometry(std::move(gm4));
		*/

		
		/*std::unique_ptr<Graph::Geometry> gm5(new Graph::Geometry);

		gm5->SetPrimitiveTopology(Graph::PrimitiveTopology::TriangleList);

		gm5->addVertex(Math::Vertex(Math::Vector3f(-40, 40, 0), Math::Color(1.f, 0.f, 0.f, 1.f), Math::Vector3f(0, 0, -1)));
		gm5->addVertex(Math::Vertex(Math::Vector3f(40, -40, 0), Math::Color(0.f, 1.f, 0.f, 1.f), Math::Vector3f(0, 0, -1)));
		gm5->addVertex(Math::Vertex(Math::Vector3f(-40, -40, 0), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0, 0, -1)));
		gm5->addVertex(Math::Vertex(Math::Vector3f(40, 40, 0), Math::Color(0.f, 0.f, 1.f, 1.f), Math::Vector3f(0, 0, -1)));

		gm5->setIndices(indicesVecor);

		gm5->translate(Math::Vector3f(pl1->worldPosition.x, pl1->worldPosition.y, pl1->worldPosition.z));

		gm5->scale(Math::Vector3f(0.1f, 0.1f, 0.1f));

		geometry = scene.AddGeometry(std::move(gm5));*/
		

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

		if (rotAmount > 90)
		{
			side = false;
		}

		if (rotAmount < 0)
		{
			side = true;
		}

		geometry->rotate(Math::Vector3f(0, -rotAmount, 0));
        //otherGeometry->rotate(Math::Vector3f(rotAmount,0,0));

		Graph::PointLightProperties * pl1 = &scene.GetPointLights()[0];
		pl1->worldPosition = pl1->worldPosition + Math::Vector4f(-0.0005f, 0, 0, 0);

		Graph::PointLightProperties * pl2 = &scene.GetPointLights()[1];
		//pl2->worldPosition = pl2->worldPosition + Math::Vector4f(0, -0.0005f, 0, 0);


		//geometry->translate(Math::Vector3f(pl1->worldPosition.x, pl1->worldPosition.y, pl1->worldPosition.z));
	}
	void Drawer::OnPostRender(Graph::Scene & scene)
	{

	}
	void Drawer::OnDeInit(Graph::Scene & scene)
	{

	}
}                                                                                                                                                                                                                                                                                                                                                                                                                                                      