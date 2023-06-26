#include "DUOLClient/BehaviorTreeNode/Action/Action_Hit.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/Manager/EnemyAudioManager.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/Manager/TimeManager.h"


DUOLClient::Action_Hit::Action_Hit(const std::string& name, const BT::NodeConfig& config) :
	SyncActionNode(name, config), _timer(2.0f), _delayTime(0.3f), _hitOnce(true)
{
	_ai = getInput<AI_EnemyBasic*>("AI").value();
}

BT::NodeStatus DUOLClient::Action_Hit::tick()
{
	_timer += DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

	if (_ai->GetIsHitCheck())
	{
		_ai->SetIsHit(false);

		if (_ai->GetNavMeshAgent()->GetIsEnabled())
		{
			_ai->GetNavMeshAgent()->SetMaxSpeed(_ai->GetMaxSpeed());
			_ai->GetNavMeshAgent()->SetVelocity(DUOLMath::Vector3(0, 0, 0));
		}

		_timer = 0;
		_hitOnce = true;
		_ai->SetIsGroupCheck();

		return BT::NodeStatus::SUCCESS;
	}
	else
	{
		if (_timer >= _delayTime) // ���� �ð��� ������ HitCondition�� ����.
		{
			_ai->GetAnimator()->SetBool(TEXT("IsHit_Front"), false);
			_ai->GetAnimator()->SetBool(TEXT("IsHit_Back"), false);

			if (!_ai->GetIsAirborne())
			{

				if (_hitOnce)
				{
					_ai->SetNavOnRigidbodyOff();
					_hitOnce = false;
				}
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
