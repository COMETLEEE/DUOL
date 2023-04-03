#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief 코루틴을 테스트하자 ..
	 */
	class DUOL_CLIENT_API CoroutineLogTest : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		CoroutineLogTest(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("CoroutineLogTest"));

		virtual ~CoroutineLogTest() override = default;

	public:
		DUOLGameEngine::CoroutineHandler LogPerTwoSeconds();

		DUOLGameEngine::CoroutineHandler CoroutineInCoroutine();

		DUOLGameEngine::CoroutineHandler InvokeInCoroutine();

		DUOLGameEngine::CoroutineHandler YieldNull();

		void InvokeTestFunc();

	public:
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};
}