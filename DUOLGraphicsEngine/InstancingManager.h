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
	//Decomposed Render Data들을 가져와서
	//인스턴싱 그룹으로 나눕니다.

	// { 머테리얼 1
	//		- 메쉬 1
	//          - 오브젝트 1, 2.....
	//      - 메쉬 2
	//			- 오브젝트 3, 4.....
	// }
	// { 머테리얼 2............... }

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
