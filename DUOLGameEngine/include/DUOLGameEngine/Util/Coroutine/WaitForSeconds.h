/**

    @file      WaitForSeconds.h
    @brief     Suspends the coroutine execution for the given amount of seconds using scaled time.
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
	 * \brief Suspends the coroutine execution for the given amount of seconds using scaled time.
	 */
	class WaitForSeconds : public YieldInstructionBase
	{
	public:
        WaitForSeconds(double waitTime);
        
        ~WaitForSeconds();

        DEFINE_DEFAULT_COPY_MOVE(WaitForSeconds)

	private:
        double _waitTime;

	public:
        virtual void UpdateInstruction(double deltaTime) override;

        virtual bool CanResume() override;
	};
}
