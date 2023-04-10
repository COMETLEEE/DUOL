#include "DUOLGraphicsEngine/RenderManager/InstancingManager.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"
#include "DUOLGraphicsEngine/Util/ByteBuffer.h"
#include "DUOLGraphicsLibrary/Renderer/CommandBuffer.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"

DUOLGraphicsEngine::InstancingManager::InstancingManager(ResourceManager* resourceManager)
{
	CreateInstancingBuffer(resourceManager);
	SetInstancingShader(resourceManager);
}

/**
 * \brief 
 * \param resourceManager 
 */
void DUOLGraphicsEngine::InstancingManager::CreateInstancingBuffer(ResourceManager* resourceManager)
{
	DUOLGraphicsLibrary::BufferDesc bufferDesc;

	bufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32G32B32A32_FLOAT;
	bufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
	bufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
	bufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);
	bufferDesc._size = ((sizeof(uint64_t) * 2) + sizeof(Transform) + sizeof(DUOLMath::Matrix) * MAX_BONE_TRANSFORM_COUNT + sizeof(Material::BindData)) * g_maxInstancingCount;
	bufferDesc._stride = 16;
	//bufferDesc._miscFlags = static_cast<long>(DUOLGraphicsLibrary::MiscFlags::RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS);

	_instanceBuffer = resourceManager->CreateBuffer(Hash::Hash64(_T("InstancingBuffer")), bufferDesc);
}

void DUOLGraphicsEngine::InstancingManager::SetInstancingShader(ResourceManager* resourceManager)
{
	_meshInstanceVertexShader = resourceManager->GetShader(Hash::Hash64(_T("MeshInstanceVS")));
	_skinnedMeshInstanceVertexShader = resourceManager->GetShader(Hash::Hash64(_T("SkinnedMeshInstanceVS")));
}

void DUOLGraphicsEngine::InstancingManager::RegistInstanceQueue(const std::vector<DecomposedRenderData>& renderObjects)
{
	for (auto& object : renderObjects)
	{
		//인스턴스큐에 이미 같은 머터리얼종류가 있는지 체크합니다.
		auto hasMaterial = _instanceQueue.find(object._material);

		if (hasMaterial == _instanceQueue.end())
		{
			//존재하는 머테리얼 데이터가 없으므로 추가해줍니다.
			MaterialGroup materialGroup;
			materialGroup._material = object._material;

			_instanceQueue.emplace(object._material, materialGroup);

			hasMaterial = _instanceQueue.find(object._material);
		}

		//이제 hasMaterial에 있는 메시그룹에 데이터를 추가합니다

		auto& meshGroups = hasMaterial->second._meshGroups;

		//큐에 메시그룹이 이미 있는지 체크합니다.
		auto hasMeshGroup = meshGroups.find(object._subMesh);

		if (hasMeshGroup == meshGroups.end())
		{
			//존재하는 메쉬가 없으므로 추가해줍니다.
			MeshGroup meshGroup;
			meshGroup._mesh = object._mesh;

			meshGroup._objects.push_back(object);
			meshGroups.emplace(object._subMesh, meshGroup);
		}
		else
		{
			//이미 그룹이 존재하므로 오브젝트 목록에 자신을 추가합니다.
			hasMeshGroup->second._objects.push_back(object);
		}
	}
}

void DUOLGraphicsEngine::InstancingManager::RegistInstanceQueue(const DecomposedRenderData& object)
{
	//인스턴스큐에 이미 같은 머터리얼종류가 있는지 체크합니다.
	auto hasMaterial = _instanceQueue.find(object._material);

	if (hasMaterial == _instanceQueue.end())
	{
		//존재하는 머테리얼 데이터가 없으므로 추가해줍니다.
		MaterialGroup materialGroup;
		materialGroup._material = object._material;

		_instanceQueue.emplace(object._material, materialGroup);

		hasMaterial = _instanceQueue.find(object._material);
	}

	//이제 hasMaterial에 있는 메시그룹에 데이터를 추가합니다

	auto& meshGroups = hasMaterial->second._meshGroups;

	//큐에 메시그룹이 이미 있는지 체크합니다.
	auto hasMeshGroup = meshGroups.find(object._subMesh);

	if (hasMeshGroup == meshGroups.end())
	{
		//존재하는 메쉬가 없으므로 추가해줍니다.
		MeshGroup meshGroup;
		meshGroup._mesh = object._mesh;

		meshGroup._objects.push_back(object);
		meshGroups.emplace(object._subMesh, meshGroup);
	}
	else
	{
		//이미 그룹이 존재하므로 오브젝트 목록에 자신을 추가합니다.
		hasMeshGroup->second._objects.push_back(object);
	}
}

