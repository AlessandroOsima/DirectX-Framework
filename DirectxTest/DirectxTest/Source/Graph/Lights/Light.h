#pragma once
#include <vector>
#include "../../Math/Math.h"
#include "../../Math/Matrix44.h"
#include "../Graph.h"
#include "../Directx.h"
#include <array>
#include <memory>

namespace Graph
{
    class DirectionalLightProperties
    {
    public:
		Math::Vector4f direction = Math::Vector4f(0, 0, 0, 0);
        Math::Color color = Math::Color(0, 0, 0, 0); //w is specular
    };

    class PointLightProperties
    {
    public:
		Math::Vector4f worldPosition = Math::Vector4f(0, 0, 0, 0);
		Math::Color color = Math::Color(0, 0, 0, 0); //w is specular
    };

}