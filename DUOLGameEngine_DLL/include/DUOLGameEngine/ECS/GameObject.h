#pragma once
#include <algorithm>
#include <vector>

#include "DUOLGameEngine/ECS/ObjectBase.h"

// For define template 'Add / Get' Component functions.
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

#include <rttr/registration_friend>

namespace DUOLEditor
{
	class Hierarchy;
	class Inspector;
}

namespace DUOLPhysics
{
	class PhysicsActorBase;
}

namespace DUOLGameEngine
{
	class ComponentBase;

	class BehaviourBase;

	class MonoBehaviourBase;

	class Transform;

	class Scene;
}

namespace DUOLGameEngine
{
	/**
	 * \brief 컴포넌트가 삭제될 때 필요한 정보들의 모임
	 */
	struct ComponentDestroyInfo
	{
		std::shared_ptr<DUOLGameEngine::ComponentBase> _targetComponent;

		float _lastTime;

		// TODO : 기다리는 시간동안 어느 리스트에 있는지 (abled, disabled, ..)
		// TODO : 갱신되었을 수도 있으니 삭제될 때 찾아서 맞는 조건대로 호출하고 삭제한다.
		DUOLCommon::Event<void> _destroyedEvent;
	};

	/**
	 * \brief 게임 내의 엔티티를 나타내는 클래스입니다.
	 */
	class DUOL_GAMEENGINE_API GameObject final : public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<GameObject>
	{
	public:
		/**
		 * \brief 기본 생성자입니다.
		 */
		GameObject();

		/**
		 * \brief 게임 오브젝트 객체를 만듭니다. 다만, 만약 해당 원시 포인터에 대해서 shared_ptr 객체를
		 * 만들지 않고 shared_from_this를 호출하면 Exception이 발생합니다.
		 * 따라서, 정석은 생성자를 private로 막고, shared_ptr을 반환하는 CreateEmpty() 등의 함수를 열어두어야 합니다.
		 * \param name 해당 게임 오브젝트의 이름입니다.
		 */
		GameObject(const DUOLCommon::tstring& name);

		virtual ~GameObject() override;

		DEFINE_DEFAULT_COPY_MOVE(GameObject)

	private:
		/**
		 * \brief 게임 오브젝트를 해제합니다.
		 */
		void UnInitialize();

#pragma region MANAGING_COMPONENTS
	private:
		/**
		 * \brief 해당 게임 오브젝트의 Transform Component (자주 접근해서 사용되니 Caching)
		 */
		DUOLGameEngine::Transform* _transform;

		std::list<DUOLGameEngine::ComponentBase*> _allComponents;

		/**
		 * \brief ComponentBase Class를 상속받은 객체들의 리스트
		 */
		std::list<std::shared_ptr<DUOLGameEngine::ComponentBase>> _components;

		/**
		 * \brief BehaviourBase Class를 상속받았고, 활성화 상태인 객체들의 리스트
		 */
		std::list<std::shared_ptr<DUOLGameEngine::BehaviourBase>> _abledBehaviours;

		/** ,
		 * \brief BehaviourBase Class를 상속받았고, 비활성화 상태인 객체들의 리스트
		 */
		std::list<std::shared_ptr<DUOLGameEngine::BehaviourBase>> _disabledBehaviours;

		/**
		 * \brief MonoBehavioursBase Class를 상속받았고, 활성화 상태인 객체들의 리스트
		 */
		std::list<std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>> _abledMonoBehaviours;

		/**
		 * \brief MonoBehaviourBase Class를 상속받았고, 비활성화 상태인 객체들의 리스트
		 */
		std::list<std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>> _disabledMonoBehaviours;

		/**
		 * \brief 삭제 예정 컴포넌트들에 대한 정보의 리스트. 매 프레임별로 순회하면서 마지막 컨텍스트에서 갱신한다.
		 */
		std::list<ComponentDestroyInfo> _componentsForDestroy;

	public:
		inline Transform* GetTransform() const { return _transform; }

		template <typename TComponent>
		TComponent* GetComponent() const;

