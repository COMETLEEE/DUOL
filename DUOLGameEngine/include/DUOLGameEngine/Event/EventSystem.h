/**

	@file    EventSystem.h
	@brief   C Sharp의 delegate와 유사한 동작하는 클래스
	@details -
	@author  JKim
	@date    11.10.2022

**/
#pragma once
#include <memory>
#include <vector>

#include "DUOLGameEngine/Event/Functor.h"

namespace DUOLGameEngine
{
	/**

		@class   EventSystem
		@brief   Functor 객체로 Event를 관리
		@details -
		@tparam  ReturnType - Functor 객체의 반환 타입
		@tparam  ArgTypes   - Functor 객체의 가변 인자 타입

	**/
	template <typename ReturnType, typename... ArgTypes>
	class EventSystem
	{
	public:
		using EventType = Functor<ReturnType, ArgTypes...>;
		using Event = std::shared_ptr<EventType>;

	private:
		std::vector<std::weak_ptr<EventType>> _events;

	public:
		/**
			@brief   EventSystem 클래스 default 생성자
			@details -
		**/
		EventSystem() = default;

		/**
			@brief   EventSystem 클래스 default 소멸자
			@details -
		**/
		~EventSystem() = default;

		/**
			@brief   Event 추가하는 함수
			@details 반환되는 shared_ptr을 받아서 관리하고 있어야 Event가 동작한다. 없앨 경우 Event에서 제거됨.
			@param   func - 추가할 Functor 객체
			@retval  Funtor 객체를 shared_ptr로 만들어서 반환
		**/
		Event AddEvent(EventType func);

		/**
			@brief   등록된 모든 Event 실행
			@details -
			@param   args - Event를 실행하기 위해 필요한 Arguments
		**/
		void Dispatch(ArgTypes... args);
	};

	template<typename ReturnType, typename ...ArgTypes>
	inline typename EventSystem<ReturnType, ArgTypes...>::Event EventSystem<ReturnType, ArgTypes...>::AddEvent(EventType func)
	{
		Event newEvent = std::make_shared<EventType>(func);

		_events.push_back(newEvent);

		return newEvent;
	}

	template<typename ReturnType, typename ...ArgTypes>
	inline void EventSystem<ReturnType, ArgTypes...>::Dispatch(ArgTypes... args)
	{
		for (int i = 0; i < _events.size();)
		{
			// 유효한 이벤트인지 확인하여 지우거나 실행한다.
			auto targetEvent = _events[i].lock();

			if (targetEvent == nullptr)
			{
				_events.erase(_events.begin() + i);
			}
			else
			{
				(*targetEvent)(args...);

				i++;
			}
		}
	}
}