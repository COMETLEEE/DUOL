#pragma once
#include <algorithm>
#include <vector>

#include "DUOLGameEngine/ECS/Component/ObjectBase.h"

namespace DUOLGameEngine
{
	class ComponentBase;

	class BehaviourBase;

	class Transform;

	/**
	 * \brief 게임 내의 엔티티를 나타내는 클래스입니다.
	 */
	class GameObject : public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<GameObject>
	{
	public:
		/**
		 * \brief 게임 오브젝트 객체를 만듭니다. 다만, 만약 해당 원시 포인터에 대해서 shared_ptr 객체를
		 * 만들지 않고 shared_from_this를 호출하면 Exception이 발생합니다.
		 * 따라서, 정석은 생성자를 private로 막고, shared_ptr을 반환하는 CreateEmptry() 등의 함수를 열어두어야 합니다.
		 * \param name 해당 게임 오브젝트의 이름입니다.
		 */
		GameObject(const tstring& name);

		virtual ~GameObject() override;

		DEFINE_DEFAULT_COPY_MOVE(GameObject)

	private:
		std::weak_ptr<DUOLGameEngine::GameObject> _parent;

		std::vector<std::shared_ptr<DUOLGameEngine::GameObject>> _childrens;

		std::shared_ptr<DUOLGameEngine::Transform> _transform;

		std::vector<std::shared_ptr<DUOLGameEngine::ComponentBase>> _components;

		std::vector<std::shared_ptr<DUOLGameEngine::BehaviourBase>> _disabledBehaviours;

	public:
		inline std::shared_ptr<DUOLGameEngine::Transform> GetTransform() const { return _transform; }

		template <typename TComponent>
		std::shared_ptr<TComponent> GetComponent() const;

		template <typename TComponent>
		std::shared_ptr<TComponent> AddComponent();

		inline std::shared_ptr<DUOLGameEngine::GameObject> GetParent() const { return _parent.lock(); }

		inline const std::vector<std::shared_ptr<DUOLGameEngine::GameObject>>& GetChildrens() const { return _childrens; }

		void SetChildren(const std::shared_ptr<DUOLGameEngine::GameObject>& children);

	private:
		void SetParent(const std::shared_ptr<DUOLGameEngine::GameObject>& parent);

		/**
		 * \brief 타겟 자식 게임 오브젝트에 대해서 계층 관계를 정리합니다.
		 * \param target 정리의 대상이 되는 자식 게임 오브젝트입니다.
		 */
		void ResetHierarchy(const std::shared_ptr<DUOLGameEngine::GameObject>& target);

	private:
		/**
		 * \brief Behaviour를 활성화될 때 호출됩니다. Loop를 시작합니다.
		 * \param target 활성화되는 Behaviour
		 */
		void SetBehaviourEnabled(const std::shared_ptr<DUOLGameEngine::BehaviourBase>& target);

		/**
		 * \brief Behaviour가 비활성화될 때 호출됩니다. Loop를 정지합니다.
		 * \param target 비활성화되는 Behaviour
		 */
		void SetBehaviourDisabled(const std::shared_ptr<DUOLGameEngine::BehaviourBase>& target);

	public:
		/**
		* \brief 씬이 시작할 때 OnStart 보다 이전에 호출됩니다.
		* 씬 시작 시 우선 순위의 게임 로직을 적용할 수 있습니다.
		*/
		virtual void OnAwake();

		/**
		 * \brief 씬이 시작할 때 OnAwake 이후에 호출됩니다.
		 * 씬 시작 시 우선 순위의 게임 로직을 적용할 수 있습니다.
		 */
		virtual void OnStart();

		/**
		 * \brief 게임 오브젝트가 활성화될 때 호출됩니다.
		 */
		virtual void OnActive();

		/**
		 * \brief 게임 오브젝트가 비활성화될 때 호출됩니다.
		 */
		virtual void OnInActive();

		/**
		 * \brief 컴포넌트가 파괴될 때 호출됩니다.
		 */
		virtual void OnDestroy();

		/**
		 * \brief 매 프레임에 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnUpdate(float deltaTime);

		/**
		 * \brief 매 물리 프레임에 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnFixedUpdate(float deltaTime);

		/**
		 * \brief OnUpdate 이후, 매 프레임 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnLateUpdate(float deltaTime);

	private:
		/**
		 * \brief 게임 오브젝트의 태그입니다. Tag와 Layer Manager에서 셋팅 후 사용합니다.
		 */
		tstring _tag;

		/**
		 * \brief 게임 오브젝트의 레이어입니다. 레이어는 선택적인 렌더링 및 레이 캐스트에 사용할 수 있습니다.
		 */
		int _layer;

		/**
		 * \brief 게임 오브젝트가 활성화 상태인지 나타냅니다.
		 */
		bool _isActive;

	public:
		inline const tstring& GetTag() const { return _tag; }

		inline void SetTag(const tstring& tag) { _tag = tag; }

		inline int GetLayer() const { return _layer; }

		inline void SetLayer(int layer) { _layer = layer; }

		inline bool GetIsActive() const { return _isActive; }

		inline void SetIsActive(bool value) { _isActive = value; }

		friend class ComponentBase;

		friend class BehaviourBase;
	};

	template <typename TComponent>
	std::shared_ptr<TComponent> GameObject::GetComponent() const
	{
		static_assert(std::is_base_of<ComponentBase, TComponent>::value,
			"TComponent must inherit from ComponentBase");

		if constexpr (std::is_same<TComponent, Transform>::value)
			return _transform;

		// TODO
		// 아무래도 자동화와 효율성을 동시에 추구하기 위해서는 .. 리플렉션 기능이 필요할 것 같습니다.
#pragma region USING_DYNAMIC_CAST
		auto&& dcFunc = [](const std::shared_ptr<ComponentBase>& component)
			{
				std::shared_ptr<TComponent> dcComponent = std::dynamic_pointer_cast<TComponent>(component);

				if (dcComponent != nullptr)
					return dcComponent;
			};

		std::for_each(_components.begin(), _components.end(), dcFunc);

		std::for_each(_disabledBehaviours.begin(), _disabledBehaviours.end(), dcFunc);

		return nullptr;
#pragma endregion
	}

	template <typename TComponent>
	std::shared_ptr<TComponent> GameObject::AddComponent()
	{
		static_assert(std::is_base_of<ComponentBase, TComponent>::value,
			"TComponent must inherit from ComponentBase");

		TComponent* primitiveCom = new TComponent(this->shared_from_this());

		// TODO
		// Resource (Memory) 관리를 위한 Deleter를 매개변수로 넣어줄 수 있다.
		std::shared_ptr<TComponent> component(primitiveCom);

		_components.push_back(component);

		return primitiveCom;
	}
}