/**

    @file      MonoBehaviourBase.h
    @brief     MonoBehaviour is a base class that many DUOL Scripts derive from.
    @details   ~
    @author    COMETLEE
    @date      25.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <functional>

#include "DUOLGameEngine/Manager/EventManager.h"

#include "DUOLCommon/Event/Event.h"
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGameEngine/Util/Coroutine/Coroutine.h"

#include "DUOLGameEngine/Util/enabled_shared_from_base.h"

namespace DUOLPhysics
{
	struct Collision;

	struct Trigger;
}

namespace DUOLGameEngine
{
	class ColliderBase;

	/**
	 * \brief 스크립트 (=> 커스텀 컴포넌트) 의 기본 클래스입니다.
	 * 스크립트는 해당 게임 오브젝트의 생애 동안 조작을 담당합니다.
	 */
	class DUOL_GAMEENGINE_API MonoBehaviourBase : public DUOLGameEngine::BehaviourBase,
		public DUOLGameEngine::enable_shared_from_base<DUOLGameEngine::MonoBehaviourBase, DUOLGameEngine::BehaviourBase>
	{
	public:
		MonoBehaviourBase(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("MonoBehaviour"));

		virtual ~MonoBehaviourBase() override;
	
		DEFINE_DEFAULT_COPY_MOVE(MonoBehaviourBase)

		virtual void SetIsEnabled(bool value) override final;

		virtual void OnCollisionEnter(const std::shared_ptr<DUOLPhysics::Collision>& collision) {}

		virtual void OnCollisionStay(const std::shared_ptr<DUOLPhysics::Collision>& collision) {}

		virtual void OnCollisionExit(const std::shared_ptr<DUOLPhysics::Collision>& collision) {}

		virtual void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) {}

		virtual void OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) {}

		virtual void OnTriggerExit(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) {}

		void AddEventFunction(const DUOLCommon::tstring& eventName, std::function<void(void)> functor);

		template <typename TParam>
		void AddEventFunction(const DUOLCommon::tstring& eventName, std::function<void(TParam)> functor);

	private:
		// MonoBehaviourBase를 상속받은 C++ 컨텐츠 코드에서 등록한 이벤트 함수들을 물고 있다가 Enable, Disable 상황 시에 On / Off 자동화합니다.
		std::unordered_map<DUOLCommon::tstring, std::function<void(void)>>												_eventFunctionsVoid;

		std::unordered_map<DUOLCommon::tstring, std::function<void(bool)>>												_eventFunctionsBool;

		std::unordered_map<DUOLCommon::tstring, std::function<void(int)>>												_eventFunctionsInt;

		std::unordered_map<DUOLCommon::tstring, std::function<void(float)>>												_eventFunctionsFloat;

		std::unordered_map<DUOLCommon::tstring, std::function<void(const DUOLCommon::tstring&)>>						_eventFunctionsTString;
			
	private:
		void InvokeEvent(const DUOLCommon::tstring& eventName);

		template <typename TParam>
		void InvokeEvent(const DUOLCommon::tstring& eventName, TParam parameter);

		/**
		 * \brief 해당 MonoBehaviour가 활성화되었을 때 일어나는 일들의 모음집
		 */
		void AllProcessOnEnable();

		/**
		 * \brief 해당 MonoBehaviourBase가 비활성화 되었을 때 일어나는 일들의 모음집
		 */
		void AllProcessOnDisable();

		/**
		 * \brief 해당 MonoBehaviour가 활성화되면 이벤트 핸들러를 각 매니저들에게 등록합니다.
		 */
		void RegisterEventHandlers();

		/**
		 * \brief 해당 MonoBehaviour가 비활성화되면 이벤트 핸들러를 각 매니저들에게서 제거합니다.
		 */
		void RemoveEventHandlers();

		/**
		 * \brief PhysicsManager에게 던진 FixedUpdate EventHandlerID
		 */
		DUOLCommon::EventListenerID _fixedUpdateEventHandlerID;

