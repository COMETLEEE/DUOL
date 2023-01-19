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

			int a = 0;

			// --------------------------------- CommonInfo ---------------------------------------------
			char* pbufferPos = static_cast<char*>(bufferStartPoint);

			auto dir = DUOLMath::Vector3(renderingData._commonInfo._transformMatrix.m[1][0], renderingData._commonInfo._transformMatrix.m[1][1], renderingData._commonInfo._transformMatrix.m[1][2]);

			memcpy(pbufferPos, &dir, sizeof(DUOLMath::Vector3));
			a += sizeof(DUOLMath::Vector3);
			pbufferPos += sizeof(DUOLMath::Vector3);

			memcpy(pbufferPos, &renderingData._commonInfo._duration, sizeof(float));
			a += sizeof(float);
			pbufferPos += sizeof(float);

			auto worldPos = DUOLMath::Vector3(renderingData._commonInfo._transformMatrix.m[3][0], renderingData._commonInfo._transformMatrix.m[3][1], renderingData._commonInfo._transformMatrix.m[3][2]);
			memcpy(pbufferPos, &worldPos, sizeof(DUOLMath::Vector3)); //emitpos
			a += sizeof(DUOLMath::Vector3);
			pbufferPos += sizeof(DUOLMath::Vector3);

			memcpy(pbufferPos, &renderingData._commonInfo._maxParticles, sizeof(float));
			a += sizeof(float);
			pbufferPos += sizeof(float);

			memcpy(pbufferPos, renderingData._commonInfo._startDelay, sizeof(renderingData._commonInfo._startDelay));
			a += sizeof(renderingData._commonInfo._startDelay);
			pbufferPos += sizeof(renderingData._commonInfo._startDelay);

			memcpy(pbufferPos, renderingData._commonInfo._startLifeTime, sizeof(renderingData._commonInfo._startLifeTime));
			a += sizeof(renderingData._commonInfo._startLifeTime);
			pbufferPos += sizeof(renderingData._commonInfo._startLifeTime);

			memcpy(pbufferPos, renderingData._commonInfo._startSpeed, sizeof(renderingData._commonInfo._startSpeed));
			a += sizeof(renderingData._commonInfo._startSpeed);
			pbufferPos += sizeof(renderingData._commonInfo._startSpeed);

			memcpy(pbufferPos, renderingData._commonInfo._startRotation, sizeof(renderingData._commonInfo._startRotation));
			a += sizeof(renderingData._commonInfo._startRotation);
			pbufferPos += sizeof(renderingData._commonInfo._startRotation);

			memcpy(pbufferPos, renderingData._commonInfo._startSize, sizeof(renderingData._commonInfo._startSize));
			a += sizeof(renderingData._commonInfo._startSize);
			pbufferPos += sizeof(renderingData._commonInfo._startSize);

			memcpy(pbufferPos, renderingData._commonInfo._startColor, sizeof(renderingData._commonInfo._startColor));
			a += sizeof(renderingData._commonInfo._startColor);
			pbufferPos += sizeof(renderingData._commonInfo._startColor);

			memcpy(pbufferPos, renderingData._commonInfo._gravityModifier, sizeof(renderingData._commonInfo._gravityModifier));
			a += sizeof(renderingData._commonInfo._gravityModifier);
			pbufferPos += sizeof(renderingData._commonInfo._gravityModifier);

			memcpy(pbufferPos, &renderingData._commonInfo._playTime, sizeof(renderingData._commonInfo._playTime));
			a += sizeof(renderingData._commonInfo._playTime);
			pbufferPos += sizeof(renderingData._commonInfo._playTime);

			memcpy(pbufferPos, &renderingData._objectID, sizeof(renderingData._objectID));
			a += sizeof(renderingData._objectID);
			pbufferPos += sizeof(renderingData._objectID);

			memcpy(pbufferPos, &renderingData._commonInfo._looping, sizeof(renderingData._commonInfo._looping));
			a += sizeof(DUOLMath::Vector4);
			pbufferPos += sizeof(DUOLMath::Vector4);
			// --------------------------------- Emission ----------------------------------------------

			memcpy(pbufferPos, &renderingData._emission._emissiveCount, sizeof(renderingData._emission._emissiveCount));
			a += sizeof(float);
			pbufferPos += sizeof(float);

			memcpy(pbufferPos, &renderingData._emission._emissiveTime, sizeof(renderingData._emission._emissiveTime));
			a += sizeof(DUOLMath::Vector3);
			pbufferPos += sizeof(DUOLMath::Vector3);

			// --------------------------------- Velocity_over_Lifetime ----------------------------------------------

			memcpy(pbufferPos, &renderingData._velocity_Over_Lifetime._linearVelocity, sizeof(renderingData._velocity_Over_Lifetime._linearVelocity));
			a += sizeof(DUOLMath::Vector4);
			pbufferPos += sizeof(DUOLMath::Vector4);

			// --------------------------------- Color_Over_Lifetime ----------------------------------------------
			memcpy(pbufferPos, &renderingData._color_Over_Lifetime._startColor, sizeof(renderingData._color_Over_Lifetime._startColor));
			a += sizeof(renderingData._color_Over_Lifetime._startColor);
			pbufferPos += sizeof(renderingData._color_Over_Lifetime._startColor);

			memcpy(pbufferPos, &renderingData._color_Over_Lifetime._endColor, sizeof(renderingData._color_Over_Lifetime._endColor));
			a += sizeof(renderingData._color_Over_Lifetime._endColor);
			pbufferPos += sizeof(renderingData._color_Over_Lifetime._endColor);
			
			// --------------------------------- Size_Over_Lifetime ----------------------------------------------

			memcpy(pbufferPos, &renderingData._size_Over_Lifetime, sizeof(renderingData._size_Over_Lifetime));
			a += sizeof(renderingData._size_Over_Lifetime);
			pbufferPos += sizeof(renderingData._size_Over_Lifetime);

			// --------------------------------- Rotation_Over_Lifetime ----------------------------------------------

			memcpy(pbufferPos, &renderingData._rotation_Over_Lifetime, sizeof(renderingData._rotation_Over_Lifetime));
			a += sizeof(renderingData._rotation_Over_Lifetime);
			pbufferPos += sizeof(DUOLMath::Vector4);

			// --------------------------------- Texture_Sheet_Animation ----------------------------------------------

			memcpy(pbufferPos, &renderingData._texture_Sheet_Animaition, sizeof(renderingData._texture_Sheet_Animaition));
			a += sizeof(renderingData._texture_Sheet_Animaition);
			pbufferPos += sizeof(renderingData._texture_Sheet_Animaition);
		}
		return true;
	}
}