		template <typename TComponent>
		std::vector<TComponent*> GetComponents() const;

		template <typename TComponent>
		TComponent* AddComponent();

		const std::list<DUOLGameEngine::ComponentBase*>& GetAllComponents() const;

	private:
		template <typename TComponent, typename TComponentBase>
		TComponent* SuchComponent() const;

		template <typename TComponent, typename TComponentBase>
		std::vector<TComponent*> SuchComponents() const;

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

		/**
		 * \brief MonoBehaviour가 비활성화될 때 호출됩니다. Loop를 정지합니다.
		 * \param target 활성화되는 MonoBehaviour
		 */
		void SetMonoBehaviourEnabled(const std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>& target);

		/**
		 * \brief MonoBehaviour가 비활성화될 때 호출됩니다. Loop를 정지합니다.
		 * \param target 비활성화되는 MonoBehaviour
		 */
		void SetMonoBehaviourDisabled(const std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>& target);

	public:
		DUOLGameEngine::ComponentBase* AddComponent(const DUOLCommon::tstring& componentName);

		DUOLGameEngine::ComponentBase* GetComponent(const DUOLCommon::tstring& componentName);
#pragma endregion

	public:
		/**
		 * \brief 게임 오브젝트가 씬이 활성화된 상태에서 생성될 때 호출됩니다.
		 * (기존 씬 내에 배치된 게임 오브젝트가 로드될 때는 호출되지 않습니다.)
		 */
		virtual void OnCreate();

		/**
		* \brief 씬이 시작할 때 OnStart 보다 이전에 호출됩니다. 비활성화 게임 오브젝트도 실행합니다.
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
		 * \brief 게임 오브젝트가 파괴될 때 호출됩니다.
		 */
		virtual void OnDestroy();

		/**
		 * \brief 매 프레임에 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnUpdate(float deltaTime);

		/**
		 * \brief 만약, 해당 게임 오브젝트의 MonoBehaviourBase를 상속한 스크립트에서 이전에 yield 했지만 이제 Resume 할
		 * Coroutine이 있다면 해당 함수에서 실시합니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnCoroutineUpdate(float deltaTime);

		/**
		 * \brief 만약, 해당 게임 오브젝트의 MonoBehasviourBase를 상속한 스크립트에서 Invok 호출을 했다면
		 * 해당 함수에서 시간을 체크하고 실행합니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnInvokeUpdate(float deltaTime);

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

		/**
		 * \brief OnLateUpdate() 에서 마지막으로 삭제 요청된 
		 */
		void UpdateDestroyComponent(float deltaTime);

		/**
		 * \brief 삭제 시점에 요청된 컴포넌트가 어디에 위치되어 있는지 확인하고, 해당하는 이벤트 함수를 호출하고 제거합니다.
		 */
		void SearchAndDestroyComponent(std::shared_ptr<DUOLGameEngine::ComponentBase> targetComponent);

	private:
		/**
		 * \brief 해당 게임 오브젝트가 속해있는 씬입니다.
		 */
		DUOLGameEngine::Scene* _scene;

		/**
		 * \brief 해당 게임 오브젝트를 대표하는 물리 액터입니다. (물리 관련 오브젝트가 아니라면 null입니다.)
		 */
		std::weak_ptr<DUOLPhysics::PhysicsActorBase> _physicsActor;

		/**
		 * \brief 게임 오브젝트의 태그입니다. Tag와 Layer Manager에서 셋팅 후 사용합니다.
		 */
		DUOLCommon::tstring _tag;

		/**
		 * \brief 게임 오브젝트의 레이어입니다. 레이어는 선택적인 렌더링 및 레이 캐스트에 사용할 수 있습니다.
		 */
		int _layer;

		/**
		 * \brief 게임 오브젝트가 활성화 상태인지 나타냅니다.
		 */
		bool _isActive;

		/**
		 * \brief 게임 오브젝트의 OnStart가 호출되었습니까.
		 */
		bool _isStarted;

	public:
		Scene* GetScene() const;

		inline const DUOLCommon::tstring& GetTag() const { return _tag; }

