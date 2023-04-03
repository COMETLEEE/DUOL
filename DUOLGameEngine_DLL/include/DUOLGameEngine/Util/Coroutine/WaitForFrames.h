/**

    @file      WaitForFrames.h
    @brief     몇 프레임 단위로 코루틴을 중지할 것인가에 대한 명령어
    @details   ~
    @author    COMETLEE
    @date      3.04.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Util/Coroutine/YieldInstructionBase.h"

namespace DUOLGameEngine
{
	/**
	 * \brief Suspends the coroutine execution for the given amount of seconds using frame count.
	 */
	class DUOL_GAMEENGINE_API WaitForFrames : public DUOLGameEngine::YieldInstructionBase
	{
	public:
		WaitForFrames(int frameCount);

		virtual ~WaitForFrames() override;

		DEFINE_DEFAULT_COPY_MOVE(WaitForFrames)

	private:
		int _waitFrame;

	public:
		virtual void UpdateInstruction(float deltaTime) override;

		virtual bool CanResume() override;
	};
}