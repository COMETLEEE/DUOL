/**

    @file      EventListenerIDHolder.h
    @brief     Simple event listener id holder.
    @details   ~
    @author    COMETLEE
    @date      23.03.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Manager/EventManager.h"

#include "DUOLCommon/Event/Event.h"
#include "DUOLCommon/StringHelper.h"

namespace DUOLClient
{
	/**
	 * \brief 컨텐츠 코드를 작성할 때, 이벤트 함수를 일괄적으로 등록하고 해제하는 경우가 많을 것 같아서 .. 미리 빼놓는다.
	 */
	template <typename TParam>
	class EventListenerIDHolder
	{
	public:
		EventListenerIDHolder() : _eventListenerIDs {} {}

		~EventListenerIDHolder()
		{
			Refresh();
		}

	private:
		/**
		 * \brief 이벤트 아이디 모음집
		 */
		std::vector<std::pair<DUOLCommon::tstring, DUOLCommon::EventListenerID>> _eventListenerIDs;

	public:
		/**
		 * \brief 이벤트 리스너를 등록하고 정보를 저장합니다.
		 * \param eventName 이벤트의 이름
		 * \param eventListener 이벤트가 발생할 때 호출될 함수
		 */
		void AddEventFunction(const DUOLCommon::tstring& eventName, std::function<void(TParam)> eventListener);

		/**
		 * \brief 모든 이벤트 함수의 등록을 해제합니다.
		 */
		void Refresh();
	};

	template <typename TParam>
	void EventListenerIDHolder<TParam>::AddEventFunction(const DUOLCommon::tstring& eventName,
		std::function<void(TParam)> eventListener)
	{
		_eventListenerIDs.push_back({ eventName,
			DUOLGameEngine::EventManager::GetInstance()->AddEventFunction<TParam>(eventName, eventListener) });
	}

	template <typename TParam>
	void EventListenerIDHolder<TParam>::Refresh()
	{
		for (auto [eventName, id] : _eventListenerIDs)
			DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<TParam>(eventName, id);

		_eventListenerIDs.clear();
	}

	template <>
	class EventListenerIDHolder<void>
	{
	public:
		EventListenerIDHolder() : _eventListenerIDs{} {}

		~EventListenerIDHolder()
		{
			Refresh();
		}

	private:
		std::vector<std::pair<DUOLCommon::tstring, DUOLCommon::EventListenerID>> _eventListenerIDs;

	public:
		/**
		 * \brief 이벤트 리스너를 등록하고 정보를 저장합니다.
		 * \param eventName 이벤트의 이름
		 * \param eventListener 이벤트가 발생할 때 호출될 함수
		 */
		void AddEventFunction(const DUOLCommon::tstring& eventName, std::function<void(void)> eventListener);

		/**
		 * \brief 모든 이벤트 함수의 등록을 해제합니다.
		 */
		void Refresh();
	};
}