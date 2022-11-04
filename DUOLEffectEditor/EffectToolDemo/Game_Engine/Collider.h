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
		Vector3 m_LocalPosition;// �ش� ������Ʈ�� ���� ���������� ������, �ǹ�, ������ 000�� ������?

		UINT _ColliderID; // ���� Ž���� ����... �ݺ����� �ʹ� ���� ���δ�.

		static UINT idCount;

	protected:
		std::unordered_map<UINT, std::shared_ptr<Collider>> m_CurColliders; //���� �浹���� �ݶ��̴���.

		std::unordered_map<UINT, std::shared_ptr<Collider>> m_PrevColliders; //���� �����ӿ� �浹�ߴ� �ݶ��̴���.

		bool m_isCollided;

		bool _isColliderIsEnable; // �ݶ��̴��� ���� üũ����..!

	public:
		ColliderType m_ColliderType;

		bool GetIsCollided() { return m_isCollided; }

		Vector3 GetWorldPosition();// �ǹ��� ���� ���� ������ ��

		Vector3 GetLocalPosition();// �ǹ��� ���� ���� ������ ��

		void SetLocalPostion(Vector3 local); // �ǹ�!

		virtual void Finalize() override;

	protected:
		std::shared_ptr<Transform> m_Transform; // ���� ������״� ĳ���ؼ� ���

		float _minX, _maxX, _minZ, _maxZ; // �ݶ��̴����� ������Ʈ ����� �ٸ���..! ���� ������ �� ����� �ǹ���?

	protected:
		void EnterEvent(std::shared_ptr<Collider> _Other);

		void StayEvent(std::shared_ptr<Collider> _Other);

		void ExitEvent(std::shared_ptr<Collider> _Other);

	public:
		virtual void ColliderCheck(std::shared_ptr<Collider> _Other) abstract;

		virtual void Start() abstract;

		virtual void Update() override; // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.

		virtual void Render() abstract; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����

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