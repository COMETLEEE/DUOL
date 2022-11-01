#pragma once
#include "DUOLCommon/LogHelper.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGame
{
	/**
	 * \brief �ڷ�ƾ�� �ΰ��� ������ �׽�Ʈ�ϱ� ���� �뵵.
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