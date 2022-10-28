#pragma once
#include <vector>
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsLibrary
{
	class PipelineState;
	class Buffer;
}

namespace DUOLGraphicsEngine
{
	struct Material;

	struct Mesh
	{
		DUOLGraphicsLibrary::Buffer* _vertexBuffer;

		unsigned int _submeshCount;

		//subMesh�� index�� material 1:1 ��Ī.
		std::vector<DUOLGraphicsLibrary::Buffer*> _indexBuffer;

		DUOLMath::Vector3 _halfExtents;;

		DUOLGraphicsLibrary::PipelineState* _pipelinestate;
	};
}