#include "Scene.h"
#include "DirectxRenderer.h"
#include <assert.h>
#include "../Platform/Platform.h"

namespace Graph
{
	struct vsConstantBuffer
	{
		Math::Matrix44 finalMatrix;
		Math::Matrix44 modelMatrix;
	};

	struct PerFramePSConstantBuffer
	{
		Math::Vector4f eye;
	};

	struct PerGeometryPSConstantBuffer
	{
		Math::Color ambientLight;
		DirectionalLightProperties  directionalLights[Constants::MAX_DIRECTIONAL_LIGHTS];
		PointLightProperties  pointLights[Constants::MAX_POINT_LIGHTS];
		Math::Vector4f activeLights;
	};

	void Scene::Init(DirectxRenderer * renderer)
	{
		assert(renderer);
		activeRenderer = renderer;

		projectionMatrix.identity();
		lookAtMatrix.identity();


		if (usePerspective)
		{
			projectionMatrix.perspective(Math::Deg2Rad(45), 800 / 600, 1.0f, 2000.0f);

			eyeLocation = Math::Vector4f(0, 0, 0, 0);

			Math::Vector3f eye = Math::Vector3f(eyeLocation.x, eyeLocation.y, eyeLocation.z);
			Math::Vector3f at = Math::Vector3f(0, 0, 10);
			Math::Vector3f up = Math::Vector3f(0, 1, 0);

			lookAtMatrix.lookAt(eye, at, up);

		}
		else
		{
			projectionMatrix.ortho(activeRenderer->GetCurrentWindowData().width, activeRenderer->GetCurrentWindowData().height, 1.0f, 100);

			Math::Matrix44 trasl;
			trasl.translate(Math::Vector3f(-activeRenderer->GetCurrentWindowData().width / 2, -activeRenderer->GetCurrentWindowData().height / 2, 0));

			projectionMatrix = trasl * projectionMatrix;
		}
	}

	Graph::Geometry * Scene::AddGeometry(std::unique_ptr<Graph::Geometry> && geometryData)
	{
		assert(activeRenderer);
        assert(geometryData);

        
		if (geometryData->getVertices().size() == 0)
		{
			return nullptr;
		}
        
		activeRenderer->BuildBuffersForGeometry(*geometryData, geometries.size());

        
        geometries.push_back(std::move(geometryData));

        
		Graph::Geometry * geom = geometries[geometries.size() - 1].get();

		//TODO Use a proper resource manager for textures and shaders
		geom->GetDiffuseTexture()->Load(activeRenderer);

		assert(GCurrentPlatform);

		ShaderSet * sSet = GCurrentPlatform->GetShaderSetManager().Get(geom->GetShaderSet());

		assert(sSet);

		sSet->GenerateConstantBuffer(activeRenderer);
		sSet->BindConstantBuffer(activeRenderer);

		return geom;
        
	}

	void Scene::Render()
	{
		activeRenderer->OnPreRender();

		for (int i = 0; i < geometries.size(); i++)
		{
			//Set primitive topologies
            activeRenderer->SetPrimitiveTopology(geometries[i]->GetPrimitiveTopology());

			//Compose object world and view matrices
			Math::Matrix44 finalMatrix;

			finalMatrix.identity();

			if (usePerspective)
			{
				finalMatrix = geometries[i]->getWorld() * lookAtMatrix * projectionMatrix;
			}
			else
			{
				finalMatrix = geometries[i]->getWorld() * projectionMatrix;
			}


			//Update Constant buffers
			PerFramePSConstantBuffer pfPSCBuffer;
			pfPSCBuffer.eye = eyeLocation;

			assert(GCurrentPlatform);

			Graph::ShaderSet * sSet = GCurrentPlatform->GetShaderSetManager().Get(geometries[i]->GetShaderSet());

			assert(sSet);

			activeRenderer->UseShader(sSet);


			ConstantBuffer & perFramePSConstantBuffer = sSet->GetBuffersForShader(Resources::Shaders::SHADER_TYPES::PIXEL_SHADER)[0];
			perFramePSConstantBuffer.UpdateBuffer(&pfPSCBuffer, activeRenderer);

			vsConstantBuffer cBuffer;
			cBuffer.finalMatrix = finalMatrix;
			cBuffer.modelMatrix = geometries[i]->getRotation();

			ConstantBuffer & vsBufferToUpdate = sSet->GetBuffersForShader(Resources::Shaders::SHADER_TYPES::VERTEX_SHADER)[0];
			vsBufferToUpdate.UpdateBuffer(&cBuffer, activeRenderer);

			PerGeometryPSConstantBuffer psCBuffer;
			psCBuffer.ambientLight = ambientLight;

			for (int x = 0; x < Constants::MAX_DIRECTIONAL_LIGHTS; x++)
			{
				psCBuffer.directionalLights[x] = directionalLights[x];
			}

			for (int x = 0; x < Constants::MAX_POINT_LIGHTS; x++)
			{
				psCBuffer.pointLights[x] = pointLights[x];
			}

			psCBuffer.activeLights = { (float)activeDirectionalLights, (float)activePointLights, 0, 0 };

			ConstantBuffer & perGeometryPSConstantBuffer = sSet->GetBuffersForShader(Resources::Shaders::SHADER_TYPES::PIXEL_SHADER)[1];
			perGeometryPSConstantBuffer.UpdateBuffer(&psCBuffer, activeRenderer);

			//Set resources

			ID3D11ShaderResourceView * resources[1] = { geometries[i]->GetDiffuseTexture()->GetShaderResource() };

			activeRenderer->SetResources(resources, 1, 0);

			activeRenderer->RenderGeometry(*geometries[i], i);
		}
		
		activeRenderer->OnPostRender();
	}
}