#include <memory>

#include "DUOLGameEngine/ECS/GameObject.h"
#include <DUOLGameEngine/ECS/Component/BehaviourBase.h>

#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLGameEngine
{
	GameObject::GameObject(const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::GameObject)
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
		, _isStarted(false)
	{
		
	}

	GameObject::~GameObject()
	{
		UnInitialize();
	}

	void GameObject::UnInitialize()
	{
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

	void GameObject::OnCreate()
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

		if (_isActive)
		{
			// ABLE 상태의 컴포넌트들에게만 전달합니다.
			for (const auto& component : _components)
			{
				component->OnStart();
			}

			for (const auto& abledBehaviour : _abledBehaviours)
			{
				abledBehaviour->OnStart();

				abledBehaviour->_isStarted = true;
			}

			for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
			{
				abledMonoBehaviour->OnStart();

				abledMonoBehaviour->_isStarted = true;

				// MonoBehaviourBase의 경우
				// 클라이언트에서 정의한 이벤트들의 등록 등 할 일이 있습니다.
				abledMonoBehaviour->AllProcessOnEnable();
			}

			_isStarted = true;
		}
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

		// 재귀적으로 자식 오브젝트까지 실시합니다.
		auto&& children = GetTransform()->GetChildGameObjects();

		// Awake는 Active / InActive 관련 없이 진행합니다.
		for (auto& child : children)
			child->OnAwake();
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

			abledBehaviour->_isStarted = true;
		}

		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->OnStart();

			abledMonoBehaviour->_isStarted = true;

			// MonoBehaviourBase의 경우
			// 클라이언트에서 정의한 이벤트들의 등록 등 할 일이 있습니다.
			abledMonoBehaviour->AllProcessOnEnable();
		}

		// 재귀적으로 자식 오브젝트까지 실시합니다.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
		{
			if (child->_isActive)
				child->OnStart();
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

		// 재귀적으로 자식 오브젝트까지 실시합니다.
		auto&& children = GetTransform()->GetChildGameObjects();

		// TODO : 자식 오브젝트의 기존 Active 여부 기억하고 있어야 합니다 ..?
		for (auto& child : children)
		{
			// 내가 켜질 때에는 기존에 켜져 있던 오브젝트 (_isActive) 한 오브젝트만 다시 켜줍니다.
			if (child->_isActive)
				child->OnActive();
		}
	}

	void GameObject::OnInActive()
	{
		// 현재 켜져 있는 컴포넌트들에 대해서만 OnDisable() 호출
		for (const auto& abledBehaviour : _abledBehaviours)
		{
			abledBehaviour->OnDisable();
		}

		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->OnDisable();
		}

		// 재귀적으로 자식 오브젝트까지 실시합니다.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
		{
			// 내가 꺼질 때는 그냥 자식 오브젝트들도 전부 꺼트리면 됩니다.
			child->OnInActive();
		}
	}

	void GameObject::OnDestroy()
	{
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

		// 재귀적으로 자식 오브젝트까지 실시합니다.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
			child->OnDestroy();
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

		// 재귀적으로 자식 오브젝트까지 실시합니다.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
		{
			if (child->_isActive)
				child->OnUpdate(deltaTime);
		}
	}

	void GameObject::OnCoroutineUpdate(float deltaTime)
	{
		// 활성화된 MonoBehaviourBase 만 호출에 해당되는 함수입니다.
		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->UpdateAllCoroutines(deltaTime);
		}

		// 재귀적으로 자식 오브젝트까지 실시합니다.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
		{
			if (child->_isActive)
				child->OnCoroutineUpdate(deltaTime);
		}
	}

	void GameObject::OnInvokeUpdate(float deltaTime)
	{
		// 활성화된 MonoBehaivourBase 만 호출에 해당되는 함수입니다.
		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->UpdateAllInvokes(deltaTime);
		}

		// 재귀적으로 자식 오브젝트까지 실시합니다.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
		{
			if (child->_isActive)
				child->OnInvokeUpdate(deltaTime);
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

		// 재귀적으로 자식 오브젝트까지 실시합니다.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
		{
			if (child->_isActive)
				child->OnFixedUpdate(deltaTime);
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

		// 재귀적으로 자식 오브젝트까지 실시합니다.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
		{
			if (child->_isActive)
				child->OnLateUpdate(deltaTime);
		}
	}

	Scene* GameObject::GetScene() const
	{
		if (_scene.expired())
			return nullptr;
		else
			return _scene.lock().get();
	}

	void GameObject::SetIsActive(bool value)
	{
		// 현재 게임 오브젝트가 존재하는 씬
		const std::shared_ptr<DUOLGameEngine::Scene> scene = _scene.lock();

		if ((scene == nullptr) || (value == _isActive))
			return;

		// 실제로 Register 에서 끝나는 것이 아니라 다음 프레임에 Active List로 들어오면 Active한다.
		if (value)
			scene->RegisterActiveGameObject(this);
		else
			scene->RegisterInActiveGameObject(this);
	}
}