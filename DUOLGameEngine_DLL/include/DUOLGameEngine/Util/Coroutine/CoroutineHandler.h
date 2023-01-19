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

namespace std
{
    using namespace DUOLGameEngine;
}

namespace DUOLGameEngine
{
    class MonoBehaviourBase;

    class DUOL_GAMEENGINE_API CoroutineHandler
    {
    public:
        struct promise_type;

        using CoroutineHandle = std::coroutine_handle<promise_type>;

        struct promise_type
        {
            std::shared_ptr<YieldInstructionBase> _yieldInstruction;

            // 이거 그냥 Instruction 전부 한 곳에서 관리 후 끝나면 CallBack으로 플래그 세워서
            // 코루틴 돌아가도록 하기 ..?
            bool _isYieldedByNullThisFrame;

            std::suspend_never initial_suspend()
            {
                return {};
            }

            std::suspend_always final_suspend() noexcept
            {
                return {};
            }

        	DUOLGameEngine::CoroutineHandler get_return_object()
            {
                return DUOLGameEngine::CoroutineHandler{ CoroutineHandle::from_promise(*this) };
            }

            std::suspend_always yield_value(const std::shared_ptr<YieldInstructionBase>& yieldInstruction)
            {
                // co_yield nullptr; => 다음 프레임에 Resume 합니다.
                if (yieldInstruction == nullptr)
                {
                    _isYieldedByNullThisFrame = true;

                    return {};
                }
                else
                {
                    _yieldInstruction = yieldInstruction;

                    return {};
                }
            }

            void return_void() const
            {
                // 할 일 없습니다.
            }

            void unhandled_exception() const
            {
                std::terminate();
            }

            // YieldInstruction의 참조 반환에 대한 문제가 발생할 수 있는가 ..?
            const std::shared_ptr<YieldInstructionBase>& GetYieldInstruction() const { return _yieldInstruction; }

            promise_type() :
                _yieldInstruction(nullptr)
                , _isYieldedByNullThisFrame(false)
            {

            }

            ~promise_type() noexcept
            {
                _yieldInstruction.reset();
            }
        };

        // 코루틴 핸들
        CoroutineHandle _coHandle;

        CoroutineHandler(CoroutineHandle handle);

        ~CoroutineHandler() = default;
        // {
            // 코루틴 핸들의 소멸에 대한 사항은 밖에서 프로그래머가 처리합니다.
            // C++ 20 코루틴 시스템 구조 상 코루틴 핸들러가 함수 내에서 스택 객체로 밖에 반환이 안 되기 때문 ..!
        //  }

        DEFINE_DEFAULT_COPY_MOVE(CoroutineHandler)

        void UpdateCoroutine(float deltaTime) const;

        [[nodiscard]]
        bool IsDone() const;

        [[nodiscard]]
        bool CanResume() const;

        void Resume() const;

        void UnInitialize() const;

        bool CompareCoroutineHandle(const CoroutineHandler& coroutineHandler) const;
    };
}