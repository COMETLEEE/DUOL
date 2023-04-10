#pragma once
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"

#include "unordered_map"
//for GPU Instancing

namespace DUOLGraphicsLibrary
{
	class Shader;
	class CommandBuffer;
	class Buffer;
}

namespace DUOLGraphicsEngine
{
	class ResourceManager;
	//Decomposed Render Data���� �����ͼ�
	//�ν��Ͻ� �׷����� �����ϴ�.

	// { ���׸��� 1
	//		- �޽� 1
	//          - ������Ʈ 1, 2.....
	//      - �޽� 2
	//			- ������Ʈ 3, 4.....
	// }
	// { ���׸��� 2............... }

	struct MeshGroup
	{
		MeshBase* _mesh;
		std::vector<DecomposedRenderData> _objects;
	};

	struct MaterialGroup
	{
		Material* _material;
		std::unordered_map<SubMesh*, MeshGroup>  _meshGroups;
	};

	class InstancingManager
	{
	public:
		InstancingManager(ResourceManager* resourceManager);

	public:
		void CreateInstancingBuffer(ResourceManager* resourceManager);

		void SetInstancingShader(ResourceManager* resourceManager);

		void RegistInstanceQueue(const std::vector<DecomposedRenderData>& renderObjects);

		void RegistInstanceQueue(const DecomposedRenderData& object);

		void ClearInstanceQueue();

		void ClearManager();

		void Render(DUOLGraphicsLibrary::CommandBuffer* commandBuffer, DUOLGraphicsLibrary::ResourceViewLayout& textureLayout, ByteBuffer* buffer);

	private:
		DUOLGraphicsLibrary::Buffer* _instanceBuffer;

		std::unordered_map<Material*, MaterialGroup> _instanceQueue;

		DUOLGraphicsLibrary::Shader* _meshInstanceVertexShader;

		DUOLGraphicsLibrary::Shader* _skinnedMeshInstanceVertexShader;

		int g_maxInstancingCount = 512;
	};
}
