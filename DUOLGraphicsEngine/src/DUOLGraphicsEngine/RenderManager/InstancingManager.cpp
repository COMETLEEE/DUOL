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
		//�ν��Ͻ�ť�� �̹� ���� ���͸��������� �ִ��� üũ�մϴ�.
		auto hasMaterial = _instanceQueue.find(object._material);

		if (hasMaterial == _instanceQueue.end())
		{
			//�����ϴ� ���׸��� �����Ͱ� �����Ƿ� �߰����ݴϴ�.
			MaterialGroup materialGroup;
			materialGroup._material = object._material;

			_instanceQueue.emplace(object._material, materialGroup);

			hasMaterial = _instanceQueue.find(object._material);
		}

		//���� hasMaterial�� �ִ� �޽ñ׷쿡 �����͸� �߰��մϴ�

		auto& meshGroups = hasMaterial->second._meshGroups;

		//ť�� �޽ñ׷��� �̹� �ִ��� üũ�մϴ�.
		auto hasMeshGroup = meshGroups.find(object._subMesh);

		if (hasMeshGroup == meshGroups.end())
		{
			//�����ϴ� �޽��� �����Ƿ� �߰����ݴϴ�.
			MeshGroup meshGroup;
			meshGroup._mesh = object._mesh;

			meshGroup._objects.push_back(object);
			meshGroups.emplace(object._subMesh, meshGroup);
		}
		else
		{
			//�̹� �׷��� �����ϹǷ� ������Ʈ ��Ͽ� �ڽ��� �߰��մϴ�.
			hasMeshGroup->second._objects.push_back(object);
		}
	}
}

void DUOLGraphicsEngine::InstancingManager::RegistInstanceQueue(const DecomposedRenderData& object)
{
	//�ν��Ͻ�ť�� �̹� ���� ���͸��������� �ִ��� üũ�մϴ�.
	auto hasMaterial = _instanceQueue.find(object._material);

	if (hasMaterial == _instanceQueue.end())
	{
		//�����ϴ� ���׸��� �����Ͱ� �����Ƿ� �߰����ݴϴ�.
		MaterialGroup materialGroup;
		materialGroup._material = object._material;

		_instanceQueue.emplace(object._material, materialGroup);

		hasMaterial = _instanceQueue.find(object._material);
	}

	//���� hasMaterial�� �ִ� �޽ñ׷쿡 �����͸� �߰��մϴ�

	auto& meshGroups = hasMaterial->second._meshGroups;

	//ť�� �޽ñ׷��� �̹� �ִ��� üũ�մϴ�.
	auto hasMeshGroup = meshGroups.find(object._subMesh);

	if (hasMeshGroup == meshGroups.end())
	{
		//�����ϴ� �޽��� �����Ƿ� �߰����ݴϴ�.
		MeshGroup meshGroup;
		meshGroup._mesh = object._mesh;

		meshGroup._objects.push_back(object);
		meshGroups.emplace(object._subMesh, meshGroup);
	}
	else
	{
		//�̹� �׷��� �����ϹǷ� ������Ʈ ��Ͽ� �ڽ��� �߰��մϴ�.
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
					//����ƽ�� ��Ű�� �޽��� �ν��Ͻ��� �����մϴ�.
					continue;
				}


				//���ؽ� ���� ���ε�
				commandBuffer->SetVertexBuffer(meshGroup.second._mesh->_vertexBuffer);
				commandBuffer->SetIndexBuffer(meshGroup.first->_indexBuffer);

				int objectCount = 0;
				int bufferStride = 0;

				int renderObjectBufferSize = meshGroup.second._objects[0]._renderInfo->GetInfoStructureSize();
				int materialBufferSize = meshGroup.second._objects[0]._material->GetBindDataSize();
				//���̴� ���ε�
 				bufferStride = renderObjectBufferSize + materialBufferSize;

				for (auto mesh : meshGroup.second._objects)
				{
					int bufferStartPoint = bufferStride * objectCount;
					//constantBuffer Update
					mesh._renderInfo->BindPipeline(buffer, bufferStartPoint);
					mesh._material->BindPipeline(buffer, &textureLayout, bufferStartPoint + renderObjectBufferSize);

					objectCount++;

					//�ִ� �ν��Ͻ� ũ�⸦ �Ѿ������ ������ ����� �մϴ�.
					if(objectCount >= g_maxInstancingCount)
					{
						commandBuffer->UpdateBuffer(_instanceBuffer, 0, buffer->GetBufferStartPoint(), bufferStride * objectCount);

						//���͸��� ��ϵ� �ؽ��ĸ� ���ε��Ѵ�.
						commandBuffer->SetResources(textureLayout);
						commandBuffer->SetResource(_instanceBuffer, 3, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), (static_cast<long>(DUOLGraphicsLibrary::StageFlags::VERTEXSTAGE) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::VERTEXSTAGE)));

						commandBuffer->DrawIndexedInstanced(meshGroup.first->_drawIndex, objectCount, 0, 0, 0);

						objectCount = 0;
					}
				}

				 commandBuffer->UpdateBuffer(_instanceBuffer, 0, buffer->GetBufferStartPoint(), bufferStride * objectCount);

				//���͸��� ��ϵ� �ؽ��ĸ� ���ε��Ѵ�.
				commandBuffer->SetResources(textureLayout);
				commandBuffer->SetResource(_instanceBuffer, 3, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), (static_cast<long>(DUOLGraphicsLibrary::StageFlags::VERTEXSTAGE) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::VERTEXSTAGE)));

				commandBuffer->DrawIndexedInstanced(meshGroup.first->_drawIndex, objectCount, 0, 0, 0);
			}
		}
	}
}


