#pragma once
#include "DUOLCommon/Event/Event.h"

namespace DUOLCommon
{
	template <typename ReturnType, typename ... Types>
	Event<ReturnType, Types...>::Event() :
		_listeners(std::unordered_map<EventListenerID, EventListener>())
		, _nextListenerID(0)
	{
	}

	template <typename ReturnType, typename ... Types>
	Event<ReturnType, Types...>::~Event()
	{
		_listeners.clear();
	}

	template <typename ReturnType, typename ... Types>
	EventListenerID Event<ReturnType, Types...>::AddListener(EventListener callback)
	{
		EventListenerID listenerID = _nextListenerID++;

		_listeners.emplace(listenerID, callback);

		return listenerID;
	}

	template <typename ReturnType, typename ... Types>
	EventListenerID Event<ReturnType, Types...>::operator+=(EventListener callback)
	{
		return AddListener(callback);
	}

	template <typename ReturnType, typename ... Types>
	bool Event<ReturnType, Types...>::RemoveListener(EventListenerID listenerID)
	{
		return _listeners.erase(listenerID) != 0;
	}

	template <typename ReturnType, typename ... Types>
	bool Event<ReturnType, Types...>::operator-=(EventListenerID listenerID)
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
		_listeners(std::unordered_map<EventListenerID, EventListener>())
		, _nextListenerID(0)
	{

	}

	template <typename ... Types>
	Event<void, Types...>::~Event()
	{
		_listeners.clear();
	}

	template <typename ... Types>
	EventListenerID Event<void, Types...>::AddListener(EventListener callback)
	{
		EventListenerID listenerID = _nextListenerID++;

		_listeners.emplace(listenerID, callback);

		return listenerID;
	}

	template <typename ... Types>
	EventListenerID Event<void, Types...>::operator+=(EventListener callback)
	{
		return this->AddListener(callback);
	}

	template <typename ... Types>
	bool Event<void, Types...>::RemoveListener(EventListenerID listenerID)
	{
		return _listeners.erase(listenerID) != 0;
	}

	template <typename ... Types>
	bool Event<void, Types...>::operator-=(EventListenerID listenerID)
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