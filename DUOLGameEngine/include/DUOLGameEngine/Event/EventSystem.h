/**

	@file    EventSystem.h
	@brief   C Sharp�� delegate�� ������ �����ϴ� Ŭ����
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
		@brief   Functor ��ü�� Event�� ����
		@details -
		@tparam  ReturnType - Functor ��ü�� ��ȯ Ÿ��
		@tparam  ArgTypes   - Functor ��ü�� ���� ���� Ÿ��

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
			@brief   EventSystem Ŭ���� default ������
			@details -
		**/
		EventSystem() = default;

		/**
			@brief   EventSystem Ŭ���� default �Ҹ���
			@details -
		**/
		~EventSystem() = default;

		/**
			@brief   Event �߰��ϴ� �Լ�
			@details ��ȯ�Ǵ� shared_ptr�� �޾Ƽ� �����ϰ� �־�� Event�� �����Ѵ�. ���� ��� Event���� ���ŵ�.
			@param   func - �߰��� Functor ��ü
			@retval  Funtor ��ü�� shared_ptr�� ���� ��ȯ
		**/
		Event AddEvent(EventType func);

		/**
			@brief   ��ϵ� ��� Event ����
			@details -
			@param   args - Event�� �����ϱ� ���� �ʿ��� Arguments
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
			// ��ȿ�� �̺�Ʈ���� Ȯ���Ͽ� ����ų� �����Ѵ�.
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