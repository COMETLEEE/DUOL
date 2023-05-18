#include "DUOLClient/BehaviorTreeNode/Action/Action_SuperArmorUpdate.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

DUOLClient::Action_SuperArmorUpdate::Action_SuperArmorUpdate(const std::string& name, const BT::NodeConfig& config) :
	BT::SyncActionNode(name, config), _ai(nullptr)
{
	_ai = getInput<AI_EnemyBasic*>("AI").value();
}

BT::NodeStatus DUOLClient::Action_SuperArmorUpdate::tick()
{
	if (!_ai->GetIsSuperArmor()) // 슈퍼아머가 비활성화 되어있다면
	{
		_ai->AddSuperArmorGauge(DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime());

		if (_ai->GetCurrentSuperArmorGauge() >= _ai->GetMaxSuperArmorGauge())
		{
			_ai->SetAnimConditionReset();
			_ai->SetSuperArmor(true, 10.0f);
			return BT::NodeStatus::SUCCESS;
		}
	}

	return BT::NodeStatus::FAILURE;
}

BT::PortsList DUOLClient::Action_SuperArmorUpdate::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<AI_EnemyBasic*>("AI")
	};

	return result;
}
