#pragma once
#include "DUOLGraphicsEngine/Export.h"
#include "RenderConstantBuffer.h"
#include "Particle.h"

namespace DUOLGraphicsEngine
{
	class RenderingPipeline;
	class MeshBase;
	class Material;

#define MAX_BONE_TRANSFORM_COUNT 128

	struct DUOLGRAPHICSENGINE_EXPORT Transform
	{
		DUOLMath::Matrix _world;

		DUOLMath::Matrix _worldInvTranspose;
	};

	enum class RenderObjectType
	{
		Mesh
		, Skinned
		, Particle
		, UI
		, Debug
	};

	class DUOLGRAPHICSENGINE_EXPORT IRenderInfo
	{
	protected:
		IRenderInfo() = default;

		virtual ~IRenderInfo() = default;

	public:
		virtual RenderObjectType GetRenderObjectType() abstract;

		virtual bool BindPipeline(void* bufferStartPoint) abstract;

		virtual int GetInfoStructureSize() abstract;
	};

	class DUOLGRAPHICSENGINE_EXPORT MeshInfo : public IRenderInfo
	{
	public:
		MeshInfo() :
			_transform(nullptr)
		{

		}

	public:
		RenderObjectType GetRenderObjectType() override { return RenderObjectType::Mesh; }

		bool BindPipeline(void* bufferStartPoint) override;

		int GetInfoStructureSize() override { return sizeof(Transform); }

		void SetTransformPointer(Transform* ptr) { _transform = ptr; }

	private:
		Transform* _transform;
	};

	struct DUOLGRAPHICSENGINE_EXPORT SkinnedMeshInfo : public IRenderInfo
	{
	public:
		SkinnedMeshInfo() :
			_transform(nullptr)
		{

		}

	public:
		RenderObjectType GetRenderObjectType() override { return RenderObjectType::Skinned; }

		bool BindPipeline(void* bufferStartPoint) override;

		int GetInfoStructureSize() override { return sizeof(Transform) + sizeof(DUOLMath::Matrix) * MAX_BONE_TRANSFORM_COUNT; }

		void SetTransformPointer(Transform* const ptr) { _transform = ptr; }

		void SetBoneTransforms(std::vector<DUOLMath::Matrix>* const boneTransforms) { _boneTransforms = boneTransforms; }

	private:
		Transform* _transform;

		std::vector<DUOLMath::Matrix>* _boneTransforms;
	};

	//LineRender
	struct DUOLGRAPHICSENGINE_EXPORT DebugInfo : public IRenderInfo
	{
	public:
		DebugInfo()
		{

		}

	public:
		RenderObjectType GetRenderObjectType() override { return RenderObjectType::Debug; }

		int GetInfoStructureSize() override { return 0; }

		bool BindPipeline(void* bufferStartPoint) override;
	};

	struct DUOLGRAPHICSENGINE_EXPORT ParticleInfo : public IRenderInfo
	{
	public:
		ParticleInfo()
		{

		}

	public:
		RenderObjectType GetRenderObjectType() override { return RenderObjectType::Particle; }

		int GetInfoStructureSize() override { return 256; }

		bool BindPipeline(void* bufferStartPoint) override;

		RenderingData_Particle _particleData;
	};

	struct RenderObject
	{
		MeshBase* _mesh;

		IRenderInfo* _renderInfo;

		std::vector<Material*>* _materials;
	};
}