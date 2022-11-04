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

		float m_Distance; // �� A�� ��B�� �Ÿ�.. 
		// �Ÿ��� ȸ�� ������ A�� B�� ���� ���̴�.

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

		virtual void Render() {}; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����

		virtual void UpdateMinMax() override;

		float DistLinetoPoint(Vector3 point); // �Ǹ������� �������� �Ÿ�
	};

}
