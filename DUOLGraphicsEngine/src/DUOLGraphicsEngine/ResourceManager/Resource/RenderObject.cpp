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

	bool MeshInfo::BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset)
	{

		bufferStartPoint->WriteData(&_objectID, sizeof(uint64_t), bufferOffset);
		bufferOffset += sizeof(uint64_t) * 2;
		bufferStartPoint->WriteData(_transform, sizeof(Transform), bufferOffset);

		return true;
	}

	SkinnedMeshInfo::~SkinnedMeshInfo()
	{
	}

	bool SkinnedMeshInfo::BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset)
	{
		bufferStartPoint->WriteData(&_objectID, sizeof(uint64_t), bufferOffset);
		bufferOffset += sizeof(uint64_t);
		bufferStartPoint->WriteData(&_offset, sizeof(float), bufferOffset);
		bufferOffset += sizeof(float) * 2;
		bufferStartPoint->WriteData(_transform, sizeof(Transform), bufferOffset);
		bufferOffset += sizeof(Transform);
		bufferStartPoint->WriteData(_boneTransforms->data(), sizeof(DUOLMath::Matrix) * _boneTransforms->size(), bufferOffset);

		return true;
	}

	bool DebugInfo::BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset)
	{
		return true;
	}

	bool ParticleInfo::BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset)
	{
		bufferStartPoint->WriteData(&_particleData, sizeof(ConstantBuffDesc::CB_PerObject_Particle), bufferOffset);

		return true;
	}
}
