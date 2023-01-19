/**

	@file    Functor.h
	@brief   Functional Wrapping Ŭ����
	@details -
	@author  JKim
	@date    11.10.2022

**/
#pragma once
#include "DUOLGameEngine/Export_Engine.h"

#include <functional>
#include <assert.h>

namespace DUOLGameEngine
{
	/**

		@class   Functor
		@brief   Functional Wrapping Ŭ����
		@details -
		@tparam  ReturnType - Functor ��ü�� ��ȯ Ÿ��
		@tparam  ArgTypes   - Functor ��ü�� ���� ���� Ÿ��

	**/
	template <typename ReturnType, typename... ArgTypes>
	class DUOL_GAMEENGINE_TEMPLATE_API Functor
	{
	public:
		using FuncType = std::function<ReturnType(ArgTypes...)>;

	public:
		/**
			@brief   Functor Ŭ���� ������
			@details -
			@param   callableFunc - function ��ü�� �޾ƿ´�.
		**/
		Functor(const FuncType& callableFunc);

		/**
			@brief   �Լ�ó�� �����ϱ� ���� Operator () ����
			@details -
			@param   args - �Լ��� �����Ű�� ���� Arguments���� Input
			@retval  �Լ��� ���� ����� �ٷ� ��ȯ
		**/
		ReturnType operator ()(ArgTypes... args);

	private:
		FuncType _func;
	};

	template<typename ReturnType, typename... ArgTypes>
	inline Functor<ReturnType, ArgTypes...>::Functor(const FuncType& callableFunc) :
		_func(callableFunc)
	{
		if (callableFunc == nullptr)
			assert(!"Functor class cannot be created with nullptr.");
	}

	template<typename ReturnType, typename... ArgTypes>
	inline ReturnType Functor<ReturnType, ArgTypes...>::operator()(ArgTypes... args)
	{
		return _func(args...);
	}
}