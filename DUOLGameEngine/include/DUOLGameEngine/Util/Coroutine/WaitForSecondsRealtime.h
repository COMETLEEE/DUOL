/**

    @file      WaitForSecondsRealtime.h
    @brief     Suspends the coroutine execution for the given amount of seconds using unscaled time.
    @details   ~
    @author    COMETLEE
    @date      27.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "YieldInstructionBase.h"

namespace DUOLGameEngine
{
	/**
	 * \brief Suspends the coroutine execution for the given amount of seconds using unscaled time.
	 */
	class WaitForSecondsRealtime : public YieldInstructionBase
	{
	public:
        WaitForSecondsRealtime(float waitTime);

        virtual ~WaitForSecondsRealtime() override;

        DEFINE_DEFAULT_COPY_MOVE(WaitForSecondsRealtime)

	private:
        float _waitTime;

	public:
        virtual void UpdateInstruction(float deltaTime) override;

        virtual bool CanResume() override;
	};
}