#pragma once
#include "DUOLGraphicsEngine/Export.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Particle.h"

namespace DUOLGraphicsEngine
{
	class ByteBuffer;
	class RenderingPipeline;
	class MeshBase;
	struct SubMesh;
	class Material;

#define MAX_BONE_TRANSFORM_COUNT 128

	struct DUOLGRAPHICSENGINE_EXPORT Transform
	{
		Transform();

		~Transform() = default;

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

		virtual bool BindPipeline(ByteBuffer* bufferStartPoint) abstract;

		virtual int GetInfoStructureSize() abstract;
	};

	class DUOLGRAPHICSENGINE_EXPORT MeshInfo : public IRenderInfo
	{
	public:
		MeshInfo() :
			_transform(nullptr)
			, _isOccluder(false)
		{

		}

		virtual ~MeshInfo() override;

	public:
		RenderObjectType GetRenderObjectType() override { return RenderObjectType::Mesh; }

		bool BindPipeline(ByteBuffer* bufferStartPoint) override;

		int GetInfoStructureSize() override { return sizeof(Transform) + (sizeof(uint64_t) * 2); }

		void SetTransformPointer(Transform* ptr) { _transform = ptr; }

		Transform* GetTransformPointer() { return _transform; }

		void SetObjectID(const uint64_t& objectID) { _objectID = objectID; }

		bool GetIsOccluder(){ return _isOccluder; }

		void SetIsOccluder(bool value){ _isOccluder = value; }

	private:
		uint64_t _objectID;

		Transform* _transform;

		bool _isOccluder;
	};

	struct DUOLGRAPHICSENGINE_EXPORT SkinnedMeshInfo : public IRenderInfo
	{
	public:
		SkinnedMeshInfo() :
			_transform(nullptr)
			, _isOccluder(false)
		{

		}

		virtual ~SkinnedMeshInfo() override;

	public:
		RenderObjectType GetRenderObjectType() override { return RenderObjectType::Skinned; }

		bool BindPipeline(ByteBuffer* bufferStartPoint) override;

		int GetInfoStructureSize() override { return (sizeof(uint64_t) * 2) + sizeof(Transform) + sizeof(DUOLMath::Matrix) * MAX_BONE_TRANSFORM_COUNT; }

		void SetTransformPointer(Transform* const ptr) { _transform = ptr; }

		Transform* GetTransformPointer() { return _transform; }

		void SetBoneTransforms(std::vector<DUOLMath::Matrix>* const boneTransforms) { _boneTransforms = boneTransforms; }

		void SetObjectID(uint64_t objectID) { _objectID = objectID; }

		bool GetIsOccluder() { return _isOccluder; }

		void SetIsOccluder(bool value) { _isOccluder = value; }

	private:
		uint64_t _objectID;

		Transform* _transform;

		std::vector<DUOLMath::Matrix>* _boneTransforms;

		bool _isOccluder;
	};

	// LineRender
	struct DUOLGRAPHICSENGINE_EXPORT DebugInfo : public IRenderInfo
	{
	public:
		DebugInfo()
		{

		}

	public:
		RenderObjectType GetRenderObjectType() override { return RenderObjectType::Debug; }

		int GetInfoStructureSize() override { return 0; }

		bool BindPipeline(ByteBuffer* bufferStartPoint) override;
	};

	struct DUOLGRAPHICSENGINE_EXPORT ParticleInfo : public IRenderInfo
	{
	public:
		ParticleInfo(RenderingData_Particle& renderingData) :
			_particleData(renderingData)
		{

		}
	public:
		RenderObjectType GetRenderObjectType() override { return RenderObjectType::Particle; }

		int GetInfoStructureSize() override { return sizeof(ConstantBuffDesc::CB_PerObject_Particle); }

		bool BindPipeline(ByteBuffer* bufferStartPoint) override;

		ConstantBuffDesc::CB_PerObject_Particle _particleData;

		uint64_t _objectID;

	};
	
	struct DUOLGRAPHICSENGINE_EXPORT RenderObject
	{
		MeshBase*				_mesh;

		std::vector<Material*>* _materials;

		IRenderInfo*			_renderInfo;
	};

	//struct DUOLGRAPHICSENGINE_EXPORT RenderInstancingData
	//{
	//	Material* _material;

	//	MeshBase* _mesh;

	//	UINT _submeshIdx;

	//	std::vector<IRenderInfo*> _renderinfos;
	//	//IRenderInfo* _renderInfo;

	//	bool _isRenderQueuePushed;
	//};

	struct DUOLGRAPHICSENGINE_EXPORT DecomposedRenderData
	{
		DecomposedRenderData():
			_material(nullptr)
			,_mesh(nullptr)
			,_subMesh(nullptr)
			,_renderInfo(nullptr)
			,_submeshIdx(0)
		{
			
		}

		Material* _material;

		MeshBase* _mesh;

		SubMesh* _subMesh;

		IRenderInfo* _renderInfo;

		struct BoundingBox
		{

			DUOLMath::Vector3 _worldTranslatedBoundingBoxExtent;

			float pad1;

			DUOLMath::Vector3 _worldTranslatedBoundingBoxCenterPos;

			float pad2;
		};

		BoundingBox _worldTranslatedBoundingBox;

		int _submeshIdx;
	};

}