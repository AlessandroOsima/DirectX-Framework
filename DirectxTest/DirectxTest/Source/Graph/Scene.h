#pragma once

#include <vector>
#include "Geometry.h"
#include "Directx.h"


class DirectxRenderer;

namespace Graph
{
	class Scene
	{
	public:
		void AddGeometry(const Graph::Geometry & geometryData);
		inline const std::vector<Graph::Geometry> & GetGeometries() const;

		void Init(DirectxRenderer * renderer);
		void Render();

	private:
		std::vector<Graph::Geometry> geometries;
		DirectxRenderer * activeRenderer;

	};

	const std::vector<Graph::Geometry> & Scene::GetGeometries() const
	{
		return geometries;
	}
}