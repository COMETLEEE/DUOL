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

	//todo.. 흠.. 하위 렌더오브젝트중에 겹치는건 또 묶어서 관리할 수 있을텐데...


	struct DUOLGRAPHICSENGINE_EXPORT Transform
	{
		Transform();

		~Transform() = default;

		DUOLMath::Matrix _world;

		DUOLMath::Matrix _worldInvTranspose;
	};

	enum class DUOLGRAPHICSENGINE_EXPORT RenderObjectType
	{
		Mesh
		, Skinned
		, Particle
		, UI
		, Debug
	};

	enum class DUOLGRAPHICSENGINE_EXPORT RenderFlags
	{
		ScreenSpaceReflection = 1u
		, RimLight = 1 << 1
	};

	struct DUOLGRAPHICSENGINE_EXPORT CommonRenderBindData
	{
		CommonRenderBindData() :
			_objectID(0)
			, _offset(0)
			, _renderFlag(0)
			, _rimColor(0)
			, _rimPower(0)
			, _paperBurnFirstColor(0)
			, _paperBurnSecondColor(0)
		{

		}

		uint64_t _objectID;

		float _offset; // 여기 추가하면 안될 것 같은데...! /신성현

		unsigned int _renderFlag;

		unsigned int _rimColor;

		float _rimPower;

		unsigned int _paperBurnFirstColor;

		unsigned int _paperBurnSecondColor;
	};

	class DUOLGRAPHICSENGINE_EXPORT IRenderInfo
	{
	protected:
		IRenderInfo() = default;

		virtual ~IRenderInfo() = default;

	public:
		virtual RenderObjectType GetRenderObjectType() abstract;

		virtual bool BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset = 0) abstract;

		virtual int GetInfoStructureSize() abstract;

		virtual bool IsStatic() abstract;

		virtual void SetStatic(bool value) abstract;
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

		bool BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset = 0) override;

		int GetInfoStructureSize() override { return sizeof(Transform) + sizeof(CommonRenderBindData); }

		void SetTransformPointer(Transform* ptr) { _transform = ptr; }

		Transform* GetTransformPointer() { return _transform; }

		void SetObjectID(const uint64_t& objectID) { _commonRenderData._objectID = objectID; }

		void SetPaperBurnColor(const DUOLMath::Vector4& firstColor, const DUOLMath::Vector4& secondColor);

		void SetScreenSpaceReflection(bool value);

		bool IsEnableScreenSpaceReflection();

		void SetRenderFlag(RenderFlags flag, bool value);

		bool GetRenderFlag(RenderFlags flag);

		bool GetIsOccluder() { return _isOccluder; }

		void SetIsOccluder(bool value) { _isOccluder = value; }

		float GetRimPower();

		DUOLMath::Vector4 GetRimColor();

		float GetOffset();

		void SetRimPower(float value);

		void SetRimColor(DUOLMath::Vector4 rimColor);

		void SetOffset(float offset);

		bool IsStatic() override
		{
			return _isStatic;
		};

		void SetStatic(bool value) override
		{
			_isStatic = value;
		};

	private:

		CommonRenderBindData _commonRenderData;

		Transform* _transform;

		bool _isOccluder;

		bool _isStatic;

		bool _isAlphaClipping;
	};

	struct DUOLGRAPHICSENGINE_EXPORT SkinnedMeshInfo : public IRenderInfo
	{
	public:
		SkinnedMeshInfo() :
			_transform(nullptr)
			, _isOccluder(false)
			, _commonRenderData()

		{
		}

		virtual ~SkinnedMeshInfo() override;

	public:
		RenderObjectType GetRenderObjectType() override { return RenderObjectType::Skinned; }

		bool BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset = 0) override;

		int GetInfoStructureSize() override { return sizeof(CommonRenderBindData) + sizeof(Transform) + sizeof(DUOLMath::Matrix) * MAX_BONE_TRANSFORM_COUNT; }

		void SetTransformPointer(Transform* const ptr) { _transform = ptr; }

		Transform* GetTransformPointer() { return _transform; }

		void SetBoneTransforms(std::vector<DUOLMath::Matrix>* const boneTransforms) { _boneTransforms = boneTransforms; }

		void SetObjectID(uint64_t objectID) { _commonRenderData._objectID = objectID; }

		bool GetIsOccluder() { return _isOccluder; }

		void SetIsOccluder(bool value) { _isOccluder = value; }

		float GetOffset() { return _commonRenderData._offset; }

		void SetOffset(float value) { _commonRenderData._offset = value; }

		void SetRenderFlag(RenderFlags flag, bool value);

		bool GetRenderFlag(RenderFlags flag);

		const float& GetRimPower();

		DUOLMath::Vector4 GetRimColor();

		void SetPaperBurnColor(const DUOLMath::Vector4& firstColor, const DUOLMath::Vector4& secondColor);

		const DUOLMath::Vector4& GetPaperBurnFirstColor();

		const DUOLMath::Vector4& GetPaperBurnSecondColor();

		void SetRimPower(float value);

		void SetRimColor(DUOLMath::Vector4 rimColor);

		bool IsStatic() override
		{
			return _isStatic;
		};

		void SetStatic(bool value) override
		{
			_isStatic = value;
		};

	private:
		CommonRenderBindData _commonRenderData;

		Transform* _transform;

		std::vector<DUOLMath::Matrix>* _boneTransforms;

		bool _isOccluder;

		bool _isStatic;
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

		bool BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset = 0) override;

		bool IsStatic() override
		{
			return false;
		};

		void SetStatic(bool value) override
		{
		};
	};

	struct DUOLGRAPHICSENGINE_EXPORT ParticleInfo : public IRenderInfo
	{
	public:
		ParticleInfo() :_particleData(), _isComputed(false)
		{

		}

		ParticleInfo& operator= (RenderingData_Particle& renderingData)
		{
			_particleData = renderingData;
			return *this;
		}

	public:
		RenderObjectType GetRenderObjectType() override { return RenderObjectType::Particle; }

		int GetInfoStructureSize() override { return sizeof(ConstantBuffDesc::CB_PerObject_Particle); }

		bool BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset = 0) override;

		ConstantBuffDesc::CB_PerObject_Particle _particleData;

		uint64_t _objectID;

		bool _isComputed;

		bool IsStatic() override
		{
			return false;
		};

		void SetStatic(bool value) override
		{
		};

	};

	struct DUOLGRAPHICSENGINE_EXPORT BoundingBox
	{

		DUOLMath::Vector3 _worldTranslatedBoundingBoxExtent;

		float pad1;

		DUOLMath::Vector3 _worldTranslatedBoundingBoxCenterPos;

		float pad2;
	};

	struct DUOLGRAPHICSENGINE_EXPORT RenderObject
	{
		MeshBase* _mesh;

		std::vector<Material*>* _materials;

		IRenderInfo* _renderInfo;

		BoundingBox _boundingBox;
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
		DecomposedRenderData() :
			_material(nullptr)
			, _mesh(nullptr)
			, _subMesh(nullptr)
			, _renderInfo(nullptr)
			, _submeshIdx(0)
		{

		}

		Material* _material;

		MeshBase* _mesh;

		SubMesh* _subMesh;

		IRenderInfo* _renderInfo;

		BoundingBox _worldTranslatedBoundingBox;

		int _submeshIdx;
	};

}
