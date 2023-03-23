#include "DUOLClient/Util/EventListenerIDHolder.h"


namespace DUOLClient
{
	// 템플릿 클래스입니다.
	void EventListenerIDHolder<void>::AddEventFunction(const DUOLCommon::tstring& eventName,
		std::function<void()> eventListener)
	{
		_eventListenerIDs.push_back({ eventName,
		DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(eventName, eventListener) });
	}

	void EventListenerIDHolder<void>::Refresh()
	{
		for (auto [eventName, id] : _eventListenerIDs)
			DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(eventName, id);

		_eventListenerIDs.clear();
	}
}