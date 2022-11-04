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

		virtual void Render() {}; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����

		virtual void UpdateMinMax() override;

	};
}