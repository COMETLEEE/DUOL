#include <memory>

#include "DUOLGameEngine/ECS/GameObject.h"
#include <DUOLGameEngine/ECS/Component/BehaviourBase.h>

#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include "DUOLGameEngine/Manager/PhysicsManager.h"

namespace DUOLGameEngine
{
	class Camera;

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

	const std::list<DUOLGameEngine::ComponentBase*>& GameObject::GetAllComponents() const
	{
		return _allComponents;
	}

	void GameObject::SetBehaviourEnabled(const std::shared_ptr<DUOLGameEngine::BehaviourBase>& target)
	{
		std::erase_if(_disabledBehaviours, [&target, this](const std::shared_ptr<DUOLGameEngine::BehaviourBase>& item)
			{
				if (target.get() != item.get())
				{
					return false;
				}
				else
				{
					// 해당 Behaviour를 활성화합니다.
					this->_abledBehaviours.push_back(target);

					// Enable !
					target->OnEnable();

					return true;
				}
			});
	}

	void GameObject::SetBehaviourDisabled(const std::shared_ptr<DUOLGameEngine::BehaviourBase>& target)
	{
		std::erase_if(_abledBehaviours, [&target, this](const std::shared_ptr<DUOLGameEngine::ComponentBase>& item)
			{
				// 주소가 다르면 다른 것입니다.
				if (target.get() != item.get())
				{
					return false;
				}
				else
				{
					// 해당 Behaviour를 비활성화합니다.
					this->_disabledBehaviours.push_back(target);

					// Disable !
					target->OnDisable();

					return true;
				}
			});
	}

	void GameObject::SetMonoBehaviourEnabled(const std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>& target)
	{
		std::erase_if(_disabledMonoBehaviours, [&target, this](const std::shared_ptr<DUOLGameEngine::MonoBehaviourBase>& item)
			{
				if (target.get() != item.get())
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
				if (target.get() != item.get())
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

	DUOLGameEngine::ComponentBase* GameObject::AddComponent(const DUOLCommon::tstring& componentName)
	{
		using namespace rttr;

		// List에서 눌린 이름의 컴포넌트를 불러옵니다.
		rttr::type componentType = type::get_by_name(DUOLCommon::StringHelper::ToString(componentName));

		auto baseClasses = componentType.get_base_classes();

		std::vector<rttr::type> param;

		rttr::variant var = weak_from_this();

		rttr::variant var1 = std::wstring();

		param.push_back(var.get_type());
		param.push_back(var1.get_type());

		rttr::constructor con = componentType.get_constructor(param);

		// 주석을 보면 만들어지는 객체는 Heap에 할당됩니다.
		rttr::variant createdCom = con.invoke(var, DUOLCommon::StringHelper::ToTString(componentType.get_name().to_string()));

		// TODO : PhysX 와 관련이 있는 Component 인지 체크하고 분기해야합니다. (콜라이더는 한 오브젝트 당 한 개만 가능, 리지드 바디 또한 한 개만 가능)

		// MonoBehaviourBase
		if (componentType.is_derived_from(type::get_by_name("MonoBehaviourBase")))
		{
			auto& monoBehaviour = createdCom.get_value<DUOLGameEngine::MonoBehaviourBase>();

			std::shared_ptr<MonoBehaviourBase> mono(const_cast<DUOLGameEngine::MonoBehaviourBase*>(&monoBehaviour));

			mono->OnAwake();

			mono->OnStart();

			mono->OnEnable();

			mono->AllProcessOnEnable();

			_abledMonoBehaviours.push_back(monoBehaviour.shared_from_base());

			_allComponents.push_back(&monoBehaviour);

			return &monoBehaviour;
		}
		// BehaviourBase
		else if (componentType.is_derived_from(type::get_by_name("BehaviourBase")))
		{
			auto& behaviour = createdCom.get_wrapped_value<DUOLGameEngine::BehaviourBase>();

			std::shared_ptr<BehaviourBase> beha(const_cast<DUOLGameEngine::BehaviourBase*>(&behaviour));

			// 물리 객체를 초기화합니다. 
#pragma region PHYSX_COMPONENTS_INIT
			if (componentType.is_derived_from(type::get_by_name("ColliderBase")))
			{
				DUOLGameEngine::PhysicsManager::GetInstance()->
					AttachPhysicsCollider(this, reinterpret_cast<ColliderBase*>(beha.get()));
			}
#pragma endregion

			beha->OnAwake();

			beha->OnStart();

			beha->OnEnable();

			_abledBehaviours.push_back(beha);

			_allComponents.push_back(beha.get());

			return beha.get();
		}
		// ComponentBase
		else if (componentType.is_derived_from(type::get_by_name("ComponentBase")))
		{
			auto& component = createdCom.get_wrapped_value<DUOLGameEngine::ComponentBase>();

			std::shared_ptr<ComponentBase> com(const_cast<DUOLGameEngine::ComponentBase*>(&component));

			// 물리 객체를 초기화합니다. 현재까지 ComponentBase 하단은 Rigidbody 뿐입니다.
#pragma region PHYSX_COMPONENTS_INIT
			if (componentType.get_name().to_string() == "Rigidbody")
			{
				DUOLGameEngine: PhysicsManager::GetInstance()->AttachPhysicsDynamicActor(this, reinterpret_cast<Rigidbody*>(com.get()));
			}
#pragma endregion

			com->OnAwake();

			com->OnStart();

			_components.push_back(com);

			_allComponents.push_back(com.get());

			return com.get();
		}
	}

	DUOLGameEngine::ComponentBase* GameObject::GetComponent(const DUOLCommon::tstring& componentName)
	{
		rttr::type targetType = rttr::type::get_by_name(DUOLCommon::StringHelper::ToString(componentName));

		for (auto& component : _allComponents)
		{
			// 같은 타입이면 해당 컴포넌트를 리턴합니다.
			if (component->get_type() == targetType)
				return component;
		}
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

			abledMonoBehaviour->AllProcessOnEnable();
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

			abledMonoBehaviour->AllProcessOnDisable();
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
		value ? scene->RegisterActiveGameObject(this) : scene->RegisterInActiveGameObject(this);
	}

	void GameObject::RegisterDestroyComponent(DUOLGameEngine::ComponentBase* component, float time)
	{
#pragma region SEARCH_COMPONENT
		for (auto com : _components)
		{
			if (com.get() == component)
				_componentsForDestroy.push_back({ com, time, {} });
		}

		for (auto behaviour : _abledBehaviours)
		{
			if (behaviour.get() == component)
			{
				_componentsForDestroy.push_back({ behaviour, time });
			}
		}

		for (auto behaviour : _disabledBehaviours)
		{
			if (behaviour.get() == component)
			{
				_componentsForDestroy.push_back({ behaviour, time });
			}
		}

		for (auto monoBehaviour : _abledMonoBehaviours)
		{
			if (monoBehaviour.get() == component)
			{
				_componentsForDestroy.push_back({ monoBehaviour, time });
			}
		}

		for (auto monoBehaviour : _disabledMonoBehaviours)
		{
			if (monoBehaviour.get() == component)
			{
				_componentsForDestroy.push_back({ monoBehaviour, time });
			}
		}
#pragma endregion
	}
}