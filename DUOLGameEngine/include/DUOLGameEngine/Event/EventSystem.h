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

#include "Functor.h"

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
		Event AddEvent(const EventType& func);

		/**
			@brief   Event 추가하는 함수
			@details 반환되는 shared_ptr을 받아서 관리하고 있어야 Event가 동작한다. 없앨 경우 Event에서 제거됨.
			@param   func - 추가할 함수 포인터
			@retval  Funtor 객체를 shared_ptr로 만들어서 반환
		**/
		Event AddEvent(std::function<ReturnType(ArgTypes...)> func);

		/**
			@brief   등록된 모든 Event 실행
			@details -
			@param   args - Event를 실행하기 위해 필요한 Arguments
			@retval  Event의 반환 값 집합
		**/
		std::vector<ReturnType> Dispatch(ArgTypes... args);
	};

	template<typename ReturnType, typename ...ArgTypes>
	inline typename EventSystem<ReturnType, ArgTypes...>::Event EventSystem<ReturnType, ArgTypes...>::AddEvent(const EventType& func)
	{
		Event newEvent = std::make_shared<EventType>(func);

		_events.push_back(newEvent);

		return newEvent;
	}

	template<typename ReturnType, typename ...ArgTypes>
	inline typename EventSystem<ReturnType, ArgTypes...>::Event EventSystem<ReturnType, ArgTypes...>::AddEvent(std::function<ReturnType(ArgTypes...)> func)
	{
		Functor<ReturnType, ArgTypes...> functor(func);

		Event newEvent = std::make_shared<EventType>(functor);

		_events.push_back(newEvent);

		return newEvent;
	}

	template<typename ReturnType, typename ...ArgTypes>
	inline std::vector<ReturnType> EventSystem<ReturnType, ArgTypes...>::Dispatch(ArgTypes... args)
	{
		std::vector<ReturnType> ret;

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
				ret.push_back((*targetEvent)(args...));

				i++;
			}
		}

		return ret;
	}

	template <typename... ArgTypes>
	class EventSystem<void, ArgTypes...>
	{
	public:
		using EventType = Functor<void, ArgTypes...>;
		using Event = std::shared_ptr<EventType>;

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

	private:
		std::vector<std::weak_ptr<EventType>> _events;

	public:
		/**
			@brief   Event 추가하는 함수
			@details 반환되는 shared_ptr을 받아서 관리하고 있어야 Event가 동작한다. 없앨 경우 Event에서 제거됨.
			@param   func - 추가할 Functor 객체
			@retval  Funtor 객체를 shared_ptr로 만들어서 반환
		**/
		Event AddEvent(EventType func);

		/**
			@brief   Event 추가하는 함수
			@details 반환되는 shared_ptr을 받아서 관리하고 있어야 Event가 동작한다. 없앨 경우 Event에서 제거됨.
			@param   func - 추가할 함수 포인터
			@retval  Funtor 객체를 shared_ptr로 만들어서 반환
		**/
		Event AddEvent(std::function<void(ArgTypes...)> func);

		/**
			@brief   등록된 모든 Event 실행
			@details -
			@param   args - Event를 실행하기 위해 필요한 Arguments
			@retval  Event의 반환 값 집합
		**/
		void Dispatch(ArgTypes... args);
	};

	template<typename ...ArgTypes>
	inline typename EventSystem<void, ArgTypes...>::Event EventSystem<void, ArgTypes...>::AddEvent(EventType func)
	{
		Event newEvent = std::make_shared<EventType>(func);

		_events.push_back(newEvent);

		return newEvent;
	}

	template<typename ...ArgTypes>
	inline typename EventSystem<void, ArgTypes...>::Event EventSystem<void, ArgTypes...>::AddEvent(std::function<void(ArgTypes...)> func)
	{
		Functor<void, ArgTypes...> functor(func);

		Event newEvent = std::make_shared<EventType>(functor);

		_events.push_back(newEvent);

		return newEvent;
	}

	template<typename ...ArgTypes>
	inline void EventSystem<void, ArgTypes...>::Dispatch(ArgTypes... args)
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