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

		inline const std::vector<int> & getIndices() const
		{
			return indices;
		}

        inline void setVertices(const std::vector<Math::Vertex> & vertices)
        {
            this->vertices = vertices;
        }
		inline void setIndices(const std::vector<int> & indices)
		{
			this->indices = indices;
		}

        inline void addVertex(const Math::Vertex & vertex)
        {
            vertices.push_back(vertex);
        }

		inline void addIndex(int index)
		{
			indices.push_back(index);
		}

        void rotate(const Math::Vector3f & rotate);

        void translate(const Math::Vector3f & translate);

        void scale(const Math::Vector3f & scaleAmount);

        inline Math::Matrix44 getWorld() const
        {
            return rotateMatrix * scaleMatrix * translateMatrix;
        }

    private:
        std::vector<Math::Vertex> vertices;
		std::vector<int> indices;

        Math::Matrix44 scaleMatrix;
        Math::Matrix44 translateMatrix;
        Math::Matrix44 rotateMatrix;
    };
}