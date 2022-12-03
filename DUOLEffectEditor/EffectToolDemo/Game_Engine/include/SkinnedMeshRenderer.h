#pragma once

#include "RendererBase.h"

namespace Muscle
{
	struct Bone;

	class SkinnedMeshRenderer : public RendererBase
	{
	public:
		SkinnedMeshRenderer(std::shared_ptr<GameObject> _GameObject);
		~SkinnedMeshRenderer();

	public:
		virtual void Start();
		virtual void Update(); // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.
		virtual void LateUpdate(); // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.
		virtual void Render(); // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����	

	public:
		void Initialize(const DirectX::SimpleMath::Matrix& skinNodeTM, const std::vector<std::shared_ptr<Bone>>& boneList);

	private:
		std::shared_ptr<Transform> _transform;

		DirectX::SimpleMath::Matrix _skinNodeTM;

		std::vector<std::shared_ptr<Bone>> _boneList;

		std::vector<DirectX::SimpleMath::Matrix> _boneMatrixList;

		uint32 _boneCount;
	};
}