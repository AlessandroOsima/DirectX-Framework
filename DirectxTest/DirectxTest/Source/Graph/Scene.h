#pragma once

#include <vector>
#include <memory>
#include "Geometry.h"
#include "Directx.h"
#include "Lights/Light.h"
#include <array>


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

		inline DirectionalLightProperties * GetDirectionalLights()
        {
            return directionalLights;
        }

		inline PointLightProperties * GetPointLights()
        {
            return pointLights;
        }

		inline const Math::Color & GetAmbientLight()
		{
			return ambientLight;
		}

		inline void SetAmbientLight(const Math::Color & ambientLight)
		{
			this->ambientLight = ambientLight;
		}

	private:
		std::vector<std::unique_ptr<Geometry>> geometries;
		DirectxRenderer * activeRenderer;

		DirectionalLightProperties directionalLights[Graph::Constants::MAX_DIRECTIONAL_LIGHTS];
		PointLightProperties pointLights[Graph::Constants::MAX_POINT_LIGHTS];

		Math::Color ambientLight;
	};

	const std::vector<std::unique_ptr<Geometry>> & Scene::GetGeometries() 
	{
		return geometries;
	}
}