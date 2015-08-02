#pragma once
#include <vector>
#include "../Math/Math.h"
#include "../Math/Matrix44.h"
#include "../Resources/Texture2d.h"
#include "Graph.h"
#include "ShaderSet.h"
#include <assert.h>

namespace Graph
{
    class Geometry
    {
    public:

        Geometry();

		inline std::unique_ptr<Resources::Texture2d> & GetDiffuseTexture()
		{
			return diffuseTexture;
		}

		inline const std::unique_ptr<Resources::Texture2d> & GetDiffuseTexture() const
		{
			return diffuseTexture;
		}

		inline std::unique_ptr<Resources::Texture2d> & GetNormalTexture()
		{
			return normalTexture;
		}

		inline const std::unique_ptr<Resources::Texture2d> & GetNormalTexture() const
		{
			return normalTexture;
		}

		inline void SetDiffuseTexture(std::unique_ptr<Resources::Texture2d> && texture)
		{
			assert(texture);

			diffuseTexture = std::move(texture);
		}

		inline void SetNormalTexture(std::unique_ptr<Resources::Texture2d> && texture)
		{
			assert(texture);

			diffuseTexture = std::move(texture);
		}

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

        inline void SetPrimitiveTopology(PrimitiveTopology primitiveTopology)
        {
            this->primitiveTopology = primitiveTopology;
        }

        inline PrimitiveTopology GetPrimitiveTopology()
        {
            return primitiveTopology;
        }

        void rotate(const Math::Vector3f & rotate);

        void translate(const Math::Vector3f & translate);

        void scale(const Math::Vector3f & scaleAmount);

        inline Math::Matrix44 getWorld() const
        {
            return rotateMatrix * scaleMatrix * translateMatrix;
        }

        inline const Math::Matrix44 & getRotation() const
        {
            return rotateMatrix;
        }

        inline const Math::Matrix44 & getScale() const
        {
            return scaleMatrix;
        }

        inline const Math::Matrix44 & getTranslate() const
        {
            return translateMatrix;
        }

		inline std::unique_ptr<Graph::ShaderSet> & GetShaderSet()
		{
			assert(shaderSet);
			return shaderSet;
		}


		inline const std::unique_ptr<Graph::ShaderSet> & GetShaderSet() const
		{
			assert(shaderSet);
			return shaderSet;
		}


		inline void SetShaderSet(std::unique_ptr<Graph::ShaderSet> && shaderSet)
		{
			assert(shaderSet);
			this->shaderSet = std::move(shaderSet);
		}

    private:
        std::vector<Math::Vertex> vertices;
		std::vector<int> indices;
		std::unique_ptr<Resources::Texture2d> diffuseTexture;
		std::unique_ptr<Resources::Texture2d> normalTexture;
		std::unique_ptr<Graph::ShaderSet> shaderSet;

        PrimitiveTopology primitiveTopology;

        Math::Matrix44 scaleMatrix;
        Math::Matrix44 translateMatrix;
        Math::Matrix44 rotateMatrix;
    };
}