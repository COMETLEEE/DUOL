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
		virtual void Render(); // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����
		virtual void UpdateMinMax() override;
	};


}