#include "DUOLClient/BehaviorTreeNode/Action/Action_EliteEnemyRandomPattern.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

DUOLClient::Action_EliteEnemyRandomPattern::Action_EliteEnemyRandomPattern(const std::string& name,
	const BT::NodeConfig& config) :
	StatefulActionNode(name, config),
	_rushCount(0),
	patternQueue(),
	_currentPattern(0),
	_timer(0),
	_isIdle(false)
{
	_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();

	_transform = _ai->GetParentTransform();

	_targetTransform = _ai->GetTargetTransform();

	_animator = _ai->GetAnimator();
}


BT::NodeStatus DUOLClient::Action_EliteEnemyRandomPattern::onStart()
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

	if (_ai->GetParameter<bool>(TEXT("IsOnSuperArmorEvent")))
	{
		_ai->SetParameter(TEXT("IsOnSuperArmorEvent"), false);

		_currentPattern = patternQueue.front();

		patternQueue.pop();

		_rushCount = 0;

		_timer = 0;

		_isIdle = false;

		_ai->SetParameter(TEXT("IsRushHit_Target"), false);

		return BT::NodeStatus::RUNNING;

	}
	else
	{
		_animator->SetBool(TEXT("IsRush"), false);
	}

	return BT::NodeStatus::FAILURE;
}

BT::NodeStatus DUOLClient::Action_EliteEnemyRandomPattern::onRunning()
{

	switch (_currentPattern)
	{
	case 0: // 돌진
	{

		if (TEXT("Idle") == _animator->GetCurrentStateName())
		{
			if (_rushCount < 3)
			{
				_animator->SetBool(TEXT("IsRush"), true);
				_isIdle = true;
			}
			else if (!_ai->GetParameter<bool>(TEXT("IsRushHit_Target")))
			{
				_animator->SetBool(TEXT("IsGroggy"), true);
			}
		}

		if (TEXT("Rush") == _animator->GetCurrentStateName())
		{
			_timer += DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

			if (_isIdle)
			{
				_isIdle = false;
				_rushCount++;
			}
		}

		if (_animator->GetBool(TEXT("IsGroggy")))
		{
			_animator->SetBool(TEXT("IsRush"), false);
			_timer = 0;
			_ai->SetSuperArmor(false, _ai->GetSuperArmorTime());
			return BT::NodeStatus::SUCCESS;
		}
		auto distance = DUOLMath::Vector3::Distance(_transform->GetWorldPosition(), _targetTransform->GetWorldPosition());


		auto enemyToTarget = _targetTransform->GetWorldPosition() - _transform->GetWorldPosition();
		if (distance >= 5 && 0 < _transform->GetLook().Dot(enemyToTarget))
			_ai->LerpLookTarget();
		else if (0 > _transform->GetLook().Dot(enemyToTarget)) // 플레이어를 지나쳤다면,
		{
			if (_timer > 3 || distance >= 8) // 일정 시간이 지났거나 거리가 멀어지면 종료한다.
			{
				_animator->SetBool(TEXT("IsRush"), false);
				_timer = 0;

				if (_ai->GetParameter<bool>(TEXT("IsRushHit_Target")))
				{
					_ai->SetSuperArmor(false, _ai->GetSuperArmorTime());

					return BT::NodeStatus::SUCCESS;
				}
				else if (_rushCount >= 3)
				{
					_animator->SetBool(TEXT("IsGroggy"), true);

					_ai->SetSuperArmor(false, _ai->GetSuperArmorTime());

					return BT::NodeStatus::SUCCESS;
				}
			}
		}

		return BT::NodeStatus::RUNNING;
	}
	case 1: // 정권지르기
		_animator->SetBool(TEXT("IsSeriousAttack"), true);
		break;

	case 2: // 점프 공격
		_animator->SetBool(TEXT("IsJumpAttack"), true);
		break;
	default:
		break;
	}

	return BT::NodeStatus::SUCCESS;
}

void DUOLClient::Action_EliteEnemyRandomPattern::onHalted()
{
}

BT::PortsList DUOLClient::Action_EliteEnemyRandomPattern::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLClient::AI_EnemyBasic* >("AI")
	};

	return result;
}
