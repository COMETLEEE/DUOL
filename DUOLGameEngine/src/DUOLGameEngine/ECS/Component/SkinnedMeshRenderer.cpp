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
		// 1. Bone Matrix�� ��°�� ������Ʈ �˴ϴ�.

		// 2. ��� ����� �°� Bone Object���� Transform�� ���������ݴϴ�.


		// 3. RenderObject ���� ä���� ������ �׸��� !
	}
}