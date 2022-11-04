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
        WaitForSeconds(float waitTime);
        
        ~WaitForSeconds();

        DEFINE_DEFAULT_COPY_MOVE(WaitForSeconds)

	private:
        float _waitTime;

	public:
        virtual void UpdateInstruction(float deltaTime) override;

        virtual bool CanResume() override;
	};
}
