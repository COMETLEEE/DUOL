#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"

namespace DUOLGameEngine
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(const std::weak_ptr<DUOLGameEngine::GameObject>& owner,
		const DUOLCommon::tstring& name) :
		RendererBase(owner, name)
	{
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
		// 1. Bone Matrix가 통째로 업데이트 됩니다.

		// 2. 계산 결과에 맞게 Bone Object들의 Transform을 변동시켜줍니다.


		// 3. RenderObject 정보 채워서 보내고 그린다 !
	}
}