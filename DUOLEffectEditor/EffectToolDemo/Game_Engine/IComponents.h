#pragma once

/// <summary>
/// 구조를 단순하게 만들어보자
/// 엔진을 만들자!
/// 2022-03-29 신성현
/// </summary>

namespace Muscle
{

	class GameObject;
	class Collider;

	class IComponents : public MonoBehavior
	{
	protected:
		IComponents(std::shared_ptr<GameObject> _GameObj); //부모를 게임 오브젝트를 인자로 받고 초기화

		virtual ~IComponents();

	protected:
		std::weak_ptr<GameObject> m_GameObject; // 이 컴포넌트가 달려있는 오브젝트.

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