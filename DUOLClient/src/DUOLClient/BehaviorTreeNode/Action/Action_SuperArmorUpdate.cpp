#include "DUOLClient/BehaviorTreeNode/Action/Action_SuperArmorUpdate.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

BT::NodeStatus DUOLClient::Action_SuperArmorUpdate::tick()
{
	if (!_ai)
	{
		_ai = getInput<AI_EnemyBasic*>("AI").value();
		_maxGauge = getInput<float>("MaxGauge").value();
	}

	if (!_ai->GetIsSuperArmor()) // 슈퍼아머가 비활성화 되어있다면
	{
		_currentGauge += DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

		if (_currentGauge >= _maxGauge)
		{
			_ai->SetSuperArmor(true, 10.0f);
			_currentGauge = 0;
			return BT::NodeStatus::SUCCESS;
		}
	}

	return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Action_SuperArmorUpdate::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<AI_EnemyBasic*>("AI"),
		BT::InputPort<float>("MaxGauge")
	};

	return result;
}
