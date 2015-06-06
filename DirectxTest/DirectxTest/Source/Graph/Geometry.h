#pragma once
#include <vector>
#include "../Math/Math.h"
#include "../Math/Matrix44.h"

namespace Graph
{
    class Geometry
    {
    public:

        Geometry();

        inline const std::vector<Math::Vertex> & getVertices() const
        {
            return vertices;
        }

        inline const std::vector<Math::Vertex> & setVertices(const std::vector<Math::Vertex> & vertices)
        {
            this->vertices = vertices;
        }

        inline void addVertex(const Math::Vertex & vertex)
        {
            vertices.push_back(vertex);
        }

        void rotate(const Math::Vector3f & rotate);

        void translate(const Math::Vector3f & translate);

        void scale(const Math::Vector3f & scaleAmount);

        inline Math::Matrix44 getWorld()
        {
            return rotateMatrix * scaleMatrix * translateMatrix;
        }

    private:
        std::vector<Math::Vertex> vertices;

        Math::Matrix44 scaleMatrix;
        Math::Matrix44 translateMatrix;
        Math::Matrix44 rotateMatrix;
    };
}