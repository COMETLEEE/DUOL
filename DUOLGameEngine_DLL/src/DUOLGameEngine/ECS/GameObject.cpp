#include <memory>

#include "DUOLGameEngine/ECS/GameObject.h"
#include <DUOLGameEngine/ECS/Component/BehaviourBase.h>

#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::GameObject>("GameObject")
	.constructor()
	(
		// �ø�������� �ȿ��� ���� �� ����Ʈ �����ͷ� ��ȯ�����ִ� ����� ���Ѵ�.
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_tag", &DUOLGameEngine::GameObject::_tag)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)

	)
	.property("_layer", &DUOLGameEngine::GameObject::_layer)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)

	)
	.property("_isActive", &DUOLGameEngine::GameObject::_isActive)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_transform", &DUOLGameEngine::GameObject::_transform)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::FileUUID)
	)
	.property("_allComponents", &DUOLGameEngine::GameObject::_allComponents)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::FileUUID)
	)
	.property("_components", &DUOLGameEngine::GameObject::_components)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_abledBehaviours", &DUOLGameEngine::GameObject::_abledBehaviours)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_disabledBehaviours", &DUOLGameEngine::GameObject::_disabledBehaviours)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_abledMonoBehaviours", &DUOLGameEngine::GameObject::_abledMonoBehaviours)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_disabledMonoBehaviours", &DUOLGameEngine::GameObject::_disabledMonoBehaviours)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);
}

namespace DUOLGameEngine
{
	class Camera;

	GameObject::GameObject() :
		ObjectBase(TEXT("GameObject"), ObjectType::GameObject)
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
		for (auto& disabledMonoBehaviour : _disabledMonoBehaviours)
		{
			disabledMonoBehaviour.reset();
		}

		for (auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour.reset();
		}

		for (auto& disabledBehaviour : _disabledBehaviours)
		{
			disabledBehaviour.reset();
		}

		for (auto& abledBehaviour : _abledBehaviours)
		{
			abledBehaviour.reset();
		}

