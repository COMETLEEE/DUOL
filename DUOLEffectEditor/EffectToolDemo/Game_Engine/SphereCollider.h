#pragma once

namespace Muscle
{
	class SphereCollider : public Collider, public std::enable_shared_from_this<SphereCollider>
	{
	public:
		SphereCollider(std::shared_ptr<GameObject> _GameObject);
		~SphereCollider();
	private:
		float m_Radius;

	public:
		const float GetRadius(); // 피벗을 더한 월드 포지션 값

		virtual void Finalize() override;

	public:
		virtual void ColliderCheck(std::shared_ptr<Collider> _Other);

		virtual void Start() {};

		virtual void Render() {}; // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더

		virtual void UpdateMinMax() override;

		void SetRadius(float _Radius);
	};

}