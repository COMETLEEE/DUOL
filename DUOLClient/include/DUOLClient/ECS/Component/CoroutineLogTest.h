#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief �ڷ�ƾ�� �ΰ��� ������ �׽�Ʈ�ϱ� ���� �뵵.
	 */
	class DUOL_CLIENT_API CoroutineLogTest : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		CoroutineLogTest(DUOLGameEngine::GameObject* owner);

		virtual ~CoroutineLogTest() override = default;

	public:
		DUOLGameEngine::CoroutineHandler LogPerTwoSeconds();

		DUOLGameEngine::CoroutineHandler CoroutineInCoroutine();

		DUOLGameEngine::CoroutineHandler InvokeInCoroutine();

		void InvokeTestFunc();

	public:
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}