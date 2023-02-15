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
	 * \brief ������Ʈ�� ������ �� �ʿ��� �������� ����
	 */
	struct ComponentDestroyInfo
	{
		std::shared_ptr<DUOLGameEngine::ComponentBase> _targetComponent;

		float _lastTime;

		// TODO : ��ٸ��� �ð����� ��� ����Ʈ�� �ִ��� (abled, disabled, ..)
		// TODO : ���ŵǾ��� ���� ������ ������ �� ã�Ƽ� �´� ���Ǵ�� ȣ���ϰ� �����Ѵ�.
		DUOLCommon::Event<void> _destroyedEvent;
	};

	/**
	 * \brief ���� ���� ��ƼƼ�� ��Ÿ���� Ŭ�����Դϴ�.
	 */
	class DUOL_GAMEENGINE_API GameObject final : public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<GameObject>
	{
	public:
		/**
		 * \brief �⺻ �������Դϴ�.
		 */
		GameObject();

		/**
		 * \brief ���� ������Ʈ ��ü�� ����ϴ�. �ٸ�, ���� �ش� ���� �����Ϳ� ���ؼ� shared_ptr ��ü��
		 * ������ �ʰ� shared_from_this�� ȣ���ϸ� Exception�� �߻��մϴ�.
		 * ����, ������ �����ڸ� private�� ����, shared_ptr�� ��ȯ�ϴ� CreateEmpty() ���� �Լ��� ����ξ�� �մϴ�.
		 * \param name �ش� ���� ������Ʈ�� �̸��Դϴ�.
		 */
		GameObject(const DUOLCommon::tstring& name);

		virtual ~GameObject() override;

		DEFINE_DEFAULT_COPY_MOVE(GameObject)

	private:
		/**
		 * \brief ���� ������Ʈ�� �����մϴ�.
		 */
		void UnInitialize();

#pragma region MANAGING_COMPONENTS
	private:
		/**
		 * \brief �ش� ���� ������Ʈ�� Transform Component (���� �����ؼ� ���Ǵ� Caching)
		 */
		DUOLGameEngine::Transform* _transform;

		std::list<DUOLGameEngine::ComponentBase*> _allComponents;

		/**
		 * \brief ComponentBase Class�� ��ӹ��� ��ü���� ����Ʈ
		 */
		std::list<std::shared_ptr<DUOLGameEngine::ComponentBase>> _components;

		/**
		 * \brief BehaviourBase Class�� ��ӹ޾Ұ�, Ȱ��ȭ ������ ��ü���� ����Ʈ
		 */
		std::list<std::shared_ptr<DUOLGameEngine::BehaviourBase>> _abledBehaviours;

		/** ,
		 * \brief BehaviourBase Class�� ��ӹ޾Ұ�, ��Ȱ��ȭ ������ ��ü���� ����Ʈ
		 */
		std::list<std::shared_ptr<DUOLGameEngine::BehaviourBase>> _disabledBehaviours;

		/**
		 * \brief MonoBehavioursBase Class�� ��ӹ޾Ұ�, Ȱ��ȭ ������ ��ü���� ����Ʈ
		 */
		std::list<std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>> _abledMonoBehaviours;

		/**
		 * \brief MonoBehaviourBase Class�� ��ӹ޾Ұ�, ��Ȱ��ȭ ������ ��ü���� ����Ʈ
		 */
		std::list<std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>> _disabledMonoBehaviours;

		/**
		 * \brief ���� ���� ������Ʈ�鿡 ���� ������ ����Ʈ. �� �����Ӻ��� ��ȸ�ϸ鼭 ������ ���ؽ�Ʈ���� �����Ѵ�.
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
		 * \brief Behaviour�� Ȱ��ȭ�� �� ȣ��˴ϴ�. Loop�� �����մϴ�.
		 * \param target Ȱ��ȭ�Ǵ� Behaviour
		 */
		void SetBehaviourEnabled(const std::shared_ptr<DUOLGameEngine::BehaviourBase>& target);
		
		/**
		 * \brief Behaviour�� ��Ȱ��ȭ�� �� ȣ��˴ϴ�. Loop�� �����մϴ�.
		 * \param target ��Ȱ��ȭ�Ǵ� Behaviour
		 */
		void SetBehaviourDisabled(const std::shared_ptr<DUOLGameEngine::BehaviourBase>& target);

		/**
		 * \brief MonoBehaviour�� ��Ȱ��ȭ�� �� ȣ��˴ϴ�. Loop�� �����մϴ�.
		 * \param target Ȱ��ȭ�Ǵ� MonoBehaviour
		 */
		void SetMonoBehaviourEnabled(const std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>& target);

		/**
		 * \brief MonoBehaviour�� ��Ȱ��ȭ�� �� ȣ��˴ϴ�. Loop�� �����մϴ�.
		 * \param target ��Ȱ��ȭ�Ǵ� MonoBehaviour
		 */
		void SetMonoBehaviourDisabled(const std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>& target);

	public:
		DUOLGameEngine::ComponentBase* AddComponent(const DUOLCommon::tstring& componentName);

		DUOLGameEngine::ComponentBase* GetComponent(const DUOLCommon::tstring& componentName);
#pragma endregion

	public:
		/**
		 * \brief ���� ������Ʈ�� ���� Ȱ��ȭ�� ���¿��� ������ �� ȣ��˴ϴ�.
		 * (���� �� ���� ��ġ�� ���� ������Ʈ�� �ε�� ���� ȣ����� �ʽ��ϴ�.)
		 */
		virtual void OnCreate();

		/**
		* \brief ���� ������ �� OnStart ���� ������ ȣ��˴ϴ�. ��Ȱ��ȭ ���� ������Ʈ�� �����մϴ�.
		* �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
		*/
		virtual void OnAwake();

		/**
		 * \brief ���� ������ �� OnAwake ���Ŀ� ȣ��˴ϴ�.
		 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
		 */
		virtual void OnStart();

		/**
		 * \brief ���� ������Ʈ�� Ȱ��ȭ�� �� ȣ��˴ϴ�.
		 */
		virtual void OnActive();

		/**
		 * \brief ���� ������Ʈ�� ��Ȱ��ȭ�� �� ȣ��˴ϴ�.
		 */
		virtual void OnInActive();

		/**
		 * \brief ���� ������Ʈ�� �ı��� �� ȣ��˴ϴ�.
		 */
		virtual void OnDestroy();

		/**
		 * \brief �� �����ӿ� ȣ��˴ϴ�.
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
		 */
		virtual void OnUpdate(float deltaTime);

		/**
		 * \brief ����, �ش� ���� ������Ʈ�� MonoBehaviourBase�� ����� ��ũ��Ʈ���� ������ yield ������ ���� Resume ��
		 * Coroutine�� �ִٸ� �ش� �Լ����� �ǽ��մϴ�.
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
		 */
		virtual void OnCoroutineUpdate(float deltaTime);

		/**
		 * \brief ����, �ش� ���� ������Ʈ�� MonoBehasviourBase�� ����� ��ũ��Ʈ���� Invok ȣ���� �ߴٸ�
		 * �ش� �Լ����� �ð��� üũ�ϰ� �����մϴ�.
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
		 */
		virtual void OnInvokeUpdate(float deltaTime);

		/**
		 * \brief �� ���� �����ӿ� ȣ��˴ϴ�.
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
		 */
		virtual void OnFixedUpdate(float deltaTime);

		/**
		 * \brief OnUpdate ����, �� ������ ȣ��˴ϴ�.
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
		 */
		virtual void OnLateUpdate(float deltaTime);

		/**
		 * \brief OnLateUpdate() ���� ���������� ���� ��û�� 
		 */
		void UpdateDestroyComponent(float deltaTime);

		/**
		 * \brief ���� ������ ��û�� ������Ʈ�� ��� ��ġ�Ǿ� �ִ��� Ȯ���ϰ�, �ش��ϴ� �̺�Ʈ �Լ��� ȣ���ϰ� �����մϴ�.
		 */
		void SearchAndDestroyComponent(std::shared_ptr<DUOLGameEngine::ComponentBase> targetComponent);

	private:
		/**
		 * \brief �ش� ���� ������Ʈ�� �����ִ� ���Դϴ�.
		 */
		DUOLGameEngine::Scene* _scene;

		/**
		 * \brief �ش� ���� ������Ʈ�� ��ǥ�ϴ� ���� �����Դϴ�. (���� ���� ������Ʈ�� �ƴ϶�� null�Դϴ�.)
		 */
		std::weak_ptr<DUOLPhysics::PhysicsActorBase> _physicsActor;

		/**
		 * \brief ���� ������Ʈ�� �±��Դϴ�. Tag�� Layer Manager���� ���� �� ����մϴ�.
		 */
		DUOLCommon::tstring _tag;

		/**
		 * \brief ���� ������Ʈ�� ���̾��Դϴ�. ���̾�� �������� ������ �� ���� ĳ��Ʈ�� ����� �� �ֽ��ϴ�.
		 */
		int _layer;

		/**
		 * \brief ���� ������Ʈ�� Ȱ��ȭ �������� ��Ÿ���ϴ�.
		 */
		bool _isActive;

		/**
		 * \brief ���� ������Ʈ�� OnStart�� ȣ��Ǿ����ϱ�.
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

	// Dynamic pointer cast �ڵ带 ���⿡ �� ���Ƴ־� �����ϴ�. ���߿� �ٲ��� .. (Enum ..? Reflection ..? �ڵ�ȭ�� �����̳� ..)
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

		// �ش��ϴ� ������Ʈ�� �����ϴ�.
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

	// ������Ʈ�� �ش� �Լ��� ���ؼ��� ��üȭ�ȴ�.
	template <typename TComponent>
	TComponent* GameObject::AddComponent()
	{
		static_assert(std::is_base_of_v<ComponentBase, TComponent>,
			"TComponent must inherit from ComponentBase");

		TComponent* primitiveCom = new TComponent(this);

		// ��� ������Ʈ ��Ͽ� �־�Ӵϴ�.
		_allComponents.push_back(primitiveCom);

		// TODO
		// Resource (Memory) ������ ���� Deleter�� �Ű������� �־��� �� �ִ�.
		std::shared_ptr<TComponent> component(primitiveCom);

		// ������Ʈ ī��Ʈ ä������ �̺�Ʈ !
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
				// Ʈ�������� ���� ĳ���Ѵ�.
				_transform = component.get();
			}

			_components.push_back(component);

			return primitiveCom;
		}
	}
#pragma endregion
}