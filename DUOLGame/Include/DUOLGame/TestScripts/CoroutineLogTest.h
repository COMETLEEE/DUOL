#pragma once
#include "DUOLCommon/LogHelper.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGame
{
	/**
	 * \brief 코루틴과 로거의 결합을 테스트하기 위한 용도.
	 */
	class CoroutineLogTest : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		CoroutineLogTest(const std::weak_ptr<DUOLGameEngine::GameObject>& owner);

		virtual ~CoroutineLogTest() override = default;

	public:
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}