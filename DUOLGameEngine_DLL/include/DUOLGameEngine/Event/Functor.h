/**

	@file    Functor.h
	@brief   Functional Wrapping 클래스
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
		@brief   Functional Wrapping 클래스
		@details -
		@tparam  ReturnType - Functor 객체의 반환 타입
		@tparam  ArgTypes   - Functor 객체의 가변 인자 타입

	**/
	template <typename ReturnType, typename... ArgTypes>
	class DUOL_GAMEENGINE_TEMPLATE_API Functor
	{
	public:
		using FuncType = std::function<ReturnType(ArgTypes...)>;

	public:
		/**
			@brief   Functor 클래스 생성자
			@details -
			@param   callableFunc - function 객체를 받아온다.
		**/
		Functor(const FuncType& callableFunc);

		/**
			@brief   함수처럼 동작하기 위해 Operator () 구현
			@details -
			@param   args - 함수를 실행시키기 위한 Arguments들의 Input
			@retval  함수의 실행 결과를 바로 반환
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