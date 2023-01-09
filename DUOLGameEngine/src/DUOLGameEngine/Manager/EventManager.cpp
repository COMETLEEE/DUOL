#include "DUOLGameEngine/Manager/EventManager.h"

namespace DUOLGameEngine
{
	EventManager::EventManager() :
		_eventsVoid {}
		, _eventsBool {}
		, _eventsInt {}
		, _eventsFloat {}
		, _eventsTString {}
		, _eventMessages {}
	{
		
	}

	EventManager::~EventManager()
	{
	}

	void EventManager::InvokeEvent(const DUOLCommon::tstring& eventName)
	{
		if (_eventsVoid.contains(eventName))
			_eventsVoid.at(eventName).Invoke();
	}

	DUOLCommon::EventListenerID EventManager::AddEventFunction(const DUOLCommon::tstring& eventName,
		std::function<void(void)> functor)
	{
		if (_eventsVoid.contains(eventName))
			return _eventsVoid.at(eventName).AddListener(functor);
		else
		{
			_eventsVoid.insert({ eventName, {} });

			return _eventsVoid.at(eventName).AddListener(functor);
		}
	}

	void EventManager::SendEventMessage(const DUOLCommon::tstring& eventName)
	{
		_eventMessages.push_back({ eventName, {EventParameterType::Void, {}} });
	}

	void EventManager::Update()
	{
		DispatchEventMessages();
	}

	void EventManager::Initialize()
	{
		// 음 .. 딱히 할 일 없을지도
	}

	void EventManager::UnInitialize()
	{
		// 등록된 이벤트들을 모두 메모리에서 해제합니다.
	}

	void EventManager::DispatchEventMessages()
	{
		for (auto& [eventName, eventParameter] : _eventMessages)
		{
			switch (eventParameter._parameterType)
			{
				case EventParameterType::Void :
				{
					InvokeEvent(eventName);
					
					break;
				}

				case EventParameterType::Bool:
				{
					InvokeEvent(eventName, std::any_cast<bool>(eventParameter._parameter));

					break;
				}

				case EventParameterType::Int:
				{
					InvokeEvent(eventName, std::any_cast<int>(eventParameter._parameter));

					break;
				}

				case EventParameterType::Float:
				{
					InvokeEvent(eventName, std::any_cast<float>(eventParameter._parameter));

					break;
				}

				case EventParameterType::TString:
				{
					InvokeEvent<const DUOLCommon::tstring&>(eventName, std::any_cast<const DUOLCommon::tstring&>(eventParameter._parameter));

					break;
				}
			}
		}

		_eventMessages.clear();
	}
}