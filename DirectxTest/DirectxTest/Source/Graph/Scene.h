#pragma once

#include <vector>
#include <memory>
#include "Geometry.h"
#include "Directx.h"


namespace Graph
{
    class DirectxRenderer;
}

namespace Graph
{
	class Scene
	{
	public:
		Graph::Geometry * AddGeometry(std::unique_ptr<Geometry> && geometryData);
		inline const std::vector<std::unique_ptr<Geometry>> & Scene::GetGeometries();

		void Init(DirectxRenderer * renderer);
		void Render();

	private:
		std::vector<std::unique_ptr<Geometry>> geometries;
		DirectxRenderer * activeRenderer;

	};

	const std::vector<std::unique_ptr<Geometry>> & Scene::GetGeometries() 
	{
		return geometries;
	}
}