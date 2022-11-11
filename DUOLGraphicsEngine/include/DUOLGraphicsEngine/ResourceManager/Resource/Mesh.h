              #pragma once
#include <vector>

#include "DUOLGraphicsLibrary/Core/Typedef.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsLibrary
{
	class PipelineState;
	class Buffer;
}

namespace DUOLGraphicsEngine
{
	struct Material;

	struct SubMesh
	{
		//subMesh¿« ¿Œµ¶Ω∫
		int _submeshIndex;

		int _parentMeshIndex;

		DUOLGraphicsLibrary::Buffer* _vertexBuffer;

		DUOLGraphicsLibrary::Buffer* _indexBuffer;

		DUOLGraphicsLibrary::UINT64 _materialID;
	};

	struct Mesh
	{
		unsigned int _submeshCount;

		std::vector<SubMesh> _subMesh;
		
		DUOLMath::Vector3 _halfExtents;
	};
}