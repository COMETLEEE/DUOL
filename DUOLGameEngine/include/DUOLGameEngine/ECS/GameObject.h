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
		std::shared_ptr<DUOLGameEngine::Transform> _transform;

		std::shared_ptr<DUOLGameEngine::MonoBehaviourBase> _monoBehaviour;

		/**
		 * \brief Ȱ��ȭ �Ǿ� �ִ� ��� ������Ʈ���� ����Ʈ
		 */
		std::vector<std::shared_ptr<DUOLGameEngine::ComponentBase>> _components;

		/**
		 * \brief ��Ȱ��ȭ �Ǿ� �ִ� ��� Behaviour���� ����Ʈ
		 */
		std::vector<std::shared_ptr<DUOLGameEngine::BehaviourBase>> _disabledBehaviours;

	public:
		inline const std::shared_ptr<DUOLGameEngine::Transform>& GetTransform() const { return _transform; }

		template <typename TComponent>
		std::shared_ptr<TComponent> GetComponent() const;

		template <typename TComponent>
		std::shared_ptr<TComponent> AddComponent();

	private:
		/**
		 * \brief ���� ������Ʈ�� �����մϴ�.
		 */
		void UnInitialize();

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

		friend class Scene;
#pragma endregion
	};

#pragma region GET_ADD_COMPONENT
	template <typename TComponent>
	std::shared_ptr<TComponent> GameObject::GetComponent() const
	{
		static_assert(std::is_base_of<ComponentBase, TComponent>::value,
			"TComponent must inherit from ComponentBase");

		// Ʈ������ ĳ�� (�̰� ����� �������� ���ڴ� .. GetTransform �����ϱ� ..
		if constexpr (std::is_same_v<TComponent, Transform>)
			return _transform;

		// ��� �����̺�� ĳ��
		if constexpr (std::is_base_of_v<MonoBehaviourBase, TComponent>)
			return _monoBehaviour;

		// TODO
		// �ƹ����� �ڵ�ȭ�� ȿ������ ���ÿ� �߱��ϱ� ���ؼ��� .. ���÷��� ����� �ʿ��� �� �����ϴ�.
#pragma region USING_DYNAMIC_CAST
		for (int i = 0 ; i < _components.size() ; i++)
		{
			std::shared_ptr<TComponent> dcComponent = std::dynamic_pointer_cast<TComponent>(_components[i]);

			if (dcComponent != nullptr)
				return dcComponent;
		}

		for (int i = 0; i < _disabledBehaviours.size(); i++)
		{
			std::shared_ptr<TComponent> dcComponent = std::dynamic_pointer_cast<TComponent>(_disabledBehaviours[i]);

			if (dcComponent != nullptr)
				return dcComponent;
		}

		// ����. nullptr ��ȯ.
		return nullptr;
#pragma endregion
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
			_monoBehaviour = component;
		}

		if constexpr (std::is_same_v<TComponent, Transform>)
		{
			// Ʈ�������� ���� ĳ���Ѵ�.
			_transform = component;
		}

		_components.push_back(component);

		return component;
	}
#pragma endregion
}