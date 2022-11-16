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
		/// 이거 리셋 하는 것이 맞나 ..? => Destroy 등 메모리에서의 삭제는
		///	SceneManager -> ObjectManager가 실시하는 것으로 합시다.
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
					// 해당 Behaviour를 활성화합니다.
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
					// 해당 Behaviour를 비활성화합니다.
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
					// 해당 MonoBehaviour를 활성화합니다.
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
					// 해당 MonoBehaviour를 활성화합니다.
					this->_disabledMonoBehaviours.push_back(target);

					return true;
				}
			});
	}

	void GameObject::OnAwake()
	{
		// 모든 컴포넌트들에게 전달합니다.
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
		// ABLE 상태의 컴포넌트들에게만 전달합니다.
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
		// 현재 켜져 있는 컴포넌트들에 대해서만 OnEnable 호출
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
		// 해당 오브젝트에 있던 컴포넌트들의 OnDestroy를 호출합니다.
		// 해당 함수는 추후 SceneManager => ObjectManager에서 호출됩니다.
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
		// 활성화된 MonoBehaviourBase 만 호출에 해당되는 함수입니다.
		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->UpdateAllCoroutines(deltaTime);
		}
	}

	void GameObject::OnInvokeUpdate(float deltaTime)
	{
		// 활성화된 MonoBehaivourBase 만 호출에 해당되는 함수입니다.
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

		// 자식들도 켜줘야함. 근데 자식들의 기존 On / Off 상태를 기억하고 있어야함 ..
		const std::vector<std::weak_ptr<Transform>>& children = _transform->GetChildren();

		for (auto& child : children)
		{
			if (child.lock() != nullptr)
				child.lock()->GetGameObject()->SetIsActive(value);
		}
	}
}