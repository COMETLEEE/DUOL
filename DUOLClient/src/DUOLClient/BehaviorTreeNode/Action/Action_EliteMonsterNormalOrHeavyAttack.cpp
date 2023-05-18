#include "DUOLClient/BehaviorTreeNode/Action/Action_EliteMonsterNormalOrHeavyAttack.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Manager/TimeManager.h"


DUOLClient::Action_EliteMonsterNormalOrHeavyAttack::Action_EliteMonsterNormalOrHeavyAttack(const std::string& name,
	const BT::NodeConfig& config) :
	SyncActionNode(name, config), _ai(nullptr), _lastHeavyAttackTime(0)
{
	_ai = getInput<AI_EnemyBasic*>("AI").value();

	_navMesh = _ai->GetNavMeshAgent();

	_animator = _ai->GetAnimator();

	_timeManager = DUOLGameEngine::TimeManager::GetInstance().get();
}

BT::NodeStatus DUOLClient::Action_EliteMonsterNormalOrHeavyAttack::tick()
{
	_ai->UseToken();

	const int rand = DUOLMath::MathHelper::Rand(0, 1);

	constexpr float coolTime = 5.0f;

	switch (rand)
	{
	case 0: // 기본공격
		_animator->SetBool(TEXT("IsAttack"), true);

		break;
	case 1: // 강공격
		if (_timeManager->GetRealtimeScaledSinceStartup() - _lastHeavyAttackTime > coolTime)
		{
			_lastHeavyAttackTime = _timeManager->GetRealtimeScaledSinceStartup();
			_animator->SetBool(TEXT("IsHeavyAttack"), true);
		}
		else
		{
			_animator->SetBool(TEXT("IsAttack"), true);
		}

		break;
	default:
		break;
	}

	return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLClient::Action_EliteMonsterNormalOrHeavyAttack::providedPorts()
{
	BT::PortsList result = {
BT::InputPort<AI_EnemyBasic*>("AI")
	};
	return result;
}