void DUOLGraphicsEngine::InstancingManager::ClearInstanceQueue()
{
	for (auto& materialGroup : _instanceQueue)
	{
		for (auto& meshGroup : materialGroup.second._meshGroups)
		{
			meshGroup.second._objects.clear();
		}
	}
}

void DUOLGraphicsEngine::InstancingManager::ClearManager()
{
	_instanceQueue.clear();
}

void DUOLGraphicsEngine::InstancingManager::Render(DUOLGraphicsLibrary::CommandBuffer* commandBuffer,
	DUOLGraphicsLibrary::ResourceViewLayout& textureLayout, ByteBuffer* buffer)
{
	for (auto& material : _instanceQueue)
	{
		commandBuffer->SetPipelineState(material.first->GetPipelineState());

		for (auto& meshGroup : material.second._meshGroups)
		{
			if (meshGroup.second._objects.size() > 0)
			{
				auto meshType = meshGroup.second._mesh->GetMeshType();;

				if(meshType == MeshBase::MeshType::Mesh)
				{
					commandBuffer->SetShader(_meshInstanceVertexShader);
				}
				else if(meshType == MeshBase::MeshType::SkinnedMesh)
				{
					commandBuffer->SetShader(_skinnedMeshInstanceVertexShader);
				}
				else
				{
					//스태틱과 스키닝 메쉬만 인스턴싱을 지원합니다.
					continue;
				}


				//버텍스 버퍼 바인딩
				commandBuffer->SetVertexBuffer(meshGroup.second._mesh->_vertexBuffer);
				commandBuffer->SetIndexBuffer(meshGroup.first->_indexBuffer);

				int objectCount = 0;
				int bufferStride = 0;

				int renderObjectBufferSize = meshGroup.second._objects[0]._renderInfo->GetInfoStructureSize();
				int materialBufferSize = meshGroup.second._objects[0]._material->GetBindDataSize();
				//셰이더 바인딩
 				bufferStride = renderObjectBufferSize + materialBufferSize;

				for (auto mesh : meshGroup.second._objects)
				{
					int bufferStartPoint = bufferStride * objectCount;
					//constantBuffer Update
					mesh._renderInfo->BindPipeline(buffer, bufferStartPoint);
					mesh._material->BindPipeline(buffer, &textureLayout, bufferStartPoint + renderObjectBufferSize);

					objectCount++;

					//최대 인스턴스 크기를 넘어버리면 나눠서 드로잉 합니다.
					if(objectCount >= g_maxInstancingCount)
					{
						commandBuffer->UpdateBuffer(_instanceBuffer, 0, buffer->GetBufferStartPoint(), bufferStride * objectCount);

						//머터리얼에 등록된 텍스쳐를 바인딩한다.
						commandBuffer->SetResources(textureLayout);
						commandBuffer->SetResource(_instanceBuffer, 3, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), (static_cast<long>(DUOLGraphicsLibrary::StageFlags::VERTEXSTAGE) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::VERTEXSTAGE)));

						commandBuffer->DrawIndexedInstanced(meshGroup.first->_drawIndex, objectCount, 0, 0, 0);

						objectCount = 0;
					}
				}

				 commandBuffer->UpdateBuffer(_instanceBuffer, 0, buffer->GetBufferStartPoint(), bufferStride * objectCount);

				//머터리얼에 등록된 텍스쳐를 바인딩한다.
				commandBuffer->SetResources(textureLayout);
				commandBuffer->SetResource(_instanceBuffer, 3, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), (static_cast<long>(DUOLGraphicsLibrary::StageFlags::VERTEXSTAGE) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::VERTEXSTAGE)));

				commandBuffer->DrawIndexedInstanced(meshGroup.first->_drawIndex, objectCount, 0, 0, 0);
			}
		}
	}
}