		for (auto& component : _components)
		{
			component.reset();
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
					// �ش� Behaviour�� Ȱ��ȭ�մϴ�.
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
				// �ּҰ� �ٸ��� �ٸ� ���Դϴ�.
				if (target.get() != item.get())
				{
					return false;
				}
				else
				{
					// �ش� Behaviour�� ��Ȱ��ȭ�մϴ�.
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
					// �ش� MonoBehaviour�� Ȱ��ȭ�մϴ�.
					this->_abledMonoBehaviours.push_back(target);

					target->OnEnable();

					target->AllProcessOnEnable();

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
					// �ش� MonoBehaviour�� Ȱ��ȭ�մϴ�
					this->_disabledMonoBehaviours.push_back(target);

					target->OnDisable();

					target->AllProcessOnDisable();

					return true;
				}
			});
	}

	DUOLGameEngine::ComponentBase* GameObject::AddComponent(const DUOLCommon::tstring& componentName)
	{
		using namespace rttr;

		// List���� ���� �̸��� ������Ʈ�� �ҷ��ɴϴ�.
		rttr::type componentType = type::get_by_name(DUOLCommon::StringHelper::ToString(componentName));

		// �븮������ ������ �ƹ��͵� ���� �ʽ��ϴ�.
		if (!componentType.is_valid())
			return nullptr;

		auto baseClasses = componentType.get_base_classes();

		std::vector<rttr::type> param;

		// �ּ�
		rttr::variant var = this;

		rttr::variant var1 = std::wstring();

		param.push_back(var.get_type());
		param.push_back(var1.get_type());

		rttr::constructor con = componentType.get_constructor(param);

		// �ּ��� ���� ��������� ��ü�� Heap�� �Ҵ�˴ϴ�.
		rttr::variant createdCom = con.invoke(var, DUOLCommon::StringHelper::ToTString(componentType.get_name().to_string()));

		// TODO : PhysX �� ������ �ִ� Component ���� üũ�ϰ� �б��ؾ��մϴ�. (�ݶ��̴��� �� ������Ʈ �� �� ���� ����, ������ �ٵ� ���� �� ���� ����)

		// MonoBehaviourBase
		if (componentType.is_derived_from(type::get_by_name("MonoBehaviourBase")))
		{
			auto& monoBehaviour = createdCom.get_wrapped_value<DUOLGameEngine::MonoBehaviourBase>();

			std::shared_ptr<MonoBehaviourBase> mono(const_cast<DUOLGameEngine::MonoBehaviourBase*>(&monoBehaviour));

			mono->OnAwake();

			mono->OnStart();

			mono->OnEnable();

			mono->AllProcessOnEnable();

			_abledMonoBehaviours.push_back(mono);

			_allComponents.push_back(mono.get());

			// ������Ʈ ī��Ʈ ä������ �̺�Ʈ �� !
			_componentCountChangedEvent.Invoke();

			return mono.get();
		}
		// BehaviourBase
		else if (componentType.is_derived_from(type::get_by_name("BehaviourBase")))
		{
			auto& behaviour = createdCom.get_wrapped_value<DUOLGameEngine::BehaviourBase>();

			std::shared_ptr<BehaviourBase> beha(const_cast<DUOLGameEngine::BehaviourBase*>(&behaviour));

			// ���� ��ü�� �ʱ�ȭ�մϴ�. 
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

			// ������Ʈ ī��Ʈ ä������ �̺�Ʈ �� !
			_componentCountChangedEvent.Invoke();

			return beha.get();
		}
		// ComponentBase
		else if (componentType.is_derived_from(type::get_by_name("ComponentBase")))
		{
			auto& component = createdCom.get_wrapped_value<DUOLGameEngine::ComponentBase>();

			std::shared_ptr<ComponentBase> com(const_cast<DUOLGameEngine::ComponentBase*>(&component));

			// ���� ��ü�� �ʱ�ȭ�մϴ�. ������� ComponentBase �ϴ��� Rigidbody ���Դϴ�.
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

			// ������Ʈ ī��Ʈ ä������ �̺�Ʈ �� !
			_componentCountChangedEvent.Invoke();

			return com.get();
		}

		return nullptr;
	}

	DUOLGameEngine::ComponentBase* GameObject::GetComponent(const DUOLCommon::tstring& componentName)
	{
		rttr::type targetType = rttr::type::get_by_name(DUOLCommon::StringHelper::ToString(componentName));

		for (auto& component : _allComponents)
		{
			// ���� Ÿ���̸� �ش� ������Ʈ�� �����մϴ�.
			if (component->get_type() == targetType)
				return component;
		}

		return nullptr;
	}

	void GameObject::OnCreate()
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

		if (_isActive)
		{
			// ABLE ������ ������Ʈ�鿡�Ը� �����մϴ�.
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

				// MonoBehaviourBase�� ���
				// Ŭ���̾�Ʈ���� ������ �̺�Ʈ���� ��� �� �� ���� �ֽ��ϴ�.
				abledMonoBehaviour->AllProcessOnEnable();
			}
			 
			_isStarted = true;
		}
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

		// ��������� �ڽ� ������Ʈ���� �ǽ��մϴ�.
		auto&& children = GetTransform()->GetChildGameObjects();

		// Awake�� Active / InActive ���� ���� �����մϴ�.
		for (auto& child : children)
			child->OnAwake();
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

			abledBehaviour->_isStarted = true;
		}

		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->OnStart();

			abledMonoBehaviour->_isStarted = true;

			// MonoBehaviourBase�� ���
			// Ŭ���̾�Ʈ���� ������ �̺�Ʈ���� ��� �� �� ���� �ֽ��ϴ�.
			// TODO : ���⼭ ���� �ʾƿ� ~!
			// abledMonoBehaviour->AllProcessOnEnable();
		}

		// ��������� �ڽ� ������Ʈ���� �ǽ��մϴ�.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
		{
			if (child->_isActive)
				child->OnStart();
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

			abledMonoBehaviour->AllProcessOnEnable();
		}

		// ��������� �ڽ� ������Ʈ���� �ǽ��մϴ�.
		auto&& children = GetTransform()->GetChildGameObjects();

		// TODO : �ڽ� ������Ʈ�� ���� Active ���� ����ϰ� �־�� �մϴ� ..?
		for (auto& child : children)
		{
			// ���� ���� ������ ������ ���� �ִ� ������Ʈ (_isActive) �� ������Ʈ�� �ٽ� ���ݴϴ�.
			if (child->_isActive)
				child->OnActive();
		}
	}

	void GameObject::OnInActive()
	{
		// ���� ���� �ִ� ������Ʈ�鿡 ���ؼ��� OnDisable() ȣ��
		for (const auto& abledBehaviour : _abledBehaviours)
		{
			abledBehaviour->OnDisable();
		}

		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->OnDisable();

			abledMonoBehaviour->AllProcessOnDisable();
		}

		// ��������� �ڽ� ������Ʈ���� �ǽ��մϴ�.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
		{
			// ���� ���� ���� �׳� �ڽ� ������Ʈ�鵵 ���� ��Ʈ���� �˴ϴ�.
			child->OnInActive();
		}
	}

	void GameObject::OnDestroy()
	{
		for (const auto& behaviour : _abledBehaviours)
		{
			behaviour->OnDisable();
		}

		for (const auto& monoBehaviour : _abledMonoBehaviours)
		{
			monoBehaviour->OnDisable();

			monoBehaviour->AllProcessOnDisable();
		}
		
		// ��������� �ڽ� ������Ʈ���� �ǽ��մϴ�.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
			child->OnDestroy();

		_destroyEventHandlers.Invoke();
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

		// ��������� �ڽ� ������Ʈ���� �ǽ��մϴ�.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
		{
			if (child->_isActive)
				child->OnUpdate(deltaTime);
		}
	}

	void GameObject::OnCoroutineUpdate(float deltaTime)
	{
		// Ȱ��ȭ�� MonoBehaviourBase �� ȣ�⿡ �ش�Ǵ� �Լ��Դϴ�.
		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->UpdateAllCoroutines(deltaTime);
		}

		// ��������� �ڽ� ������Ʈ���� �ǽ��մϴ�.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
		{
			if (child->_isActive)
				child->OnCoroutineUpdate(deltaTime);
		}
	}

	void GameObject::OnInvokeUpdate(float deltaTime)
	{
		// Ȱ��ȭ�� MonoBehaivourBase �� ȣ�⿡ �ش�Ǵ� �Լ��Դϴ�.
		for (const auto& abledMonoBehaviour : _abledMonoBehaviours)
		{
			abledMonoBehaviour->UpdateAllInvokes(deltaTime);
		}

		// ��������� �ڽ� ������Ʈ���� �ǽ��մϴ�.
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

		// ��������� �ڽ� ������Ʈ���� �ǽ��մϴ�.
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

		// ��������� �ڽ� ������Ʈ���� �ǽ��մϴ�.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
		{
			if (child->_isActive)
				child->OnLateUpdate(deltaTime);
		}
	}

	void GameObject::UpdateDestroyComponent(float deltaTime)
	{
		std::erase_if(_componentsForDestroy, [this, deltaTime](ComponentDestroyInfo& info)
			{
				info._lastTime -= deltaTime;

				if (info._lastTime <= 0.f)
				{
					// ���� ������ �����ϴ� ����Ʈ���� �ش��ϴ� �̺�Ʈ �Լ��� ȣ���ϰ� �����մϴ� !
					SearchAndDestroyComponent(info._targetComponent);

					_componentCountChangedEvent.Invoke();

					info._targetComponent.reset();

					return true;
				}
				else
					return false;
			});

		// ��������� �ڽ� ������Ʈ���� �ǽ��մϴ�.
		auto&& children = GetTransform()->GetChildGameObjects();

		for (auto& child : children)
			child->UpdateDestroyComponent(deltaTime);
	}

	void GameObject::SearchAndDestroyComponent(std::shared_ptr<DUOLGameEngine::ComponentBase> targetComponent)
	{
		std::erase_if(_allComponents, [this, targetComponent](DUOLGameEngine::ComponentBase* component)
			{
				return component == targetComponent.get() ? true : false;
			});

		std::erase_if(_components, [this, targetComponent](std::shared_ptr<DUOLGameEngine::ComponentBase> component)
			{
				if (targetComponent.get() == component.get())
				{
					component->OnDestroy();

					component.reset();

					return true;
				}
				else
					return false;
			});

		std::erase_if(_abledBehaviours, [this, targetComponent](std::shared_ptr<DUOLGameEngine::BehaviourBase> behaviour)
			{
				if (targetComponent.get() == behaviour.get())
				{
					behaviour->OnDisable();

					behaviour->OnDestroy();

					behaviour.reset();

					return true;
				}
				else
					return false;
			});

		std::erase_if(_disabledBehaviours, [this, targetComponent](std::shared_ptr<DUOLGameEngine::BehaviourBase> behaviour)
			{
				if (targetComponent.get() == behaviour.get())
				{
					behaviour->OnDestroy();

					behaviour.reset();

					return true;
				}
				else
					return false;
			});

		std::erase_if(_abledMonoBehaviours, [this, targetComponent](std::shared_ptr<DUOLGameEngine::MonoBehaviourBase> monoBehaviour)
			{
				if (targetComponent.get() == monoBehaviour.get())
				{
					monoBehaviour->OnDisable();

					monoBehaviour->AllProcessOnDisable();

					monoBehaviour->OnDestroy();

					monoBehaviour.reset();

					return true;
				}
				else
					return false;
			});

		std::erase_if(_disabledMonoBehaviours, [this, targetComponent](std::shared_ptr<DUOLGameEngine::MonoBehaviourBase> monoBehaviour)
			{
				if (targetComponent.get() == monoBehaviour.get())
				{
					monoBehaviour->OnDestroy();

					monoBehaviour.reset();

					return true;
				}
				else
					return false;
			});
	}

	Scene* GameObject::GetScene() const
	{
		return _scene;
	}

	void GameObject::SetIsActive(bool value)
	{
		if ((_scene == nullptr) || (value == _isActive))
			return;

		// ������ Register ���� ������ ���� �ƴ϶� ���� �����ӿ� Active List�� ������ Active�Ѵ�.
		value ? _scene->RegisterActiveGameObject(this) : _scene->RegisterInActiveGameObject(this);
	}

	void GameObject::RegisterDestroyComponent(DUOLGameEngine::ComponentBase* component, float time)
	{
#pragma region SEARCH_COMPONENT
		for (auto com : _components)
		{
			if (com.get() == component)

				_componentsForDestroy.push_back({ com, time });
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