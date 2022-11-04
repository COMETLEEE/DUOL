#pragma once
namespace Muscle
{
	class AABBCollider : public BoxCollider, public std::enable_shared_from_this<AABBCollider>
	{
	public:
		AABBCollider(std::shared_ptr<GameObject> _GameObject);
		~AABBCollider();

	public:
		virtual void Finalize() override;

	public:
		virtual void ColliderCheck(std::shared_ptr<Collider> _Other);
		virtual void Start();
		virtual void Render(); // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더
		virtual void UpdateMinMax() override;
	};


}