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
		SubMesh() :
			_submeshIndex(0)
			, _parentMeshIndex(0)
			, _drawIndex(0)
			, _vertexBuffer(nullptr)
			, _indexBuffer(nullptr)
			, _materialID(0)
		{

		}

		//subMesh¿« ¿Œµ¶Ω∫
		int _submeshIndex;

		int _parentMeshIndex;

		DUOLGraphicsLibrary::UINT32 _drawIndex;

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