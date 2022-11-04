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
		const float GetRadius(); // �ǹ��� ���� ���� ������ ��

		virtual void Finalize() override;

	public:
		virtual void ColliderCheck(std::shared_ptr<Collider> _Other);

		virtual void Start() {};

		virtual void Render() {}; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����

		virtual void UpdateMinMax() override;

		void SetRadius(float _Radius);
	};

}