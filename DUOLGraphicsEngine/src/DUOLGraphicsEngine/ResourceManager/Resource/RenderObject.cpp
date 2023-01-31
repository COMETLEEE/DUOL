#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"

namespace DUOLGraphicsEngine
{
	Transform::Transform() :
		_world(DUOLMath::Matrix::Identity)
		, _worldInvTranspose(DUOLMath::Matrix::Identity)
	{
	}

	MeshInfo::~MeshInfo()
	{
	}

	bool MeshInfo::BindPipeline(void* bufferStartPoint)
	{
		memcpy(bufferStartPoint, &_objectID, sizeof(uint64_t));
		memcpy(static_cast<char*>(bufferStartPoint) + sizeof(uint64_t) * 2, _transform, sizeof(Transform));	 // sizeof(uint64_t) * 2 (+ pad)
		return true;
	}

	SkinnedMeshInfo::~SkinnedMeshInfo()
	{
	}

	bool SkinnedMeshInfo::BindPipeline(void* bufferStartPoint)
	{
		memcpy(bufferStartPoint, &_objectID, sizeof(uint64_t));
		memcpy(static_cast<char*>(bufferStartPoint) + sizeof(uint64_t) * 2, _transform, sizeof(Transform));
		memcpy(static_cast<char*>(bufferStartPoint) + sizeof(uint64_t) * 2 + sizeof(Transform), _boneTransforms->data(), sizeof(DUOLMath::Matrix) * _boneTransforms->size());
		return true;
	}

	bool DebugInfo::BindPipeline(void* bufferStartPoint)
	{
		return true;
	}

	bool ParticleInfo::BindPipeline(void* bufferStartPoint)
	{
		{
			auto& renderingData = _particleData;

			//todo:: 나중에 최적화 할때 고칠 것
			//현재 두번복사중임
			CB_PerObject_Particle buffer(_particleData);
			memcpy(bufferStartPoint, &buffer, sizeof(CB_PerObject_Particle));
		}
		return true;
	}
}