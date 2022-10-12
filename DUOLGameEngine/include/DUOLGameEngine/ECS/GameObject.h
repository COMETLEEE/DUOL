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
	 * \brief ���� ���� ��ƼƼ�� ��Ÿ���� Ŭ�����Դϴ�.
	 */
	class GameObject : public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<GameObject>
	{
	public:
		/**
		 * \brief ���� ������Ʈ ��ü�� ����ϴ�. �ٸ�, ���� �ش� ���� �����Ϳ� ���ؼ� shared_ptr ��ü��
		 * ������ �ʰ� shared_from_this�� ȣ���ϸ� Exception�� �߻��մϴ�.
		 * ����, ������ �����ڸ� private�� ����, shared_ptr�� ��ȯ�ϴ� CreateEmptry() ���� �Լ��� ����ξ�� �մϴ�.
		 * \param name �ش� ���� ������Ʈ�� �̸��Դϴ�.
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
		 * \brief Ÿ�� �ڽ� ���� ������Ʈ�� ���ؼ� ���� ���踦 �����մϴ�.
		 * \param target ������ ����� �Ǵ� �ڽ� ���� ������Ʈ�Դϴ�.
		 */
		void ResetHierarchy(const std::shared_ptr<DUOLGameEngine::GameObject>& target);

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
		* \brief ���� ������ �� OnStart ���� ������ ȣ��˴ϴ�.
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
		 * \brief ���� ������Ʈ�� �±��Դϴ�. Tag�� Layer Manager���� ���� �� ����մϴ�.
		 */
		tstring _tag;

		/**
		 * \brief ���� ������Ʈ�� ���̾��Դϴ�. ���̾�� �������� ������ �� ���� ĳ��Ʈ�� ����� �� �ֽ��ϴ�.
		 */
		int _layer;

		/**
		 * \brief ���� ������Ʈ�� Ȱ��ȭ �������� ��Ÿ���ϴ�.
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
		// �ƹ����� �ڵ�ȭ�� ȿ������ ���ÿ� �߱��ϱ� ���ؼ��� .. ���÷��� ����� �ʿ��� �� �����ϴ�.
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
		// Resource (Memory) ������ ���� Deleter�� �Ű������� �־��� �� �ִ�.
		std::shared_ptr<TComponent> component(primitiveCom);

		_components.push_back(component);

		return primitiveCom;
	}
}