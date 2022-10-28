/**

    @file      CoroutineHandler.h
    @brief     C++ 20 Coroutine Wrapper for DUOLGameEngine.
    @details   ~
    @author    COMETLEE
    @date      28.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <memory>
#include <coroutine>

#include "DUOLGameEngine/Util/Coroutine/YieldInstructionBase.h"
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	class CoroutineHandler
	{
        struct promise_type;

        using CoroutineHandle = std::coroutine_handle<promise_type>;

        struct promise_type
        {
            std::shared_ptr<YieldInstructionBase> _yieldInstruction;

            std::suspend_never initial_suspend()
            {
                return {};
            }

            CoroutineHandler get_return_object()
            {
                return CoroutineHandler{ CoroutineHandle::from_promise(*this) };
            }

            std::suspend_always yield_value(const std::shared_ptr<YieldInstructionBase>& yieldInstruction)
            {
                _yieldInstruction = yieldInstruction;

                return {};
            }

            void return_void()
            {
	            // 할 일 없습니다.
            }

            void unhandled_exception()
            {
                std::terminate();
            }

            const std::shared_ptr<YieldInstructionBase> GetYieldInstruction() const { return _yieldInstruction; }

            promise_type() : _yieldInstruction(nullptr) { }

            ~promise_type() noexcept
            {
            	_yieldInstruction.reset();
            }
        };

        // 코루틴 핸들
        CoroutineHandle _coHandle;

        CoroutineHandler(CoroutineHandle handle);

        DEFINE_DEFAULT_COPY_MOVE(CoroutineHandler)

        ~CoroutineHandler()
        {
            // 코루틴 핸들의 소멸에 대한 사항은 밖에서 프로그래머가 처리합니다.
            // C++ 20 코루틴 시스템 구조 상 코루틴 핸들러가 함수 내에서 스택 객체로 밖에 반환이 안 되기 때문 ..!
        }

        void UpdateCoroutine(double deltaTime);

        [[nodiscard]]
        bool IsDone() const;

        [[nodiscard]]
        bool CanResume() const;

        void Resume() const;

        void UnInitialize() const;

        bool CompareCoroutineHandle(const CoroutineHandler& coroutineHandler) const;
	};
}