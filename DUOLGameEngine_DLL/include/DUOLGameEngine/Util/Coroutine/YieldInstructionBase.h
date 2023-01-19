/**

    @file      YieldInstructionBase.h
    @brief     Base class for all yield instructions.
    @details   ~
    @author    COMETLEE
    @date      27.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Export_Engine.h"

#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	enum class YieldInstructionType
	{
		NONE,
		WAIT_FOR_SECONDS,
		WAIT_FOR_SECONDS_REALTIME,
		COROUTINE
	};

	/**
	 * \brief DUOLGameEngine 코루틴 시스템에서 모든 yield 명령어의 기본 클래스입니다.
	 */
	class DUOL_GAMEENGINE_API YieldInstructionBase
	{
		friend class CoroutineHandler;

	protected:
		YieldInstructionBase(YieldInstructionType type);

		virtual ~YieldInstructionBase();

		DEFINE_DEFAULT_COPY_MOVE(YieldInstructionBase)

	protected:
		YieldInstructionType _instructionType;

	public:
		/**
		 * \brief 해당 코루틴 함수의 Resume Timing을 계산하기 위해 Yield 명령어를 업데이트합니다.
		 */
		virtual void UpdateInstruction(float deltaTime) abstract;

		/**
		 * \brief 해당 코루틴 함수가 Resume 할 수 있는지 확인합니다.
		 * \return 만약, 해당 코루틴 함수가 Resume 할 수 있는 상황이면 true, 아니라면 false를 반환합니다.
		 */
		virtual bool CanResume() abstract;
	};
}