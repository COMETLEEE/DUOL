#include "DUOLClient/BehaviorTreeNode/Action/Action_Hit.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBase.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/Manager/TimeManager.h"


DUOLClient::Action_Hit::Action_Hit(const std::string& name, const BT::NodeConfig& config) :
	SyncActionNode(name, config), _ai(nullptr), _timer(2.0f), _delayTime(2.0f)
{
	_hitEnum = static_cast<HitEnum>(DUOLMath::MathHelper::Rand(0, 1));
}

BT::NodeStatus DUOLClient::Action_Hit::tick()
{
	if (!_ai)
		_ai = getInput<AI_EnemyBase*>("AI").value();

	_timer += DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

	if (_ai->GetIsHitCheck())
	{
		_ai->SetIsHit(false);
		switch (_hitEnum)
		{
		case HitEnum::Front:
			_ai->SetAnimConditionReset();
			_ai->GetAnimator()->SetBool(TEXT("IsHit_Front"), true);
			_hitEnum = HitEnum::Back;
			break;
		case HitEnum::Back:
			_ai->SetAnimConditionReset();
			_ai->GetAnimator()->SetBool(TEXT("IsHit_Back"), true);
			_hitEnum = HitEnum::Front;
			break;
		default:
			break;
		}

		if (_ai->GetNavMeshAgent()->GetIsEnabled())
		{
			_ai->GetNavMeshAgent()->SetMaxSpeed(3.5f);
			_ai->GetNavMeshAgent()->SetVelocity(DUOLMath::Vector3(0, 0, 0));
		}

		_timer = 0;

		return BT::NodeStatus::SUCCESS;
	}
	else
	{
		_ai->GetAnimator()->SetBool(TEXT("IsHit_Front"), false);
		_ai->GetAnimator()->SetBool(TEXT("IsHit_Back"), false);

		if (_timer >= _delayTime)
			return BT::NodeStatus::FAILURE;
		else
			return BT::NodeStatus::SUCCESS;
	}
}

BT::PortsList DUOLClient::Action_Hit::providedPorts()
{

	BT::PortsList result = {
		BT::InputPort<AI_EnemyBase*>("AI")
	};

	return result;
}
