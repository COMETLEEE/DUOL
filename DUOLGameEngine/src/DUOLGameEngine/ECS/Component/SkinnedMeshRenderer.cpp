#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/ECS/Object/Mesh.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

namespace DUOLGameEngine
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(const std::weak_ptr<DUOLGameEngine::GameObject>& owner,
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
		if (_skinnedMesh == nullptr)
			return;

		// 1. 메쉬 넣어줍니다.
		_renderObjectInfo._mesh = _skinnedMesh->GetPrimitiveMesh();

		// 1 - 1. Transform Information Update
		const Matrix& worldTM = GetTransform()->GetWorldMatrix();

		_transformInfo._world = worldTM;
		_transformInfo._worldInvTranspose = (worldTM.Invert()).Transpose();

		// 1 - 2. Material Information Update
		// TODO : 추후 업데이트 구조 생각하면서 Material의 변동이 있는 경우에만 변환하도록 수정
		_renderObjectInfo._materials = &_primitiveMaterials;

		// 1 - 3. Bone Matrices pallet Update.
		DUOLGameEngine::GameObject* rootObject = GetGameObject()->GetTransform()->GetRootTransform()->GetGameObject();

		DUOLGameEngine::Animator* animator = rootObject->GetComponent<Animator>();

		if (animator != nullptr)
			_skinnedMeshInfo.SetBoneTransforms(animator->GetBoneMatrices());

		// 2. Render Object의 참조를 보냅니다.
		GraphicsManager::GetInstance()->ReserveRenderObject(_renderObjectInfo);
	}
}
