#pragma once
namespace Muscle
{
	class Collider;

	class MonoBehavior abstract
	{
	public:
		MonoBehavior() = default;

		~MonoBehavior() = default;

	protected:
		virtual void OnEnterCollision(std::shared_ptr<Collider> _Other) {};

		virtual void OnStayCollision(std::shared_ptr<Collider> _Other) {};

		virtual void OnExitCollision(std::shared_ptr<Collider> _Other) {};

	protected:
		virtual void Start() {};

		virtual void Update() {}; // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.

		virtual void LateUpdate() {}; // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.

		virtual void Render() {}; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����
	};
}

