#pragma once
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"

#include "unordered_map"
//for GPU Instancing

namespace DUOLGraphicsLibrary
{
	class Buffer;
}

namespace DUOLGraphicsEngine
{
	extern constexpr int _maxInstancingCount = 128;
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
		MeshBase* mesh;
		std::vector<DecomposedRenderData> objects;
};

	struct MaterialGroup
	{
		Material* _material;
		std::vector<MeshGroup>  _meshGroups

	};


	class InstancingManager
	{
	public:
		InstancingManager();


	public:
		std::unordered_map<Material*,  >

	private:
		DUOLGraphicsLibrary::Buffer* _instancingBuffer;

		


	};
}
