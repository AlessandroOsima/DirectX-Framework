#include "Scene.h"
#include "DirectxRenderer.h"
#include <assert.h>

namespace Graph
{
	void Scene::Init(DirectxRenderer * renderer)
	{
		assert(renderer);

		activeRenderer = renderer;
	}

	Graph::Geometry * Scene::AddGeometry(std::unique_ptr<Graph::Geometry> && geometryData)
	{
		assert(activeRenderer);
        assert(geometryData);

        
		if (geometryData->getVertices().size() == 0)
		{
			return nullptr;
		}
        
		activeRenderer->BuildBuffersForGeometry(*geometryData, geometries.size());

        
        geometries.push_back(std::move(geometryData));

        
		Graph::Geometry * geom = geometries[geometries.size() - 1].get();

		//TODO Use a proper resource manager for textures
		geom->GetDiffuseTexture().Load(activeRenderer);

		return geom;
        
	}

	void Scene::Render()
	{
		activeRenderer->OnPreRender();

		for (int i = 0; i < geometries.size(); i++)
		{
            activeRenderer->SetPrimitiveTopology(geometries[i]->GetPrimitiveTopology());
			activeRenderer->RenderGeometry(GetAmbientLight(), GetDirectionalLights(), GetPointLights(), *geometries[i], i);
		}
		
		activeRenderer->OnPostRender();
	}
}