#pragma region COROUTINE
	protected:
		/**
		 * \brief 흐름의 형태로 남아있는 코루틴 함수들의 리스트
		 */
		std::list<DUOLGameEngine::CoroutineHandler> _coroutineHandlers;

		/**
		 * \brief 전역 함수를 대상으로 코루틴을 시작합니다.
		 * \tparam Types 코루틴 함수가 받는 가변 템플릿
		 * \param routine 시작할 코루틴 함수
		 * \param args 코루틴 함수가 받는 매개변수
		 * \return 해당 코루틴 핸들 Wrapper
		 */
		template <typename ...Types>
		std::shared_ptr<Coroutine> StartCoroutine(DUOLGameEngine::CoroutineHandler(*routine)(Types...), Types... args);

		/**
		 * \brief 멤버 함수를 대상으로 코루틴을 시작합니다.
		 * \tparam TDerivedClass MonoBehaviour를 상속한 Script Class
		 * \tparam Types 코루틴 함수가 받는 가변 템플릿
		 * \param object 해당 코루틴 함수를 실행할 오브젝트 (== *this)
		 * \param routine 시작할 코루틴 함수
		 * \param args 코루틴 함수가 받는 매개변수
		 * \return 해당 코루틴 핸들 Wrapper
		 */
		template <typename TDerivedClass, typename ...Types>
		std::shared_ptr<Coroutine> StartCoroutine(DUOLGameEngine::CoroutineHandler(TDerivedClass::*routine)(Types...), Types... args)
			requires std::derived_from<TDerivedClass, MonoBehaviourBase>;

		void StopCoroutine(const std::shared_ptr<DUOLGameEngine::Coroutine>& coroutine);

		// 함수의 비교는 아직 힘들다 .. => 리플렉션 ..? 컴파일할 때 코루틴 함수들을 전역 테이블에 등록 ..?
		// => 핸들 래퍼에 함수 주소를 저장 ..?
		/*template <typename ...Types>
		void StopCoroutine(CoroutineHandler(*routine)(Types...));*/

		void StopAllCoroutines();

	private:
		void UpdateAllCoroutines(float deltaTime);

#pragma endregion

#pragma region INVOKE
	protected:
		std::list<std::pair<std::function<void()>, float>> _invokeReservedFunctions;

		std::list<std::function<void()>> _invokeThisFrameFunctions;

		/**
		 * \brief 파라미터로 받은 시간이 지난 이후 첫 번째 프레임에 해당 함수를 호출합니다. (전역 함수)
		 * \param invoke 호출하고 싶은 함수의 포인터
		 * \param time 이 시간 이후 호출합니다.
		 */
		void Invoke(void(*func)(), float time);

		/**
		 * \brief 파라미터로 받은 시간이 지난 이후 첫 번째 프레임에 해당 함수를 호출합니다. (멤버 함수)
		 * \tparam TDerivedClass MonoBehaviourBase를 상속 받은 클래스. 해당 클래스 내에 정의된 멤버 함수를 호출하기 위함.
		 * \param invoke 호출하고 싶은 멤버 함수의 포인터
		 * \param time 이 시간 이후 호출합니다.
		 */
		template <typename TDerivedClass>
		void Invoke(void(TDerivedClass::* func)(), float time)
			requires std::derived_from<TDerivedClass, MonoBehaviourBase>;

		/**
		 * \brief Cancel all Invoke calls on this MonoBehaviourBase.
		 */
		void CancleAllInvokes();

		/**
		 * \brief Invoke 요청된 함수의 예약을 취소합니다.
		 * \param func 취소를 원하는 Invoke 요청된 함수의 포인터
		 */
		void CancleInvoke(void(*func)());

		///**
		// * \brief Invoke 요청된 멤버 함수의 예약을 취소합니다.
		// * \tparam TDerivedClass MonoBehaviourBase를 상속받은 클래스.
		// * \param func 취소를 원하는 Invoke 요청된 멤버 함수의 포인터 
		// */
		//template <typename TDerivedClass>
		//void CancleInvoke(void(TDerivedClass::* func)());

	private:
		void UpdateAllInvokes(float deltaTime);
#pragma endregion

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

#pragma region FRIEND_CLASS
		friend class GameObject;
#pragma endregion
	};

