/**

    @file      Coroutine.h
    @brief     Coroutine system using in MonoBehaviour class.
    @details   ~
    @author    COMETLEE
    @date      26.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/

#pragma once
#include <coroutine>

namespace DUOLGameEngine
{
	/**
	 * \brief C++ 20 Coroutine system wrapper class using in DUOLGameEngine.
	 */
	class Coroutine
	{
		struct promise_type;

		using CoroutineHandle = std::coroutine_handle<promise_type>;

		struct promise_type
		{


			std::suspend_never initial_suspend()
			{
				return {};
			}

			std::suspend_always final_suspend() noexcept
			{
				return {};
			}

			Coroutine get_return_object()
			{
				return Coroutine{ CoroutineHandle::from_promise(*this) };
			}

			
		};

		CoroutineHandle _coHandle;

		Coroutine(CoroutineHandle handle) : _coHandle(handle) {};

		~Coroutine()
		{
			if (_coHandle)
			{
				_coHandle.destroy();
			}
		}

		bool IsDone() { return _coHandle.done(); }

		void Resume() { _coHandle.resume(); }
	};
}