		inline void SetTag(const DUOLCommon::tstring& tag) { _tag = tag; }

		inline int GetLayer() const { return _layer; }

		inline void SetLayer(int layer) { _layer = layer; }

		inline bool GetIsActive() const { return _isActive; }

		void SetIsActive(bool value);

	private:
		void RegisterDestroyComponent(DUOLGameEngine::ComponentBase* component, float time);

	private:
		DUOLCommon::Event<void> _destroyEventHandlers;

		DUOLCommon::Event<void> _componentCountChangedEvent;

		RTTR_REGISTRATION_FRIEND

		RTTR_ENABLE(ObjectBase)

#pragma region FRIEND_CLASS
		friend class ObjectBase;

		friend class ComponentBase;

		friend class BehaviourBase;

		friend class MonoBehaviourBase;

		friend class Scene;

		friend class PhysicsManager;

		friend class DUOLEditor::Hierarchy;

		friend class DUOLEditor::Inspector;
#pragma endregion
	};

#pragma region GET_ADD_COMPONENT
	template <typename TComponent>
	TComponent* GameObject::GetComponent() const
	{
		static_assert(std::is_base_of<ComponentBase, TComponent>::value,
			"TComponent must inherit from ComponentBase");

		if constexpr (std::is_base_of_v<MonoBehaviourBase, TComponent>)
		{
			return SuchComponent<TComponent, MonoBehaviourBase>();
		}
		else if constexpr (std::is_base_of_v<BehaviourBase, TComponent>)
		{
			return SuchComponent<TComponent, BehaviourBase>();
		}
		else if constexpr (std::is_base_of_v<ComponentBase, TComponent>)
		{
			return SuchComponent<TComponent, ComponentBase>();
		}

		return nullptr;
	}

	template <typename TComponent>
	std::vector<TComponent*> GameObject::GetComponents() const
	{
		static_assert(std::is_base_of_v<ComponentBase, TComponent>,
			"TComponent must inherit from ComponentBase");

		if constexpr (std::is_base_of_v<MonoBehaviourBase, TComponent>)
		{
			return SuchComponents<TComponent, MonoBehaviourBase>();
		}
		else if constexpr (std::is_base_of_v<BehaviourBase, TComponent>)
		{
			return SuchComponents<TComponent, BehaviourBase>();
		}
		else if constexpr (std::is_base_of_v<ComponentBase, TComponent>)
		{
			return SuchComponents<TComponent, ComponentBase>();
		}
		else
			return std::vector<TComponent*>();
	}

	// Dynamic pointer cast 코드를 여기에 다 몰아넣어 놓습니다. 나중에 바꾸자 .. (Enum ..? Reflection ..? 자동화냐 성능이냐 ..)
	template <typename TComponent, typename TComponentBase>
	TComponent* GameObject::SuchComponent() const
	{
		if constexpr (std::is_same_v<TComponentBase, MonoBehaviourBase>)
		{
			for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
			{
				std::shared_ptr<TComponent> dcMonoBehaviour = std::dynamic_pointer_cast<TComponent>(abledMonoBehaviour);

				if (dcMonoBehaviour != nullptr)
					return dcMonoBehaviour.get();
			}

			for (const auto& disabledMonoBehaviour : _disabledMonoBehaviours)
			{
				std::shared_ptr<TComponent> dcMonoBehaviour = std::dynamic_pointer_cast<TComponent>(disabledMonoBehaviour);

				if (dcMonoBehaviour != nullptr)
					return dcMonoBehaviour.get();
			}
		}
		else if constexpr (std::is_same_v<TComponentBase, BehaviourBase>)
		{
			for (const auto& abledBehaviour : _abledBehaviours)
			{
				std::shared_ptr<TComponent> dcBehaviour = std::dynamic_pointer_cast<TComponent>(abledBehaviour);

				if (dcBehaviour != nullptr)
					return dcBehaviour.get();
			}

			for (const auto& disabledBehaviour : _disabledBehaviours)
			{
				std::shared_ptr<TComponent> dcBehaviour = std::dynamic_pointer_cast<TComponent>(disabledBehaviour);

				if (dcBehaviour != nullptr)
					return dcBehaviour.get();
			}
		}
		else if constexpr (std::is_same_v<TComponentBase, ComponentBase>)
		{
			if constexpr (std::is_same_v<Transform, TComponent>)
				return _transform;

			for (const auto& component : _components)
			{
				std::shared_ptr<TComponent> dcComponent = std::dynamic_pointer_cast<TComponent>(component);

				if (dcComponent != nullptr)
					return dcComponent.get();
			}
		}

		// 해당하는 컴포넌트가 없습니다.
		return nullptr;
	}

