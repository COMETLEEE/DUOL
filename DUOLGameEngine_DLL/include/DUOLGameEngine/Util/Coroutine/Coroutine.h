/**

    @file      Coroutine.h
    @brief     Coroutine function wrapper using by yieldInstruction of other coroutine function.
    @details   ~
    @author    COMETLEE
    @date      26.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Export_Engine.h"

#include "DUOLGameEngine/Util/Coroutine/YieldInstructionBase.h"
#include "DUOLGameEngine/Util/Coroutine/CoroutineHandler.h"

namespace DUOLGameEngine
{
	class CoroutineHandler;

	/**
	 * \brief MonoBehaviour.StartCoroutine() returns a Coroutine. Using only as reference of CoroutineHandler.
	 */
	class DUOL_GAMEENGINE_API Coroutine : public YieldInstructionBase
	{
		friend class MonoBehaviourBase;

	public:
		Coroutine(const CoroutineHandler& coroutineHandler);

		virtual ~Coroutine() override;

		// 콜백 방식은 어떰 .. StartCoroutine 함수 호출 시 CoroutineHandler가 CoroutineManager 내부로 들어감.
		// StopCoroutine은 그냥 핸들 딜리트하면 됨. => 멀티 쓰레드 아니면 딱히 핸들 소멸 타이밍 고민할 필요 X ..
		CoroutineHandler _coroutineHandler;

		DEFINE_DEFAULT_COPY_MOVE(Coroutine)

	public:
		virtual void UpdateInstruction(float deltaTime) override;

		virtual bool CanResume() override;

		inline const CoroutineHandler& GetCoroutineHandler() { return _coroutineHandler; }
	};
}