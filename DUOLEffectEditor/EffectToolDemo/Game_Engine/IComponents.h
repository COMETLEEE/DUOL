#pragma once

/// <summary>
/// ������ �ܼ��ϰ� ������
/// ������ ������!
/// 2022-03-29 �ż���
/// </summary>

namespace Muscle
{

	class GameObject;
	class Collider;

	class IComponents : public MonoBehavior
	{
	protected:
		IComponents(std::shared_ptr<GameObject> _GameObj); //�θ� ���� ������Ʈ�� ���ڷ� �ް� �ʱ�ȭ

		virtual ~IComponents();

	protected:
		std::weak_ptr<GameObject> m_GameObject; // �� ������Ʈ�� �޷��ִ� ������Ʈ.

	public:
		bool _isEnable;

	public:
		std::shared_ptr<GameObject> GetGameObject();

	public:
		template  <class  TComponent>
		std::shared_ptr<TComponent> AddComponent();

		template  <class  TComponent>
		std::shared_ptr<TComponent> GetComponent();

	public:
		bool GetIsEnable();

		inline void SetIsEnable(bool value) { _isEnable = value; }

		virtual void Finalize() {}


		friend class GameObject;
	};

	template  <class  TComponent>
	std::shared_ptr<TComponent>
		Muscle::IComponents::GetComponent()
	{
		return GetGameObject()->GetComponent<TComponent>();
	}

	template  <class  TComponent>
	std::shared_ptr<TComponent>
		Muscle::IComponents::AddComponent()
	{
		return GetGameObject()->AddComponent<TComponent>();
	}
}