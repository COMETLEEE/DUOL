#pragma once

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
		virtual void Update(); // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.
		virtual void LateUpdate(); // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.
		virtual void Render(); // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더	

	public:
		void Initialize(const Matrix& skinNodeTM, const std::vector<std::shared_ptr<Bone>>& boneList);

	private:
		std::shared_ptr<Transform> _transform;

		Matrix _skinNodeTM;

		std::vector<std::shared_ptr<Bone>> _boneList;

		std::vector<Matrix> _boneMatrixList;

		uint32 _boneCount;
	};
}