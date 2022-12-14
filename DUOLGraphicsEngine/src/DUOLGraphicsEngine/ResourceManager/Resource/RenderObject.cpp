#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"

namespace DUOLGraphicsEngine
{

	bool MeshInfo::BindPipeline(void* bufferStartPoint)
	{
		memcpy(bufferStartPoint, _transform, sizeof(Transform));
		return true;
	}

	bool SkinnedMeshInfo::BindPipeline(void* bufferStartPoint)
	{
		memcpy(bufferStartPoint, _transform, sizeof(Transform));
		memcpy(static_cast<char*>(bufferStartPoint) + sizeof(Transform), _transform, sizeof(DUOLMath::Matrix) * _boneTransforms->size());
		return true;
	}

	bool DebugInfo::BindPipeline(void* bufferStartPoint)
	{
		return true;
	}
}