	template <typename TComponent, typename TComponentBase>
	std::vector<TComponent*> GameObject::SuchComponents() const
	{
		std::vector<TComponent*> retVec;

		if constexpr (std::is_same_v<TComponentBase, MonoBehaviourBase>)
		{
			for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
			{
				std::shared_ptr<TComponent> dcMonoBehaviour = std::dynamic_pointer_cast<TComponent>(abledMonoBehaviour);

				if (dcMonoBehaviour != nullptr)
					retVec.push_back(dcMonoBehaviour.get());
			}

			for (const auto& disabledMonoBehaviour : _disabledMonoBehaviours)
			{
				std::shared_ptr<TComponent> dcMonoBehaviour = std::dynamic_pointer_cast<TComponent>(disabledMonoBehaviour);

				if (dcMonoBehaviour != nullptr)
					retVec.push_back(dcMonoBehaviour.get());
			}
		}
		else if constexpr (std::is_same_v<TComponentBase, BehaviourBase>)
		{
			for (const auto& abledBehaviour : _abledBehaviours)
			{
				std::shared_ptr<TComponent> dcBehaviour = std::dynamic_pointer_cast<TComponent>(abledBehaviour);

				if (dcBehaviour != nullptr)
					retVec.push_back(dcBehaviour.get());
			}

			for (const auto& disabledBehaviour : _disabledBehaviours)
			{
				std::shared_ptr<TComponent> dcBehaviour = std::dynamic_pointer_cast<TComponent>(disabledBehaviour);

				if (dcBehaviour != nullptr)
					retVec.push_back(dcBehaviour.get());
			}
		}
		else if constexpr (std::is_same_v<TComponentBase, ComponentBase>)
		{
			if constexpr (std::is_same_v<Transform, TComponent>)
				return _transform;

			for (const auto& component : _components)
			{
				std::shared_ptr<TComponent> dcComponent = std::dynamic_pointer_cast<TComponent>(component);

				if (dcComponent != nullptr)
					retVec.push_back(dcComponent.get());
			}
		}

		return retVec;
	}

	// 컴포넌트는 해당 함수를 통해서만 객체화된다.
	template <typename TComponent>
	TComponent* GameObject::AddComponent()
	{
		static_assert(std::is_base_of_v<ComponentBase, TComponent>,
			"TComponent must inherit from ComponentBase");

		TComponent* primitiveCom = new TComponent(this);

		// 모든 컴포넌트 기록에 넣어둡니다.
		_allComponents.push_back(primitiveCom);

		// TODO
		// Resource (Memory) 관리를 위한 Deleter를 매개변수로 넣어줄 수 있다.
		std::shared_ptr<TComponent> component(primitiveCom);

		// 컴포넌트 카운트 채인지드 이벤트 !
		_componentCountChangedEvent.Invoke();

		if constexpr (std::is_base_of_v<MonoBehaviourBase, TComponent>)
		{
			_abledMonoBehaviours.push_back(component);

			return primitiveCom;
		}
		else if constexpr (std::is_base_of_v<BehaviourBase, TComponent>)
		{
			_abledBehaviours.push_back(component);

			return primitiveCom;
		}
		else
		{
			if constexpr (std::is_same_v<Transform, TComponent>)
			{
				// 트랜스폼은 따로 캐싱한다.
				_transform = component.get();
			}

			_components.push_back(component);

			return primitiveCom;
		}
	}
#pragma endregion
}