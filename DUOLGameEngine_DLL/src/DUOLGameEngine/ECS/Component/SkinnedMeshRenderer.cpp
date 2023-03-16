#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/ECS/Object/Mesh.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"


#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

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
		// �̸� �˻�
		, metadata(DUOLCommon::MetaDataType::SerializeByString, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::Resource)
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

	void SkinnedMeshRenderer::Render()
	{
		if ((_skinnedMesh == nullptr) || (!_isEnabled))
			return;

		// 1. �޽� �־��ݴϴ�.
		_renderObjectInfo._mesh = _skinnedMesh->GetPrimitiveMesh();

		Transform* transform = GetTransform();

		// 1 - 1. Transform Information Update
		const Matrix& worldTM = transform->GetWorldMatrix();

		_transformInfo._world = worldTM;
		_transformInfo._worldInvTranspose = (worldTM.Invert()).Transpose();

		// 1 - 2. Material Information Update
		// TODO : ���� ������Ʈ ���� �����ϸ鼭 Material�� ������ �ִ� ��쿡�� ��ȯ�ϵ��� ����
		_renderObjectInfo._materials = &_primitiveMaterials;

		// 1 - 3. Bone Matrices pallet Update.
		DUOLGameEngine::GameObject* rootObject = GetGameObject()->GetTransform()->GetRootTransform()->GetGameObject();

		DUOLGameEngine::Animator* animator = rootObject->GetComponent<Animator>();

		// TODO : �ִϸ��̼��� �ְ� �ִϸ����Ͱ� ��� ���϶� ..
		if (animator != nullptr)
		{
			_skinnedMeshInfo.SetBoneTransforms(animator->GetBoneMatrices());
		}

		// 1 - 4. Root Object ID
		_skinnedMeshInfo.SetObjectID(rootObject->GetUUID());

		// _skinnedMeshInfo.SetObjectID(DUOLMath::Vector2(rootObject->GetUUID(), 1.f));

		// 2. Render Object�� ������ �����ϴ�.
		GraphicsManager::GetInstance()->ReserveRenderObject(&_renderObjectInfo);
	}
}