#include "DUOLClient/BehaviorTreeNode/Action/Action_Hit.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/Manager/TimeManager.h"


DUOLClient::Action_Hit::Action_Hit(const std::string& name, const BT::NodeConfig& config) :
	SyncActionNode(name, config), _ai(nullptr), _timer(2.0f), _delayTime(0.3f), _hitOnce(false)
{
}

BT::NodeStatus DUOLClient::Action_Hit::tick()
{
	if (!_ai)
		_ai = getInput<AI_EnemyBasic*>("AI").value();

	_timer += DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

	if (_ai->GetIsHitCheck())
	{
		_ai->SetIsHit(false);

		if (_ai->GetNavMeshAgent()->GetIsEnabled())
		{
			_ai->GetNavMeshAgent()->SetMaxSpeed(3.5f);
			_ai->GetNavMeshAgent()->SetVelocity(DUOLMath::Vector3(0, 0, 0));
		}

		_timer = 0;
		_hitOnce = true;
		_ai->SetIsGroupCheck();

		return BT::NodeStatus::SUCCESS;
	}
	else
	{
		if (_timer >= _delayTime) // 일정 시간이 지나고 HitCondition을 끈다.
		{

			_ai->GetAnimator()->SetBool(TEXT("IsHit_Front"), false);
			_ai->GetAnimator()->SetBool(TEXT("IsHit_Back"), false);
			_ai->GetAnimator()->SetBool(TEXT("IsHit_Air_1"), false);
			_ai->GetAnimator()->SetBool(TEXT("IsHit_Air_2"), false);
			_ai->GetAnimator()->SetBool(TEXT("IsHit_Air_3"), false);


			if (_hitOnce && !_ai->GetIsAirborne())
			{
				_ai->SetNavOnRigidbodyOff();
				_hitOnce = false;
			}
			return BT::NodeStatus::FAILURE;
		}
		else
			return BT::NodeStatus::SUCCESS;
	}
}

BT::PortsList DUOLClient::Action_Hit::providedPorts()
{

	BT::PortsList result = {
		BT::InputPort<AI_EnemyBasic*>("AI")
	};

	return result;
}
