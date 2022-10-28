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
        WaitForSecondsRealtime(double waitTime);

        ~WaitForSecondsRealtime();

        DEFINE_DEFAULT_COPY_MOVE(WaitForSecondsRealtime)

	private:
        double _waitTime;

	public:
        virtual void UpdateInstruction(double deltaTime) override;

        virtual bool CanResume() override;
	};
}