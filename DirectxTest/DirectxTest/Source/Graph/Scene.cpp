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

	void Scene::AddGeometry(const Graph::Geometry & geometryData)
	{
		if (geometryData.getVertices().size() == 0)
		{
			return;
		}

		activeRenderer->BuildBuffersForGeometry(geometryData, geometries.size());

		geometries.push_back(geometryData);

		assert(activeRenderer);
	}

	void Scene::Render()
	{
		activeRenderer->OnPreRender();

		for (int i = 0; i < geometries.size(); i++)
		{
			activeRenderer->RenderGeometry(geometries[i], i);
		}
		
		activeRenderer->OnPostRender();
	}
}