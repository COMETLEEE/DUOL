#pragma once
namespace Muscle
{

	class CapsuleCollider : public Collider, public std::enable_shared_from_this<CapsuleCollider>
	{

	public:
		CapsuleCollider(std::shared_ptr<GameObject> _GameObject);
		~CapsuleCollider();
	private:
		float m_Radius;

		float m_Distance; // 점 A와 점B의 거리.. 
		// 거리와 회전 값으로 A와 B를 구할 것이다.

	public:
		void SetCapsule(float _Radius, float _Distnace);

		const float GetRadius();

		const float GetDistance();

		const Vector3 GetTopPosition();

		const Vector3 GetBottomPosition();

		const Vector3 GetWorldTopPosition();

		const Vector3 GetWorldBottomPosition();

		const Matrix GetWorldRotTM();
		
		virtual void Finalize() override;
	public:
		virtual void ColliderCheck(std::shared_ptr<Collider> _Other) override;

		virtual void Start() {};

		virtual void Render() {}; // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더

		virtual void UpdateMinMax() override;

		float DistLinetoPoint(Vector3 point); // 실린더에서 점까지의 거리
	};

}
