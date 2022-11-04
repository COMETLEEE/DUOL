#pragma once
namespace Muscle
{
	class OBBCollider : public BoxCollider, public std::enable_shared_from_this<OBBCollider>
	{

	public:
		OBBCollider(std::shared_ptr<GameObject> _GameObject);

		~OBBCollider();

	public:
		virtual void Finalize() override;

	public:
		virtual void ColliderCheck(std::shared_ptr<Collider> _Other) override;

		virtual void Start() {};

		virtual void Render() {}; // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더

		virtual void UpdateMinMax() override;

	};
}