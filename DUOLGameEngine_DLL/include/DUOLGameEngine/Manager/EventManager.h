#pragma once
#include "DUOLCommon/Event/Event.h"
#include <queue>

#include <any>

#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
	enum class EventParameterType
	{
		Void
		, Bool
		, Float
		, Int
		, TString
		, Any
	};

	struct EventParameter
	{
		EventParameterType	_parameterType;

		std::any			_parameter;
	};

	struct EventMessage
	{
		DUOLCommon::tstring _eventName;

		EventParameter _eventParameter;
	};

	/**
	 * \brief DUOLGameEngine과 관련된 콜백 이벤트들을 담당합니다. 클라이언트 게임 코드에서도 사용할 수 있습니다.
	 */
	class DUOL_GAMEENGINE_API EventManager : public DUOLGameEngine::SingletonBase<EventManager>
	{
		DECLARE_SINGLETON(EventManager)

		DELETE_COPY_MOVE(EventManager)

	private:
		virtual ~EventManager() override;

	private:
		/**
		 * \brief 매개 변수를 받지 않는 이벤트들의 목록
		 */
		std::unordered_map<DUOLCommon::tstring, DUOLCommon::Event<void>>								_eventsVoid;

		/**
		 * \brief 'bool' 매개 변수를 받는 이벤트들의 목록
		 */
		std::unordered_map<DUOLCommon::tstring, DUOLCommon::Event<void, bool>>							_eventsBool;

		/**
		 * \brief 'int' 매개 변수를 받는 이벤트들의 목록
		 */
		std::unordered_map<DUOLCommon::tstring, DUOLCommon::Event<void, int>>							_eventsInt;

		/**
		 * \brief 'float' 매개 변수를 받는 이벤트들의 목록
		 */
		std::unordered_map<DUOLCommon::tstring, DUOLCommon::Event<void, float>>							_eventsFloat;

		/**
		 * \brief 'tstring' 매개 변수를 받는 이벤트들의 목록
		 */
		std::unordered_map<DUOLCommon::tstring, DUOLCommon::Event<void, const DUOLCommon::tstring&>>	_eventsTString;

		/**
		 * \brief 'std::any' 매개 변수를 받는 이벤트들의 목록
		 */
		std::unordered_map<DUOLCommon::tstring, DUOLCommon::Event<void, std::any>>						_eventsAny;

		/**
		 * \brief 함수의 호출 요청과 호출 시점을 분리하기 위해서 중간 매개자로 Queue를 둡니다.
		 */
		std::vector<DUOLGameEngine::EventMessage> _eventMessages;

	private:
		/**
		 * \brief DUOLGameEngine 여러 곳에서 참조하는 공통적인 이벤트들을 등록합니다.
		 */
		void Initialize();

		void UnInitialize();

		void DispatchEventMessages();

		void Update();

	public:
		template <typename TParam>
		void RegisterEvent(const DUOLCommon::tstring& eventName);

		template <typename TParam>
		DUOLCommon::EventListenerID AddEventFunction(const DUOLCommon::tstring& eventName,
			std::function<void(TParam)> functor);

		DUOLCommon::EventListenerID AddEventFunction(const DUOLCommon::tstring& eventName,
		                                             std::function<void(void)> functor);

		template <typename TParam>
		bool RemoveEventFunction(const DUOLCommon::tstring& eventName, DUOLCommon::EventListenerID id);

		template <typename TParam>
		void SendEventMessage(const DUOLCommon::tstring& eventName, TParam eventParameter);

		void SendEventMessage(const DUOLCommon::tstring& eventName);

		template <typename TParam>
		void InvokeEvent(const DUOLCommon::tstring& eventName, TParam eventParameter);

		void InvokeEvent(const DUOLCommon::tstring& eventName);

#pragma region FRIEND_CLASS
		friend class Engine;
#pragma endregion
	};

	template <typename TParam>
	void EventManager::InvokeEvent(const DUOLCommon::tstring& eventName, TParam eventParameter)
	{
		static_assert(std::is_same_v<bool, TParam> || std::is_same_v<int, TParam> || 
			std::is_same_v<float, TParam> || std::is_same_v<const DUOLCommon::tstring&, TParam> || std::is_same_v<std::any, TParam>, "That type is not supported.");

		if constexpr (std::is_same_v<TParam, bool>)
		{
			if (_eventsBool.contains(eventName))
				_eventsBool.at(eventName).Invoke(eventParameter);
		}
		else if constexpr (std::is_same_v<TParam, int>)
		{
			if (_eventsInt.contains(eventName))
				_eventsInt.at(eventName).Invoke(eventParameter);
		}
		else if constexpr (std::is_same_v<TParam, float>)
		{
			if (_eventsFloat.contains(eventName))
				_eventsFloat.at(eventName).Invoke(eventParameter);
		}
		else if constexpr (std::is_same_v<TParam, const DUOLCommon::tstring&>)
		{
			if (_eventsTString.contains(eventName))
				_eventsTString.at(eventName).Invoke(eventParameter);
		}
		else if constexpr (std::is_same_v<TParam, std::any>)
		{
			if (_eventsAny.contains(eventName))
				_eventsAny.at(eventName).Invoke(eventParameter);
		}
	}

	template <typename TParam>
	void EventManager::RegisterEvent(const DUOLCommon::tstring& eventName)
	{
		static_assert(std::is_same_v<void, TParam> || std::is_same_v<bool, TParam> || std::is_same_v<int, TParam>
			|| std::is_same_v<float, TParam> || std::is_same_v<const DUOLCommon::tstring&, TParam> || std::is_same_v<std::any, TParam>, "That type isn't supported.");

		if constexpr (std::is_same_v<void, TParam>)
		{
			if (!_eventsVoid.contains(eventName))
				_eventsVoid.insert({ eventName, {} });
		}
		else if constexpr (std::is_same_v<bool, TParam>)
		{
			if (!_eventsBool.contains(eventName))
				_eventsBool.insert({ eventName, {} });
		}
		else if constexpr (std::is_same_v<int, TParam>)
		{
			if (!_eventsInt.contains(eventName))
				_eventsInt.insert({ eventName, {} });
		}
		else if constexpr (std::is_same_v<float, TParam>)
		{
			if (!_eventsFloat.contains(eventName))
				_eventsFloat.insert({ eventName, {} });
		}
		else if constexpr (std::is_same_v<const DUOLCommon::tstring&, TParam>)
		{
			if (!_eventsTString.contains(eventName))
				_eventsTString.insert({ eventName, {} });
		}
		else if constexpr (std::is_same_v<std::any, TParam>)
		{
			if (!_eventsAny.contains(eventName))
				_eventsAny.insert({ eventName, {} });
		}
	}

	template <typename TParam>
	DUOLCommon::EventListenerID EventManager::AddEventFunction(const DUOLCommon::tstring& eventName, std::function<void(TParam)> functor)
	{
		static_assert(std::is_same_v<bool, TParam> || std::is_same_v<int, TParam> || std::is_same_v<float, TParam>
			|| std::is_same_v<const DUOLCommon::tstring&, TParam> || std::is_same_v<std::any, TParam>, "That type is not supported.");

		if constexpr (std::is_same_v<bool, TParam>)
		{
			if (_eventsBool.contains(eventName))
				return _eventsBool.at(eventName).AddListener(functor);
			else
			{
				_eventsBool.insert({ eventName, {} });

				return _eventsBool.at(eventName).AddListener(functor);
			}
		}
		else if constexpr (std::is_same_v<int, TParam>)
		{
			if (_eventsInt.contains(eventName))
				return _eventsInt.at(eventName).AddListener(functor);
			else
			{
				_eventsInt.insert({ eventName, {} });

				return _eventsInt.at(eventName).AddListener(functor);
			}
		}
		else if constexpr (std::is_same_v<float, TParam>)
		{
			if (_eventsFloat.contains(eventName))
				return _eventsFloat.at(eventName).AddListener(functor);
			else
			{
				_eventsFloat.insert({ eventName, {} });

				return _eventsFloat.at(eventName).AddListener(functor);
			}
		}
		else if constexpr (std::is_same_v<const DUOLCommon::tstring&, TParam>)
		{
			if (_eventsTString.contains(eventName))
				return _eventsTString.at(eventName).AddListener(functor);
			else
			{
				_eventsTString.insert({ eventName, {} });

				return _eventsTString.at(eventName).AddListener(functor);
			}
		}
		else if constexpr (std::is_same_v<std::any, TParam>)
		{
			if (_eventsAny.contains(eventName))
				return _eventsAny.at(eventName).AddListener(functor);
			else
			{
				_eventsAny.insert({ eventName, {} });

				return _eventsAny.at(eventName).AddListener(functor);
			}
		}
		else
			return false;
	}

	template <typename TParam>
	bool EventManager::RemoveEventFunction(const DUOLCommon::tstring& eventName, DUOLCommon::EventListenerID id)
	{
		static_assert(std::is_same_v<void, TParam> || std::is_same_v<bool, TParam> || std::is_same_v<int, TParam> 
			|| std::is_same_v<TParam, float> || std::is_same_v<TParam, const DUOLCommon::tstring&> || std::is_same_v<std::any, TParam>, "That type is not supported.");

		if constexpr (std::is_same_v<void, TParam>)
			return _eventsVoid.contains(eventName) ? _eventsVoid.at(eventName).RemoveListener(id) : false;
		else if constexpr (std::is_same_v<bool, TParam>)
			return _eventsBool.contains(eventName) ? _eventsBool.at(eventName).RemoveListener(id) : false;
		else if constexpr (std::is_same_v<float, TParam>)
			return _eventsFloat.contains(eventName) ? _eventsFloat.at(eventName).RemoveListener(id) : false;
		else if constexpr (std::is_same_v<int, TParam>)
			return _eventsInt.contains(eventName) ? _eventsInt.at(eventName).RemoveListener(id) : false;
		else if constexpr (std::is_same_v<const DUOLCommon::tstring&, TParam>)
			return _eventsTString.contains(eventName) ? _eventsTString.at(eventName).RemoveListener(id) : false;
		else if constexpr (std::is_same_v<std::any, TParam>)
			return _eventsAny.contains(eventName) ? _eventsAny.at(eventName).RemoveListener(id) : false;
		else
			return false;
	}

	template <typename TParam>
	void EventManager::SendEventMessage(const DUOLCommon::tstring& eventName, TParam eventParameter)
	{
		static_assert(std::is_same_v<bool, TParam> || std::is_same_v<int, TParam> || std::is_same_v<float, TParam>
			|| std::is_same_v< const DUOLCommon::tstring&, TParam> || std::is_same_v<std::any, TParam>, "That type is not supported.");

		if constexpr (std::is_same_v<bool, TParam>)
		{
			_eventMessages.push_back({ eventName, {EventParameterType::Bool, {eventParameter}} });
		}
		else if constexpr (std::is_same_v<int, TParam>)
		{
			_eventMessages.push_back({ eventName, {EventParameterType::Int, {eventParameter}} });
		}
		else if constexpr (std::is_same_v<float, TParam>)
		{
			_eventMessages.push_back({ eventName, {EventParameterType::Float, {eventParameter}} });
		}
		else if constexpr (std::is_same_v<const DUOLCommon::tstring&, TParam>)
		{
			_eventMessages.push_back({ eventName, {EventParameterType::TString, {eventParameter}} });
		}
		else if constexpr (std::is_same_v<std::any, TParam>)
		{
			_eventMessages.push_back({ eventName, {EventParameterType::Any, {eventParameter}} });
		}
	}
}
