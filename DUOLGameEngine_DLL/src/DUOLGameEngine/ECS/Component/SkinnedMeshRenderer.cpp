#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/ECS/Object/Mesh.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Object/Material.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::SkinnedMeshRenderer>("SkinnedMeshRenderer")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_skinnedMesh", &DUOLGameEngine::SkinnedMeshRenderer::_skinnedMesh)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		// 이름 검색
		, metadata(DUOLCommon::MetaDataType::SerializeByString, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::Resource)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::SkinnedMesh)
	)
	.property("RimColor", &DUOLGameEngine::SkinnedMeshRenderer::GetRimColor, &DUOLGameEngine::SkinnedMeshRenderer::SetRimColor)
	(
	metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Color)
	)
	.property("RimPower", &DUOLGameEngine::SkinnedMeshRenderer::GetRimPower, &DUOLGameEngine::SkinnedMeshRenderer::SetRimPower)
	(
	metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("EnableRimLight", &DUOLGameEngine::SkinnedMeshRenderer::GetRimLight, &DUOLGameEngine::SkinnedMeshRenderer::SetRimLight)
	(
	metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	);

/*.property("_rootBone", &DUOLGameEngine::SkinnedMeshRenderer::_rootBone)
(
	metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
	, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::FileUUID)
);*/
}

namespace DUOLGameEngine
{
	SkinnedMeshRenderer::SkinnedMeshRenderer() :
		RendererBase(nullptr, TEXT("SkinnedMeshRenderer"))
	{
		_renderObjectInfo._renderInfo = &_skinnedMeshInfo;
		_skinnedMeshInfo.SetTransformPointer(&_transformInfo);
	}

	SkinnedMeshRenderer::SkinnedMeshRenderer(DUOLGameEngine::GameObject* owner,
		const DUOLCommon::tstring& name) :
		RendererBase(owner, name)
	{
		_renderObjectInfo._renderInfo = &_skinnedMeshInfo;
		_skinnedMeshInfo.SetTransformPointer(&_transformInfo);
	}

	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
	}

	Animator* SkinnedMeshRenderer::SearchAnimatorComponent(DUOLGameEngine::GameObject* gameObject)
	{
		const auto animator = gameObject->GetComponent<Animator>();

		if (animator)
			return animator;
		else
		{
			const auto parent = gameObject->GetTransform()->GetParent();

			if (parent)
				return SearchAnimatorComponent(parent->GetGameObject());
		}
		return nullptr;
	}



	DUOLGraphicsEngine::SkinnedMeshInfo& SkinnedMeshRenderer::GetSkinnedMeshInfo()
	{
		return _skinnedMeshInfo;
	}

	DUOLGameEngine::Mesh* SkinnedMeshRenderer::GetSkinnedMesh()
	{
		return _skinnedMesh;
	}

	void SkinnedMeshRenderer::SetSkinnedMesh(DUOLGameEngine::Mesh* mesh)
	{
		_skinnedMesh = mesh;
	}

	DUOLGameEngine::Transform* SkinnedMeshRenderer::GetRootBone()
	{
		return _rootBone;
	}

	void SkinnedMeshRenderer::SetRootBone(DUOLGameEngine::Transform* rootBone)
	{
		_rootBone = rootBone;
	}

	void SkinnedMeshRenderer::SetRimLight(bool value)
	{
		_skinnedMeshInfo.SetRenderFlag(DUOLGraphicsEngine::RenderFlags::RimLight, value);
	}

	bool SkinnedMeshRenderer::GetRimLight()
	{
		return 	_skinnedMeshInfo.GetRenderFlag(DUOLGraphicsEngine::RenderFlags::RimLight);
	}

	void SkinnedMeshRenderer::SetRimColor(DUOLMath::Vector3 albedo)
	{
		DUOLMath::Vector4 input = DUOLMath::Vector4{ albedo.x, albedo.y, albedo.z, 1.f };

		_skinnedMeshInfo.SetRimColor(input);
	}

	void SkinnedMeshRenderer::SetRimPower(float value)
	{
		_skinnedMeshInfo.SetRimPower(value);
	}

	DUOLMath::Vector3 SkinnedMeshRenderer::GetRimColor()
	{

		auto&& color = _skinnedMeshInfo.GetRimColor();
		DUOLMath::Vector3 output =
		{
			color.x, color.y, color.z
		};

		return output;
	}

	float SkinnedMeshRenderer::GetRimPower()
	{
		return _skinnedMeshInfo.GetRimPower();
	}

	void SkinnedMeshRenderer::OnStart()
	{
		if (!_animator)
			_animator = SearchAnimatorComponent(GetGameObject()); // 루트 오브젝트!!!!!!!!!!!!
	}

	void SkinnedMeshRenderer::Render()
	{
		if ((_skinnedMesh == nullptr) || (!_isEnabled))
			return;

		// 1. 메쉬 넣어줍니다.
		_renderObjectInfo._mesh = _skinnedMesh->GetPrimitiveMesh();

		Transform* transform = GetTransform();

		// 1 - 1. Transform Information Update
		const Matrix& worldTM = transform->GetWorldMatrix();

		_transformInfo._world = worldTM;
		_transformInfo._worldInvTranspose = (worldTM.Invert()).Transpose();

		// 1 - 2. Material Information Update
		// TODO : 추후 업데이트 구조 생각하면서 Material의 변동이 있는 경우에만 변환하도록 수정
		_renderObjectInfo._materials = &_primitiveMaterials;

		// 1 - 3. Bone Matrices pallet Update.
		DUOLGameEngine::GameObject* rootObject = GetGameObject()->GetTransform()->GetRootTransform()->GetGameObject(); // 루트 오브젝트!!!!!!!!!!!!

		if (!_animator)
			_animator = SearchAnimatorComponent(GetGameObject());

		// TODO : 애니메이션이 있고 애니메이터가 재생 중일때 ..
		if (_animator != nullptr)
		{
			_skinnedMeshInfo.SetBoneTransforms(_animator->GetBoneMatrices());
		}

		// 1 - 4. Root Object ID
		_skinnedMeshInfo.SetObjectID(rootObject->GetUUID());

		// _skinnedMeshInfo.SetObjectID(DUOLMath::Vector2(rootObject->GetUUID(), 1.f));

		// 2. Render Object의 참조를 보냅니다.
		GraphicsManager::GetInstance()->ReserveRenderObject(&_renderObjectInfo);
	}

	void SkinnedMeshRenderer::SetSurfaceType(SurfaceType surfaceType)
	{
		if (_materials.empty() || !_materials[_currentSelectedMaterial]) return;

		_materials[_currentSelectedMaterial]->SetSurfaceType(surfaceType, _renderObjectInfo._renderInfo->GetRenderObjectType());
	}

	SurfaceType SkinnedMeshRenderer::GetSurfaceType()
	{
		if (_materials.empty() || !_materials[_currentSelectedMaterial]) return SurfaceType::Opaque; 

		 return _materials[_currentSelectedMaterial]->GetSurfaceType();
	}
}
