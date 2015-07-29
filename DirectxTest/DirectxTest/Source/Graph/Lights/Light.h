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
        Math::Vector4f direction;
        Math::Color color; //w is specular
    };

    class PointLightProperties
    {
    public:
        Math::Vector4f worldPosition;
        Math::Color color;
        float specularExponent;
        float radius;
    };

}