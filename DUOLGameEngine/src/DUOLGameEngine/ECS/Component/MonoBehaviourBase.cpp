#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

#include <iostream>

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

namespace DUOLGameEngine
{
	MonoBehaviourBase::MonoBehaviourBase(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, enable_shared_from_base<DUOLGameEngine::MonoBehaviourBase, DUOLGameEngine::BehaviourBase>()
		, _coroutineHandlers(std::list<CoroutineHandler>())
	{

	}

	MonoBehaviourBase::~MonoBehaviourBase()
	{

	}

	void MonoBehaviourBase::SetIsEnabled(bool value)
	{
		// 게임 오브젝트에게 MonoBehaviour에 맞는 동작을 실시시킨다.
		// 이펙티브 C++에서 Protected 도 왠만하면 쓰지 말라는데 ..
		if (value == _isEnabled)
			return;

		// 해당 컴포넌트의 Awake와 Start가 실행되지 않았습니다. 첫 번째 Enable(true) 라는 것
		if ((value) && (!_isStarted))
		{
			OnStart();

			_isStarted = true;
		}

		GameObject* gameObject = GetGameObject();

		if (gameObject != nullptr)
		{
			value == true ? gameObject->SetMonoBehaviourEnabled(shared_from_base())
				: gameObject->SetMonoBehaviourDisabled(shared_from_base());
		}

		// 값을 바꿉니다.
		_isEnabled = value;

		// isEnable 변수에 따른 Work 들을 수행한다.
		_isEnabled ? AllProcessOnEnable() : AllProcessOnDisable();
	}

	void MonoBehaviourBase::AddEventFunction(const DUOLCommon::tstring& eventName, std::function<void()> functor)
	{
		// DUOLCommon::EventListenerID id = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(eventName, functor);

		_eventFunctionsVoid.push_back({ eventName, 0, functor });
	}

	void MonoBehaviourBase::AllProcessOnEnable()
	{
		// Event handlers
		RegisterEventHandlers();

		// Collision Event
	}

	void MonoBehaviourBase::AllProcessOnDisable()
	{
		// Event handlers
		RemoveEventHandlers();

		// Collision Event
	}

	void MonoBehaviourBase::RegisterEventHandlers()
	{
		// OnFixedUpdate
		const std::function<void(float)> onFixedUpdate = std::bind(&MonoBehaviourBase::OnFixedUpdate, this, std::placeholders::_1);

		_fixedUpdateEventHandlerID = PhysicsManager::GetInstance()->AddFixedUpdateEventHandler(onFixedUpdate);

		// OnCollisionXXX & OnTriggerXXX
		const std::weak_ptr<DUOLPhysics::PhysicsActorBase> actor = GetGameObject()->_physicsActor;

		if (actor.lock() != nullptr)
		{
			const std::function<void(const std::shared_ptr<DUOLPhysics::Collision>&)> colEnter = std::bind(&MonoBehaviourBase::OnCollisionEnter,
				this, std::placeholders::_1);

			const std::function<void(const std::shared_ptr<DUOLPhysics::Collision>&)> colStay = std::bind(&MonoBehaviourBase::OnCollisionStay,
				this, std::placeholders::_1);

			const std::function<void(const std::shared_ptr<DUOLPhysics::Collision>&)> colExit = std::bind(&MonoBehaviourBase::OnCollisionExit,
				this, std::placeholders::_1);

			const std::function<void(const std::shared_ptr<DUOLPhysics::Trigger>&)> triEnter = std::bind(&MonoBehaviourBase::OnTriggerEnter,
				this, std::placeholders::_1);

			const std::function<void(const std::shared_ptr<DUOLPhysics::Trigger>&)> triStay = std::bind(&MonoBehaviourBase::OnTriggerStay,
				this, std::placeholders::_1);

			const std::function<void(const std::shared_ptr<DUOLPhysics::Trigger>&)> triExit = std::bind(&MonoBehaviourBase::OnTriggerExit,
				this, std::placeholders::_1);

			actor.lock()->SetCollisionEnterEvent(colEnter);
			actor.lock()->SetCollisionStayEvent(colStay);
			actor.lock()->SetCollisionExitEvent(colExit);

			actor.lock()->SetTriggerEnterEvent(triEnter);
			actor.lock()->SetTriggerStayEvent(triStay);
			actor.lock()->SetTriggerExitEvent(triExit);
		}

		// In game, EventManager::AddEventFunctions(...)
		for (auto& value : _eventFunctionsVoid)
			std::get<1>(value) = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(std::get<0>(value), std::get<2>(value));

		for (auto& value : _eventFunctionsBool)
			std::get<1>(value) = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(std::get<0>(value), std::get<2>(value));

		for (auto& value : _eventFunctionsInt)
			std::get<1>(value) = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(std::get<0>(value), std::get<2>(value));

		for (auto& value : _eventFunctionsFloat)
			std::get<1>(value) = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(std::get<0>(value), std::get<2>(value));

		for (auto& value : _eventFunctionsTString)
			std::get<1>(value) = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction<const DUOLCommon::tstring&>(std::get<0>(value), std::get<2>(value));
	}

	void MonoBehaviourBase::RemoveEventHandlers()
	{
		// OnFixedUpdate
		PhysicsManager::GetInstance()->RemoveFixedUpdateEventHandler(_fixedUpdateEventHandlerID);

		// OnCollisionXXX & OnTriggerXXX
		const std::weak_ptr<DUOLPhysics::PhysicsActorBase> actor = GetGameObject()->_physicsActor;

		if (actor.lock() != nullptr)
		{
			actor.lock()->SetCollisionEnterEvent(nullptr);
			actor.lock()->SetCollisionStayEvent(nullptr);
			actor.lock()->SetCollisionExitEvent(nullptr);

			actor.lock()->SetTriggerEnterEvent(nullptr);
			actor.lock()->SetTriggerStayEvent(nullptr);
			actor.lock()->SetTriggerExitEvent(nullptr);
		}

		// In game, EventManager::AddEventFunctions(...)
		for (auto& value : _eventFunctionsVoid)
			DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(std::get<0>(value), std::get<1>(value));

		for (auto& value : _eventFunctionsBool)
			DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<bool>(std::get<0>(value), std::get<1>(value));

		for (auto& value : _eventFunctionsInt)
			DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<int>(std::get<0>(value), std::get<1>(value));

		for (auto& value : _eventFunctionsFloat)
			DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<float>(std::get<0>(value), std::get<1>(value));

		for (auto& value : _eventFunctionsTString)
			DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<const DUOLCommon::tstring&>(std::get<0>(value), std::get<1>(value));
	}

	void MonoBehaviourBase::StopCoroutine(const std::shared_ptr<Coroutine>& coroutine)
	{
		for (auto iter = _coroutineHandlers.begin() ; iter != _coroutineHandlers.end() ; )
		{
			// 같은 코루틴 핸들이라면 .. 같습니다.
			// 해당 코루틴을 소멸시킵니다.
			if (iter->CompareCoroutineHandle(coroutine->GetCoroutineHandler()))
				_coroutineHandlers.erase(iter++);
			else
				iter++;
		}
	}

	void MonoBehaviourBase::StopAllCoroutines()
	{
		for (auto iter = _coroutineHandlers.begin() ; iter != _coroutineHandlers.end() ; iter++)
		{
			iter->UnInitialize();
		}

		_coroutineHandlers.clear();
	}

	void MonoBehaviourBase::UpdateAllCoroutines(float deltaTime)
	{
		for (auto iter = _coroutineHandlers.begin() ; iter != _coroutineHandlers.end() ; )
		{
			iter->UpdateCoroutine(deltaTime);

			if (iter->IsDone())
			{
				iter->UnInitialize();

				_coroutineHandlers.erase(iter++);

				continue;
			}

			if (iter->CanResume())
				iter->Resume();

			iter++;
		}
	}

	void MonoBehaviourBase::Invoke(void(* func)(), float time)
	{
		_invokeReservedFunctions.push_back({ func, time });
	}

	void MonoBehaviourBase::CancleAllInvokes()
	{
		_invokeThisFrameFunctions.clear();

		_invokeReservedFunctions.clear();
	}

	void MonoBehaviourBase::CancleInvoke(void(* func)())
	{
		std::function<void()> functor = func;

		std::erase_if(_invokeReservedFunctions, [func](const std::pair<std::function<void()>, float>& elem)
			{
				return *elem.first.target<void(*)(void)>() == func
					? true : false;
			});

		std::erase_if(_invokeThisFrameFunctions, [func](const std::function<void()>& elem)
			{
				return *elem.target<void(*)(void)>() == func
					? true : false;
			});
	}

	void MonoBehaviourBase::UpdateAllInvokes(float deltaTime)
	{
		// 예약되었던 함수들 중 제약 시간이 모두 지난 함수들을 호출합니다.
		for (auto iter = _invokeThisFrameFunctions.begin() ; iter != _invokeThisFrameFunctions.end() ; )
		{
			// Invoke !
			(*(iter++))();
		}

		_invokeThisFrameFunctions.clear();

		// Invoke Reserved Time Update
		for (auto iter = _invokeReservedFunctions.begin() ; iter != _invokeReservedFunctions.end() ; )
		{
			iter->second -= deltaTime;

			// 시간이 다 되었으니 다음 프레임에 호출시킵니다.
			if (iter->second < 0.f)
			{
				_invokeThisFrameFunctions.push_back(iter->first);

				_invokeReservedFunctions.erase(iter++);

				continue;
			}
			else
				iter++;
		}
	}
}