#include <memory>

#include "DUOLGameEngine/ECS/GameObject.h"
#include <DUOLGameEngine/ECS/Component/BehaviourBase.h>

#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLGameEngine
{
	GameObject::GameObject(const DUOLCommon::tstring& name) :
		ObjectBase(name)
		, _transform(nullptr)
		, _components(std::list<std::shared_ptr<DUOLGameEngine::ComponentBase>>())
		, _abledBehaviours(std::list<std::shared_ptr<DUOLGameEngine::BehaviourBase>>())
		, _disabledBehaviours(std::list<std::shared_ptr<DUOLGameEngine::BehaviourBase>>())
		, _abledMonoBehaviours(std::list<std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>>())
		, _disabledMonoBehaviours(std::list<std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>>())
		, _physicsActor()
		, _tag(TEXT("GameObject"))
		, _layer(0)
		, _isActive(true)
	{
		
	}

	GameObject::~GameObject()
	{
		UnInitialize();
	}

	void GameObject::UnInitialize()
	{
		/// <summary>
		/// �̰� ���� �ϴ� ���� �³� ..? => Destroy �� �޸𸮿����� ������
		///	SceneManager -> ObjectManager�� �ǽ��ϴ� ������ �սô�.
		/// </summary>
		_transform.reset();

		for (auto& component : _components)
		{
			component.reset();
		}

		for (auto& abledBehaviour : _abledBehaviours)
		{
			abledBehaviour.reset();
		}

		for (auto& disabledBehaviour : _disabledBehaviours)
		{
			disabledBehaviour.reset();
		}

		for (auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour.reset();
		}

		for (auto& disabledMonoBehaviour : _disabledMonoBehaviours)
		{
			disabledMonoBehaviour.reset();
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
					this->_abledBehaviours.push_back(target);

					return true;
				}
			});
	}

	void GameObject::SetBehaviourDisabled(const std::shared_ptr<DUOLGameEngine::BehaviourBase>& target)
	{
		std::erase_if(_abledBehaviours, [&target, this](const std::shared_ptr<DUOLGameEngine::ComponentBase>& item)
			{
				if (*target != *item)
				{
					return false;
				}
				else
				{
					// �ش� Behaviour�� ��Ȱ��ȭ�մϴ�.
					this->_disabledBehaviours.push_back(target);

					return true;
				}
			});
	}

	void GameObject::SetMonoBehaviourEnabled(const std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>& target)
	{
		std::erase_if(_disabledMonoBehaviours, [&target, this](const std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>& item)
			{
				if (*target != *item)
				{
					return false;
				}
				else
				{
					// �ش� MonoBehaviour�� Ȱ��ȭ�մϴ�.
					this->_abledMonoBehaviours.push_back(target);

					return true;
				}
			});
	}

	void GameObject::SetMonoBehaviourDisabled(const std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>& target)
	{
		std::erase_if(_abledMonoBehaviours, [&target, this](const std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>& item)
			{
				if (*target != *item)
				{
					return false;
				}
				else
				{
					// �ش� MonoBehaviour�� Ȱ��ȭ�մϴ�.
					this->_disabledMonoBehaviours.push_back(target);

					return true;
				}
			});
	}

	void GameObject::OnAwake()
	{
		// ��� ������Ʈ�鿡�� �����մϴ�.
		for (const auto& component : _components)
		{
			component->OnAwake();
		}

		for (const auto& abledBehaviour : _abledBehaviours)
		{
			abledBehaviour->OnAwake();
		}

		for (const auto& disabledBehaviour : _disabledBehaviours)
		{
			disabledBehaviour->OnAwake();
		}

		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->OnAwake();
		}

		for (const auto& disabledMonoBehaviour : _disabledMonoBehaviours)
		{
			disabledMonoBehaviour->OnAwake();
		}
	}

	void GameObject::OnStart()
	{
		// ABLE ������ ������Ʈ�鿡�Ը� �����մϴ�.
		for (const auto& component : _components)
		{
			component->OnStart();
		}

		for (const auto& abledBehaviour : _abledBehaviours)
		{
			abledBehaviour->OnStart();
		}

		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->OnStart();
		}
	}

	void GameObject::OnActive()
	{
		// ���� ���� �ִ� ������Ʈ�鿡 ���ؼ��� OnEnable ȣ��
		for (const auto& abledBehaviour : _abledBehaviours)
		{
			abledBehaviour->OnEnable();
		}

		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->OnEnable();
		}
	}

	void GameObject::OnInActive()
	{
		for (const auto& abledBehaviour : _abledBehaviours)
		{
			abledBehaviour->OnDisable();
		}

		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->OnDisable();
		}
	}

	void GameObject::OnDestroy()
	{
		// TODO
		// �ش� ������Ʈ�� �ִ� ������Ʈ���� OnDestroy�� ȣ���մϴ�.
		// �ش� �Լ��� ���� SceneManager => ObjectManager���� ȣ��˴ϴ�.
		for (const auto& behaviour : _abledBehaviours)
		{
			behaviour->OnDisable();
		}

		for (const auto& behaviour : _disabledBehaviours)
		{
			behaviour->OnDisable();
		}

		for (const auto& monoBehaviour : _abledBehaviours)
		{
			monoBehaviour->OnDisable();
		}

		for (const auto& monoBehaviour : _disabledMonoBehaviours)
		{
			monoBehaviour->OnDisable();
		}
	}

	void GameObject::OnUpdate(float deltaTime)
	{
		for (const auto& component : _components)
		{
			component->OnUpdate(deltaTime);
		}

		for (const auto& abledBehaviour : _abledBehaviours)
		{
			abledBehaviour->OnUpdate(deltaTime);
		}

		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->OnUpdate(deltaTime);
		}
	}

	void GameObject::OnCoroutineUpdate(float deltaTime)
	{
		// Ȱ��ȭ�� MonoBehaviourBase �� ȣ�⿡ �ش�Ǵ� �Լ��Դϴ�.
		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->UpdateAllCoroutines(deltaTime);
		}
	}

	void GameObject::OnInvokeUpdate(float deltaTime)
	{
		// Ȱ��ȭ�� MonoBehaivourBase �� ȣ�⿡ �ش�Ǵ� �Լ��Դϴ�.
		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->UpdateAllInvokes(deltaTime);
		}
	}

	void GameObject::OnFixedUpdate(float deltaTime)
	{
		for (const auto& component : _components)
		{
			component->OnFixedUpdate(deltaTime);
		}

		for (const auto& abledBehaviour : _abledBehaviours)
		{
			abledBehaviour->OnFixedUpdate(deltaTime);
		}

		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->OnFixedUpdate(deltaTime);
		}
	}

	void GameObject::OnLateUpdate(float deltaTime)
	{
		for (const auto& component : _components)
		{
			component->OnLateUpdate(deltaTime);
		}

		for (const auto& abledBehaviour : _abledBehaviours)
		{
			abledBehaviour->OnLateUpdate(deltaTime);
		}

		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->OnLateUpdate(deltaTime);
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

		// �ڽĵ鵵 �������. �ٵ� �ڽĵ��� ���� On / Off ���¸� ����ϰ� �־���� ..
		const std::vector<std::weak_ptr<Transform>>& children = _transform->GetChildren();

		for (auto& child : children)
		{
			if (child.lock() != nullptr)
				child.lock()->GetGameObject()->SetIsActive(value);
		}
	}
}