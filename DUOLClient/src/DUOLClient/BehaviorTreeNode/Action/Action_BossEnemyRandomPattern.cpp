#include "DUOLClient/BehaviorTreeNode/Action/Action_BossEnemyRandomPattern.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

DUOLClient::Action_BossEnemyRandomPattern::Action_BossEnemyRandomPattern(const std::string& name,
	const BT::NodeConfig& config) :
	SyncActionNode(name, config),
	patternQueue(),
	_currentPattern(0)
{
	_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();

	_transform = _ai->GetParentTransform();

	_targetTransform = _ai->GetTargetTransform();

	_animator = _ai->GetAnimator();
}



BT::NodeStatus DUOLClient::Action_BossEnemyRandomPattern::tick()
{
	if (patternQueue.empty())
	{
		// 겹치치 않는 난수 생성
		std::vector<int> randArr = { 0,1,2 };
		int max = randArr.size() - 1;

		for (int i = 0; i < 3; i++)
		{
			int index = DUOLMath::MathHelper::Rand(0, max);
			patternQueue.push(randArr[index]);
			std::swap(randArr[index], randArr[max--]);
		}
	}

	_currentPattern = patternQueue.front();

	patternQueue.pop();

	if (_animator->GetBool(TEXT("IsSwordForm")))
		SwordPattern();
	else
		FistPattern();

	return BT::NodeStatus::SUCCESS;

}

void DUOLClient::Action_BossEnemyRandomPattern::SwordPattern()
{
	switch (_currentPattern)
	{
	case 0: // SwordPattern1
		_animator->SetBool(TEXT("IsSwordPattern1"), true);
		break;
	case 1: // SwordPattern1
		_animator->SetBool(TEXT("IsSwordPattern2"), true);
		break;
	case 2: // SwordPattern1
		_animator->SetBool(TEXT("IsSwordPattern3"), true);
		break;
	default:
		break;
	}
}

void DUOLClient::Action_BossEnemyRandomPattern::FistPattern()
{
	switch (_currentPattern)
	{
	case 0: // SwordPattern1
		_animator->SetBool(TEXT("IsFistPattern1"), true);
		break;
	case 1: // SwordPattern1
		_animator->SetBool(TEXT("IsFistPattern2"), true);
		break;
	case 2: // SwordPattern1
		_animator->SetBool(TEXT("IsFistPattern3"), true);
		break;
	default:
		break;
	}
}

BT::PortsList DUOLClient::Action_BossEnemyRandomPattern::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLClient::AI_EnemyBasic* >("AI")
	};

	return result;
}
