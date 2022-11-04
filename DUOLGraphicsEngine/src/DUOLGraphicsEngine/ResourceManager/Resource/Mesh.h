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

		//subMesh´Â index¿Í material 1:1 ¸ÅÄª.
		std::vector<DUOLGraphicsLibrary::Buffer*> _indexBuffer;

		DUOLMath::Vector3 _halfExtents;;

		DUOLGraphicsLibrary::PipelineState* _pipelinestate;
	};
}