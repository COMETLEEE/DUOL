#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsEngine/Util/ByteBuffer.h"

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

	bool MeshInfo::BindPipeline(ByteBuffer* bufferStartPoint)
	{
		int bufferOffset = 0;

		bufferStartPoint->WriteData(&_objectID, sizeof(uint64_t));
		bufferOffset += sizeof(uint64_t) * 2;
		bufferStartPoint->WriteData(_transform, sizeof(Transform), bufferOffset);

		return true;
	}

	SkinnedMeshInfo::~SkinnedMeshInfo()
	{
	}

	bool SkinnedMeshInfo::BindPipeline(ByteBuffer* bufferStartPoint)
	{
		int bufferOffset = 0;

		bufferStartPoint->WriteData(&_objectID, sizeof(uint64_t));
		bufferOffset += sizeof(uint64_t) * 2;
		bufferStartPoint->WriteData(_transform, sizeof(Transform), bufferOffset);
		bufferOffset += sizeof(Transform);
		bufferStartPoint->WriteData(_boneTransforms->data(), sizeof(DUOLMath::Matrix) * _boneTransforms->size(), bufferOffset);

		return true;
	}

	bool DebugInfo::BindPipeline(ByteBuffer* bufferStartPoint)
	{
		return true;
	}

	bool ParticleInfo::BindPipeline(ByteBuffer* bufferStartPoint)
	{
		{
			//todo:: 나중에 최적화 할때 고칠 것
			//현재 두번복사중임
			CB_PerObject_Particle buffer(_particleData);

			bufferStartPoint->WriteData(&buffer, sizeof(CB_PerObject_Particle));
		}

		return true;
	}
}
