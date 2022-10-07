#pragma once
#include <functional>

namespace DUOLGameEngine
{
	// Functional Wrapping Å¬·¡½º
	template <typename ReturnType, typename... ArgTypes>
	class Functor
	{
	public:
		using FuncPtr = std::function<ReturnType(ArgTypes...)>;

	private:
		FuncPtr _func;

	public:
		Functor(FuncPtr callableFunc);

		ReturnType operator ()(ArgTypes... args);
	};

	template<typename ReturnType, typename... ArgTypes>
	inline Functor<ReturnType, ArgTypes...>::Functor(FuncPtr callableFunc)
		: _func(callableFunc)
	{

	}

	template<typename ReturnType, typename... ArgTypes>
	inline ReturnType Functor<ReturnType, ArgTypes...>::operator()(ArgTypes... args)
	{
		return _func(args...);
	}
}