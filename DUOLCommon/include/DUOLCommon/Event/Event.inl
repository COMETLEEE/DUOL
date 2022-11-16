#pragma once
#include "DUOLCommon/Event/Event.h"

namespace DUOLCommon
{
	template <typename ReturnType, typename ... Types>
	Event<ReturnType, Types...>::Event() :
		_listeners(std::unordered_map<EventHandlerID, EventListener>())
		, _nextListenerID(0)
	{
	}

	template <typename ReturnType, typename ... Types>
	Event<ReturnType, Types...>::~Event()
	{
		_listeners.clear();
	}

	template <typename ReturnType, typename ... Types>
	EventHandlerID Event<ReturnType, Types...>::AddListener(EventListener callback)
	{
		EventHandlerID listenerID = _nextListenerID++;

		_listeners.emplace(listenerID, callback);

		return listenerID;
	}

	template <typename ReturnType, typename ... Types>
	EventHandlerID Event<ReturnType, Types...>::operator+=(EventListener callback)
	{
		return AddListener(callback);
	}

	template <typename ReturnType, typename ... Types>
	bool Event<ReturnType, Types...>::RemoveListener(EventHandlerID listenerID)
	{
		return _listeners.erase(listenerID) != 0;
	}

	template <typename ReturnType, typename ... Types>
	bool Event<ReturnType, Types...>::operator-=(EventHandlerID listenerID)
	{
		return RemoveListener(listenerID);
	}

	template <typename ReturnType, typename ... Types>
	void Event<ReturnType, Types...>::RemoveAllListeners()
	{
		_listeners.clear();
	}

	template <typename ReturnType, typename ... Types>
	uint64_t Event<ReturnType, Types...>::GetListenerCount()
	{
		return _listeners.size();
	}

	template <typename ReturnType, typename ... Types>
	std::vector<ReturnType> Event<ReturnType, Types...>::Invoke(Types... args)
	{
		std::vector<ReturnType> ret;

		ret.reserve(_listeners.size());

		for (auto const& [key, value] : _listeners)
		{
			ReturnType val = value(args...);

			ret.push_back(val);
		}

		return ret;
	}

	template <typename ... Types>
	Event<void, Types...>::Event() :
		_listeners(std::unordered_map<EventHandlerID, EventListener>())
		, _nextListenerID(0)
	{

	}

	template <typename ... Types>
	Event<void, Types...>::~Event()
	{
		_listeners.clear();
	}

	template <typename ... Types>
	EventHandlerID Event<void, Types...>::AddListener(EventListener callback)
	{
		EventHandlerID listenerID = _nextListenerID++;

		_listeners.emplace(listenerID, callback);

		return listenerID;
	}

	template <typename ... Types>
	EventHandlerID Event<void, Types...>::operator+=(EventListener callback)
	{
		return this->AddListener(callback);
	}

	template <typename ... Types>
	bool Event<void, Types...>::RemoveListener(EventHandlerID listenerID)
	{
		return _listeners.erase(listenerID) != 0;
	}

	template <typename ... Types>
	bool Event<void, Types...>::operator-=(EventHandlerID listenerID)
	{
		return this->RemoveListener(listenerID);
	}

	template <typename ... Types>
	void Event<void, Types...>::RemoveAllListeners()
	{
		_listeners.clear();
	}

	template <typename ... Types>
	uint64_t Event<void, Types...>::GetListenerCount()
	{
		return _listeners.size();
	}

	template <typename ... Types>
	void Event<void, Types...>::Invoke(Types... args)
	{
		for (auto const& [key, value] : _listeners)
			value(args...);
	}
}