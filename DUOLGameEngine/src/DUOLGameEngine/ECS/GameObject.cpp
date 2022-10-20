#include <memory>

#include "DUOLGameEngine/ECS/GameObject.h"
#include <DUOLGameEngine/ECS/Component/BehaviourBase.h>

#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLGameEngine
{
	GameObject::GameObject(const DUOLCommon::tstring& name) :
		ObjectBase(name)
		, _parent(std::weak_ptr<GameObject>())
		, _childrens(std::vector<std::shared_ptr<GameObject>>())
		, _disabledBehaviours(std::vector<std::shared_ptr<BehaviourBase>>())
		, _layer(0)
		, _isActive(true)
	{
		// _transform
		// _components �� CreateEmptry���� �ʱ�ȭ�˴ϴ�.

		auto ptr = std::shared_ptr<DUOLGameEngine::GameObject>(this);
	}

	GameObject::~GameObject()
	{
		UnInitialize();
	}

	void GameObject::SetChildren(const std::shared_ptr<DUOLGameEngine::GameObject>& children)
	{
		for (auto& child : _childrens)
			if (child == children)
				return;

		_childrens.push_back(children);

		children->SetParent(this->shared_from_this());
	}

	void GameObject::SetParent(const std::shared_ptr<DUOLGameEngine::GameObject>& parent)
	{
		/// <summary>
		/// ���� �θ� ���踦 �����մϴ�.
		/// </summary>
		/// <param name="parent"></param>
		if (_parent.lock() != nullptr)
		{
			_parent.lock()->ResetHierarchy(this->shared_from_this());
		}

		if (parent == nullptr)
			this->_parent = std::weak_ptr<DUOLGameEngine::GameObject>();
		else
			this->_parent = parent->weak_from_this();
	}

	void GameObject::ResetHierarchy(const std::shared_ptr<DUOLGameEngine::GameObject>& target)
	{
		std::erase_if(_childrens, [&target](const std::shared_ptr<DUOLGameEngine::GameObject>& item)
			{
				return *target == *item;
			});
	}

	void GameObject::UnInitialize()
	{
		_parent.reset();

		_transform.reset();

		/// <summary>
		/// �̰� ���� �ϴ� ���� �³� ..? => Destroy �� �޸𸮿����� ������
		///	ObjectManager�� �ǽ��ϴ� ������ �սô�.
		/// </summary>
		for (auto& children : _childrens)
		{
			children.reset();
		}

		for (auto& component : _components)
		{
			component.reset();
		}

		for (auto& disabledBehaviour : _disabledBehaviours)
		{
			disabledBehaviour.reset();
		}
	}

	void GameObject::SetBehaviourEnabled(const std::shared_ptr<DUOLGameEngine::BehaviourBase>& target)
	{
		std::erase_if(_disabledBehaviours, [&target, this](const std::shared_ptr<DUOLGameEngine::BehaviourBase>& item)
			{
				if (*target != *item)
				{
					return false;
				}
				else
				{
					// �ش� Behaviour�� Ȱ��ȭ�մϴ�.
					this->_components.push_back(target);

					return true;
				}
			});
	}

	void GameObject::SetBehaviourDisabled(const std::shared_ptr<DUOLGameEngine::BehaviourBase>& target)
	{
		std::erase_if(_components, [&target, this](const std::shared_ptr<DUOLGameEngine::ComponentBase>& item)
			{
				if (*target != *item)
				{
					return false;
				}
				else
				{
					// �ش� Behaviour�� Ȱ��ȭ�մϴ�.
					this->_disabledBehaviours.push_back(target);

					return true;
				}
			});
	}

	void GameObject::OnAwake()
	{
		for (const auto& component : _components)
		{
			component->OnAwake();
		}

		/// <summary>
		/// OnAwake �� Ȱ��ȭ ���°� �ƴ� Behaviour�� �߻��մϴ�.
		/// </summary>
		for (const auto& behaviour : _disabledBehaviours)
		{
			behaviour->OnAwake();
		}
	}

	void GameObject::OnStart()
	{
		for (const auto& component : _components)
		{
			component->OnStart();
		}
	}

	void GameObject::OnActive()
	{
		// TODO
		// ���� ��� ������ �Ǿ� �ִ� ������Ʈ�� ���ؼ��� �����ϴ°� ..? �� ���� �ǹ�.
		for (const auto& component : _components)
		{
			component->OnEnable();
		}
	}

	void GameObject::OnInActive()
	{
		// TODO
		// ���� ��� ������ �Ǿ� �ִ� ������Ʈ�� ���ؼ��� �����ϴ°� ..? �� ���� �ǹ�.
		for (const auto& component : _components)
		{
			component->OnDisable();
		}
	}

	void GameObject::OnDestroy()
	{
		// TODO
		// �ش� ������Ʈ�� �ִ� ������Ʈ���� OnDestroy�� ȣ���մϴ�.
		// �ش� �Լ��� ���� SceneManager => ObjectManager���� ȣ��˴ϴ�.
		for (const auto& component : _components)
		{
			component->OnDisable();
		}

		for (const auto& behaviour : _disabledBehaviours)
		{
			behaviour->OnDisable();
		}
	}

	void GameObject::OnUpdate(float deltaTime)
	{
		for (const auto& component : _components)
		{
			component->OnUpdate(deltaTime);
		}
	}

	void GameObject::OnFixedUpdate(float deltaTime)
	{
		for (const auto& component : _components)
		{
			component->OnFixedUpdate(deltaTime);
		}
	}

	void GameObject::OnLateUpdate(float deltaTime)
	{
		for (const auto& component : _components)
		{
			component->OnLateUpdate(deltaTime);
		}
	}

	void GameObject::SetIsActive(bool value)
	{
		const std::shared_ptr<DUOLGameEngine::Scene> scene = _scene.lock();

		if ((scene == nullptr) || (value == _isActive))
			return;

		if (value)
			scene->RegisterActive(this->shared_from_this());
		else
			scene->RegisterInActive(this->shared_from_this());
	}
}