#pragma region COROUTINE_IMPL
	template <typename ...Types>
	std::shared_ptr<Coroutine> MonoBehaviourBase::StartCoroutine(DUOLGameEngine::CoroutineHandler(*routine)(Types...), Types... args)
	{
		// 루틴 시작하면서 반환되는 코루틴 핸들러의 복사본을 리스트에 넣는다.
		_coroutineHandlers.push_back(routine(args...));

		// 해당 코루틴 핸들러를 Wrapping 하는 코루틴 객체를 만들어서 반환한다. (For Coroutine Chain)
		return std::make_shared<Coroutine>(_coroutineHandlers.back());
	}

	template <typename TDerivedClass, typename ...Types>
	std::shared_ptr<Coroutine> MonoBehaviourBase::StartCoroutine(DUOLGameEngine::CoroutineHandler(TDerivedClass::*routine)(Types...), Types... args)
		requires std::derived_from<TDerivedClass, MonoBehaviourBase>
	{
		_coroutineHandlers.push_back((static_cast<TDerivedClass*>(this)->*routine)(args...));

		return std::make_shared<Coroutine>(_coroutineHandlers.back());
	}
#pragma endregion

#pragma region INVOKE_IMPL
	template <typename TDerivedClass>
	void MonoBehaviourBase::Invoke(void(TDerivedClass::* func)(), float time)
		requires std::derived_from<TDerivedClass, MonoBehaviourBase>
	{
		std::function<void()> functor = std::bind(func, static_cast<TDerivedClass*>(this));

		_invokeReservedFunctions.push_back({ functor, std::max<float>(time, 0.f) });
	}

	/*template <typename TDerivedClass>
	void MonoBehaviourBase::CancleInvoke(void(TDerivedClass::* func)())
	{
		std::function<void()> functor = std::bind(func, static_cast<TDerivedClass*>(this));

		std::erase_if(_invokeReservedFunctions, [functor](const std::pair<std::function<void()>, float>& elem)
			{
				return elem.first._target<void()>()  == functor._target<void()>()
					? true : false;
			});

		std::erase_if(_invokeThisFrameFunctions, [functor](const std::function<void()>& elem)
			{
				return elem._target<void()>() == functor._target<void()>()
					? true : false;
			});
	}*/
#pragma endregion

	template <typename TParam>
	void MonoBehaviourBase::AddEventFunction(const DUOLCommon::tstring& eventName, std::function<void(TParam)> functor)
	{
		static_assert(std::is_same_v<bool, TParam> || std::is_same_v<int, TParam> || std::is_same_v<float, TParam>
			|| std::is_same_v<const DUOLCommon::tstring&, TParam>, "That type is not supported.");

		if constexpr (std::is_same_v<bool, TParam>)
		{
			_eventFunctionsBool.insert({ eventName, functor });
		}
		else if constexpr (std::is_same_v<int, TParam>)
		{
			_eventFunctionsInt.insert({ eventName, functor });
		}
		else if constexpr (std::is_same_v<float, TParam>)
		{
			_eventFunctionsFloat.insert({ eventName, functor });
		}
		else if constexpr (std::is_same_v<const DUOLCommon::tstring&, TParam>)
		{
			_eventFunctionsTString.insert({ eventName, functor });
		}
	}

	template <typename TParam>
	void MonoBehaviourBase::InvokeEvent(const DUOLCommon::tstring& eventName, TParam parameter)
	{
		static_assert(std::is_same_v<bool, TParam> || std::is_same_v<int, TParam> || std::is_same_v<float, TParam>
			|| std::is_same_v<const DUOLCommon::tstring&, TParam>, "That type is not supported.");

		if constexpr (std::is_same_v<bool, TParam>)
		{
			if (_eventFunctionsBool.contains(eventName))
				_eventFunctionsBool.at(eventName)(parameter);
		}
		else if constexpr (std::is_same_v<int, TParam>)
		{
			if (_eventFunctionsInt.contains(eventName))
				_eventFunctionsInt.at(eventName)(parameter);
		}
		else if constexpr (std::is_same_v<float, TParam>)
		{
			if (_eventFunctionsFloat.contains(eventName))
				_eventFunctionsFloat.at(eventName)(parameter);
		}
		else if constexpr (std::is_same_v<const DUOLCommon::tstring&, TParam>)
		{
			if (_eventFunctionsTString.contains(eventName))
				_eventFunctionsTString.at(eventName)(parameter);
		}
	}
}