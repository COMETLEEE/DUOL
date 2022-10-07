#pragma once
#include <memory>
#include <vector>

#include "DUOLGameEngine/Event/Functor.h"

namespace DUOLGameEngine
{
	template <typename ReturnType, typename... ArgTypes>
	class EventManager
	{
	public:
		using EventType = Functor<ReturnType, ArgTypes...>;
		using Event = std::shader_ptr<EventType>;

	private:
		std::vector<std::weak_ptr<EventType>> _events;

	public:
		EventManager() = default;

		~EventManager() = default;

		Event AddEvent(EventType func);

		// 등록된 모든 이벤트 실행
		void Dispatch(ArgTypes... args);
	};

	template<typename ReturnType, typename ...ArgTypes>
	inline EventManager<ReturnType, ArgTypes...>::Event EventManager<ReturnType, ArgTypes...>::AddEvent(EventType func)
	{
		Event newEvent = std::make_shared<EventType>(func);

		_events.push_back(newEvent);

		return newEvent;
	}

	template<typename ReturnType, typename ...ArgTypes>
	inline void EventManager<ReturnType, ArgTypes...>::Dispatch(ArgTypes... args)
	{
		for (int i = 0; i < _events.size();)
		{
			if (_events[i].expired() == true)
				_events.erase(_events.begin() + i);
			else
				(*_events[i++])(args...);
		}
	}
}