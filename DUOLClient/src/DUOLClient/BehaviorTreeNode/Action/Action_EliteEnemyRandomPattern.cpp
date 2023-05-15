#include "DUOLClient/BehaviorTreeNode/Action/Action_EliteEnemyRandomPattern.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

BT::NodeStatus DUOLClient::Action_EliteEnemyRandomPattern::onStart()
{
	if (!_ai)
	{
		_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();

		_transform = _ai->GetParentTransform();

		_targetTransform = _ai->GetTargetTransform();

		_animator = _ai->GetAnimator();
	}

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

	if (_ai->GetIsSuperArmor())
	{
		_currentPattern = patternQueue.front();

		patternQueue.pop();

		_rushCount = 0;

		_timer = 0;

		_isIdle = false;

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
	_timer += DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();
	switch (_currentPattern)
	{
	case 0: // 돌진
	{

		if (TEXT("Idle") == _animator->GetCurrentStateName() && _rushCount < 3)
		{
			_animator->SetBool(TEXT("IsRush"), true);
			_isIdle = true;
		}

		if (TEXT("Rush") == _animator->GetCurrentStateName() && _isIdle)
		{
			_isIdle = false;
			_rushCount++;
		}

		auto distance = DUOLMath::Vector3::Distance(_transform->GetWorldPosition(), _targetTransform->GetWorldPosition());


		if (distance >= 5 && 0 < _transform->GetLook().Dot(_targetTransform->GetWorldPosition() - _transform->GetWorldPosition()))
			_ai->LerpLookTarget();

		if (_timer > 5)
		{
			_animator->SetBool(TEXT("IsRush"), false);
			_timer = 0;

			if (_rushCount >= 3)
				break;
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
