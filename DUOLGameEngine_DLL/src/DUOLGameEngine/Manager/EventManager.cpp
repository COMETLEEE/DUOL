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
		// 'std::any' Ÿ�Կ� ���� Resize event register. => ���� ���� ���ڴ� DUOLMath::Vector2*
		RegisterEvent<std::any>(TEXT("Resize"));

		// 'void' Ÿ�Կ� ���� SceneRendering event register.
		RegisterEvent<void>(TEXT("SceneRendering"));

		// 'void' Ÿ�Կ� ���� SceneLighting event register.
		RegisterEvent<void>(TEXT("SceneLighting"));
	}

	void EventManager::UnInitialize()
	{
		// ��ϵ� �̺�Ʈ���� ��� �޸𸮿��� �����մϴ�.
		_eventsVoid.clear();

		_eventsBool.clear();

		_eventsInt.clear();

		_eventsFloat.clear();

		_eventsTString.clear();

		_eventsAny.clear();

		_eventMessages.clear();
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

				case EventParameterType::Any:
				{
					InvokeEvent(eventName, eventParameter._parameter);

					break;
				}
			}
		}

		_eventMessages.clear();
	}
}