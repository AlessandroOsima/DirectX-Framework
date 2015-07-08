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
	typedef std::array<std::unique_ptr<DirectionalLightProperties>, Graph::Constants::MAX_DIRECTIONAL_LIGHTS> DirectionalLightsArray;
	typedef std::array<std::unique_ptr<PointLightProperties>, Graph::Constants::MAX_POINT_LIGHTS> PointLightsArray;

	class Scene
	{
	public:

		Graph::Geometry * AddGeometry(std::unique_ptr<Geometry> && geometryData);
		inline const std::vector<std::unique_ptr<Geometry>> & Scene::GetGeometries();

		void Init(DirectxRenderer * renderer);
		void Render();

        inline const DirectionalLightsArray & GetDirectionalLights() const
        {
            return directionalLights;
        }

        inline const PointLightsArray & GetPointLights() const
        {
            return pointLights;
        }

		inline DirectionalLightProperties * addDirectionalLight(unsigned int index, std::unique_ptr<DirectionalLightProperties> && directionalLightProperties)
        {
            directionalLights[index] = std::move(directionalLightProperties);


			return directionalLights[index].get();
        }

        inline PointLightProperties * addPointLight(unsigned int index, std::unique_ptr<PointLightProperties> && pointLightProperties) 
        {
            pointLights[index] = std::move(pointLightProperties);
            return pointLights[index].get();
        }

	private:
		std::vector<std::unique_ptr<Geometry>> geometries;
		DirectxRenderer * activeRenderer;

        DirectionalLightsArray directionalLights;
		PointLightsArray pointLights;
	};

	const std::vector<std::unique_ptr<Geometry>> & Scene::GetGeometries() 
	{
		return geometries;
	}
}