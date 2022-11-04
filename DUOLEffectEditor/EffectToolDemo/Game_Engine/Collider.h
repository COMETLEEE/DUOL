#pragma once

namespace Muscle
{

	enum class ColliderType
	{
		Sphere,
		AABB,
		OBB,
		Capsule
	};
	class CapsuleCollider;
	class AABBCollider;
	class OBBCollider;
	class SphereCollider;
	class BoxCollider;

	class Collider : public IComponents
	{
	public:
		Collider(std::shared_ptr<GameObject> _GameObject);
		virtual ~Collider();
	private:
		Vector3 m_LocalPosition;// 해당 오브젝트의 로컬 공간에서의 포지션, 피벗, 보통은 000을 넣을듯?

		UINT _ColliderID; // 빠른 탐색을 위한... 반복문에 너무 많이 쓰인다.

		static UINT idCount;

	protected:
		std::unordered_map<UINT, std::shared_ptr<Collider>> m_CurColliders; //현재 충돌중인 콜라이더들.

		std::unordered_map<UINT, std::shared_ptr<Collider>> m_PrevColliders; //이전 프레임에 충돌했던 콜라이더들.

		bool m_isCollided;

		bool _isColliderIsEnable; // 콜라이더는 따로 체크하자..!

	public:
		ColliderType m_ColliderType;

		bool GetIsCollided() { return m_isCollided; }

		Vector3 GetWorldPosition();// 피벗을 더한 월드 포지션 값

		Vector3 GetLocalPosition();// 피벗을 더한 월드 포지션 값

		void SetLocalPostion(Vector3 local); // 피벗!

		virtual void Finalize() override;

	protected:
		std::shared_ptr<Transform> m_Transform; // 많이 사용할테니 캐싱해서 사용

		float _minX, _maxX, _minZ, _maxZ; // 콜라이더마다 업데이트 방법이 다르다..! 공간 분할할 때 사용할 피벗값?

	protected:
		void EnterEvent(std::shared_ptr<Collider> _Other);

		void StayEvent(std::shared_ptr<Collider> _Other);

		void ExitEvent(std::shared_ptr<Collider> _Other);

	public:
		virtual void ColliderCheck(std::shared_ptr<Collider> _Other) abstract;

		virtual void Start() abstract;

		virtual void Update() override; // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.

		virtual void Render() abstract; // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더

		virtual void UpdateMinMax() abstract;

	protected:

		static void CapsuleToSphere(std::shared_ptr<CapsuleCollider> capsule, std::shared_ptr<SphereCollider> sphere);

		static void AABBToAABB(std::shared_ptr<AABBCollider> aabb1, std::shared_ptr<AABBCollider> aabb2);

		static void SphereToSphere(std::shared_ptr<SphereCollider> sphere1, std::shared_ptr<SphereCollider> sphere2);

		static void CapsuleToCapsule(std::shared_ptr<CapsuleCollider> capsule1, std::shared_ptr<CapsuleCollider> capsule2);

		static void SphereToBox(std::shared_ptr<SphereCollider> sphere, std::shared_ptr<BoxCollider> box);

		static void CapsuleToBox(std::shared_ptr<CapsuleCollider> capsule, std::shared_ptr<BoxCollider> box);

		static float PointToBoxDistance(Vector3 pointPos, std::shared_ptr<BoxCollider> box);

	public:
		friend class ObjectManager;

		friend class QuadTree;
	};


}