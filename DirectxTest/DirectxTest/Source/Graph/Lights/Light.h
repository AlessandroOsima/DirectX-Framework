#pragma once
#include <vector>
#include "../../Math/Math.h"
#include "../../Math/Matrix44.h"
#include "../Graph.h"

namespace Graph
{
    class DirectionalLightProperties
    {
    public:
        Math::Vector4f worldPosition;
        Math::Color color;
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