/**

	@file    Functor.h
	@brief   Functional Wrapping Ŭ����
	@details -
	@author  JKim
	@date    11.10.2022

**/
#pragma once
#include <functional>
#include <type_traits>

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
	class Functor
	{
	public:
		using FuncPtr = std::function<ReturnType(ArgTypes...)>;

	private:
		FuncPtr _func;

	public:
		/**
			@brief   Functor Ŭ���� ������
			@details -
			@param   callableFunc - function ��ü�� �޾ƿ´�.
		**/
		Functor(FuncPtr callableFunc);

		/**
			@brief   �Լ�ó�� �����ϱ� ���� Operator () ����
			@details -
			@param   args - �Լ��� �����Ű�� ���� Arguments���� Input
			@retval  �Լ��� ���� ����� �ٷ� ��ȯ
		**/
		ReturnType operator ()(ArgTypes... args);
	};

	template<typename ReturnType, typename... ArgTypes>
	inline Functor<ReturnType, ArgTypes...>::Functor(FuncPtr callableFunc)
		: _func(callableFunc)
	{
		static_assert(std::is_null_pointer<decltype(callableFunc)>::value, "Functor class cannot be created with nullptr.");
	}

	template<typename ReturnType, typename... ArgTypes>
	inline ReturnType Functor<ReturnType, ArgTypes...>::operator()(ArgTypes... args)
	{
		return _func(args...);
	}
}