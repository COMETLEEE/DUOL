#pragma once
#include <algorithm>
#include <vector>

#include "DUOLGameEngine/ECS/Component/ObjectBase.h"

// For define template 'Add / Get' Component functions.
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class ComponentBase;

	class BehaviourBase;

	class MonoBehaviourBase;

	class Transform;

	class Scene;

	/**
	 * \brief ���� ���� ��ƼƼ�� ��Ÿ���� Ŭ�����Դϴ�.
	 */
	class GameObject : public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<GameObject>
	{
	public:
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
		std::shared_ptr<DUOLGameEngine::Transform> _transform;

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

	public:
		inline const std::shared_ptr<DUOLGameEngine::Transform>& GetTransform() const { return _transform; }

		template <typename TComponent>
		std::shared_ptr<TComponent> GetComponent() const;


		template <typename TComponent>
		std::shared_ptr<TComponent> AddComponent();

	private:
		template <typename TComponent, typename TComponentBase>
		std::shared_ptr<TComponent> SuchComponent() const;

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
#pragma endregion
	public:
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
		 * \brief ������Ʈ�� �ı��� �� ȣ��˴ϴ�.
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
		 * \brief �� ���� �����ӿ� ȣ��˴ϴ�.
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
		 */
		virtual void OnFixedUpdate(float deltaTime);

		/**
		 * \brief OnUpdate ����, �� ������ ȣ��˴ϴ�.
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
		 */
		virtual void OnLateUpdate(float deltaTime);

	private:
		/**
		 * \brief �ش� ���� ������Ʈ�� �����ִ� ���Դϴ�.
		 */
		std::weak_ptr<DUOLGameEngine::Scene> _scene;

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

	public:
		inline std::shared_ptr<DUOLGameEngine::Scene> GetScene() const { return _scene.lock(); }

		inline const DUOLCommon::tstring& GetTag() const { return _tag; }

		inline void SetTag(const DUOLCommon::tstring& tag) { _tag = tag; }

		inline int GetLayer() const { return _layer; }

		inline void SetLayer(int layer) { _layer = layer; }

		inline bool GetIsActive() const { return _isActive; }

		void SetIsActive(bool value);
#pragma region FRIEND_CLASS
		friend class ComponentBase;

		friend class BehaviourBase;

		friend class MonoBehaviourBase;

		friend class Scene;
#pragma endregion
	};

#pragma region GET_ADD_COMPONENT
	template <typename TComponent>
	std::shared_ptr<TComponent> GameObject::GetComponent() const
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
		else
		{
			return SuchComponent<TComponent, ComponentBase>();
		}
	}

	// Dynamic pointer cast �ڵ带 ���⿡ �� ���Ƴ־� �����ϴ�. ���߿� �ٲ��� .. (Enum ..? Reflection ..? �ڵ�ȭ�� �����̳� ..)
	template <typename TComponent, typename TComponentBase>
	std::shared_ptr<TComponent> GameObject::SuchComponent() const
	{
		if constexpr (std::is_same_v<TComponentBase, MonoBehaviourBase>)
		{
			for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
			{
				std::shared_ptr<TComponent> dcMonoBehaviour = std::dynamic_pointer_cast<TComponent>(abledMonoBehaviour);

				if (dcMonoBehaviour != nullptr)
					return dcMonoBehaviour;
			}

			for (const auto& disabledMonoBehaviour : _disabledMonoBehaviours)
			{
				std::shared_ptr<TComponent> dcMonoBehaviour = std::dynamic_pointer_cast<TComponent>(disabledMonoBehaviour);

				if (dcMonoBehaviour != nullptr)
					return dcMonoBehaviour;
			}
		}
		else if constexpr (std::is_same_v<TComponentBase, BehaviourBase>)
		{
			for (const auto& abledBehaviour : _abledBehaviours)
			{
				std::shared_ptr<TComponent> dcBehaviour = std::dynamic_pointer_cast<TComponent>(abledBehaviour);

				if (dcBehaviour != nullptr)
					return dcBehaviour;
			}

			for (const auto& disabledBehaviour : _disabledBehaviours)
			{
				std::shared_ptr<TComponent> dcBehaviour = std::dynamic_pointer_cast<TComponent>(disabledBehaviour);

				if (dcBehaviour != nullptr)
					return dcBehaviour;
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
					return dcComponent;
			}
		}

		// �ش��ϴ� ������Ʈ�� �����ϴ�.
		return nullptr;
	}
	
	// ������Ʈ�� �ش� �Լ��� ���ؼ��� ��üȭ�ȴ�.
	template <typename TComponent>
	std::shared_ptr<TComponent> GameObject::AddComponent()
	{
		static_assert(std::is_base_of_v<ComponentBase, TComponent>,
			"TComponent must inherit from ComponentBase");

		TComponent* primitiveCom = new TComponent(this->weak_from_this());

		// TODO
		// Resource (Memory) ������ ���� Deleter�� �Ű������� �־��� �� �ִ�.
		std::shared_ptr<TComponent> component(primitiveCom);

		if constexpr (std::is_base_of_v<MonoBehaviourBase, TComponent>)
		{
			_abledMonoBehaviours.push_back(component);
		}
		else if constexpr (std::is_base_of_v<BehaviourBase, TComponent>)
		{
			_abledBehaviours.push_back(component);
		}
		else
		{
			if constexpr (std::is_same_v<Transform, TComponent>)
			{
				// Ʈ�������� ���� ĳ���Ѵ�.
				_transform = component;
			}

			_components.push_back(component);
		}

		return component;
	}
#pragma endregion
}