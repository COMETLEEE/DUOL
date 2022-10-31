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
		, _disabledBehaviours(std::vector<std::shared_ptr<BehaviourBase>>())
		, _layer(0)
		, _isActive(true)
	{
		// _transform
		// _components 는 CreateEmptry에서 초기화됩니다.

		auto ptr = std::shared_ptr<DUOLGameEngine::GameObject>(this);
	}

	GameObject::~GameObject()
	{
		UnInitialize();
	}


	void GameObject::UnInitialize()
	{
		_transform.reset();

		_monoBehaviour.reset();

		/// <summary>
		/// 이거 리셋 하는 것이 맞나 ..? => Destroy 등 메모리에서의 삭제는
		///	ObjectManager가 실시하는 것으로 합시다.
		/// </summary>
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
					// 해당 Behaviour를 활성화합니다.
					this->_components.push_back(target);

					// 그냥 상수를 바꿔준다. BehaviourBase의 SetIsEnabled 함수는 해당 객체가 속한 게임 오브젝트의 이 함수를 호출한다.
					target->_isEnabled = true;

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
					// 해당 Behaviour를 비활성화합니다.
					this->_disabledBehaviours.push_back(target);

					// 그냥 상수를 바꿔준다. BehaviourBase의 SetIsEnabled 함수는 해당 객체가 속한 게임 오브젝트의 이 함수를 호출한다.
					target->_isEnabled = false;

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
		/// OnAwake 는 활성화 상태가 아닌 Behaviour도 발생합니다.
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
		// 현재 사용 중으로 되어 있는 컴포넌트에 대해서만 실행하는가 ..? 에 대한 의문.
		for (const auto& component : _components)
		{
			component->OnEnable();
		}
	}

	void GameObject::OnInActive()
	{
		// TODO
		// 현재 사용 중으로 되어 있는 컴포넌트에 대해서만 실행하는가 ..? 에 대한 의문.
		for (const auto& component : _components)
		{
			component->OnDisable();
		}
	}

	void GameObject::OnDestroy()
	{
		// TODO
		// 해당 오브젝트에 있던 컴포넌트들의 OnDestroy를 호출합니다.
		// 해당 함수는 추후 SceneManager => ObjectManager에서 호출됩니다.
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

	void GameObject::OnCoroutineUpdate(float deltaTime)
	{
		if (_monoBehaviour != nullptr)
		{
			_monoBehaviour->UpdateAllCoroutines(deltaTime);
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

		// 자식들도 켜줘야함
		const std::vector<std::weak_ptr<Transform>>& children = _transform->GetChildren();

		for (auto& child : children)
		{
			if (child.lock() != nullptr)
				child.lock()->GetGameObject()->SetIsActive(value);
		}
	}
}