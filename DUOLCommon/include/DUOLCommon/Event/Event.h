/**

    @file      Event.h
    @brief     Simple Event class.
    @details   ~
    @author    COMETLEE
    @date      2.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <functional>

namespace DUOLCommon
{
	/**
	 * \brief The ID of a listener (Registered callback in Event)
	 * This value is needed to remove a listener from an Event.
	 */
	using EventListenerID = uint64_t;

	/**
	 * \brief 이벤트 리스너들을 물고 있는 이벤트. Invoke 함수를 통해서 이벤트 리스너들이 진행된다.
	 * \tparam ArgTypes 이벤트가 받는 파라미터. 이벤트 리스너들에게 전달된다.
	 */
	template <typename ReturnType, typename ...Types>
    class Event final
	{
		using EventListener = std::function<void(Types...)>;

	public:
		Event();

		~Event();

		Event(const Event&) = default;

		Event& operator=(const Event&) = default;

		Event(Event&&) = default;

		Event& operator=(Event&&) = default;

		EventListenerID AddListener(EventListener callback);

		EventListenerID operator+=(EventListener callback);

		bool RemoveListener(EventListenerID listenerID);

		bool operator-=(EventListenerID listenerID);

		void RemoveAllListeners();

		uint64_t GetListenerCount();
		
		std::vector<ReturnType> Invoke(Types... args);

	private:
		std::unordered_map<EventListenerID, EventListener> _listeners;

		EventListenerID _nextListenerID;
	};

	/**
	 * \brief 리턴 타입이 void인 Event (가장 많이 쓰일 예정이겠지만 ..)의 템플릿 특수화 Ver.
	 * \tparam ArgTypes 이벤트가 받는 파라미터. 이벤트 리스너들에게 전달된다.
	 */
	template <typename ...Types>
	class Event<void, Types...> final
	{
		using EventListener = std::function<void(Types...)>;

	public:
		Event();

		~Event();

		Event(const Event&) = default;

		Event& operator=(const Event&) = default;

		Event(Event&&) = default;

		Event& operator=(Event&&) = default;

		EventListenerID AddListener(EventListener callback);

		EventListenerID operator+=(EventListener callback);

		bool RemoveListener(EventListenerID listenerID);

		bool operator-=(EventListenerID listenerID);

		void RemoveAllListeners();

		uint64_t GetListenerCount();

		void Invoke(Types... args);

	private:
		std::unordered_map<EventListenerID, EventListener> _listeners;

		EventListenerID _nextListenerID;
	};
}

#include "DUOLCommon/Event/Event.inl"