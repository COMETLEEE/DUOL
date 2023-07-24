#include "DUOLClient/BehaviorTreeNode/Action/Action_RushPattern.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"


DUOLClient::Action_RushPattern::Action_RushPattern(const std::string& name, const BT::NodeConfig& config)
	:
	StatefulActionNode(name, config),
	_ai(nullptr),
	_animator(nullptr),
	_targetTransform(nullptr),
	_transform(nullptr),
	_timer(0),
	_rushCount(0),
	_isIdle(false),
	_particleRenderer(nullptr)
{
	_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();

	_transform = _ai->GetParentTransform();

	_targetTransform = _ai->GetTargetTransform();

	_animator = _ai->GetAnimator();
}

BT::NodeStatus DUOLClient::Action_RushPattern::onStart()
{


	_rushCount = 0;

	_timer = 0;

	_isIdle = false;

	_ai->SetParameter(TEXT("IsRushHit_Target"), false);
	_ai->GetEnemy()->SetCurrentPaternHitPlayer(false);
	if (_ai->GetParameter<bool>(TEXT("IsCanSuperArmor")) && !_ai->GetParameter<bool>(TEXT("IsSuperArmor")))
	{
		_animator->SetFloat(TEXT("MoveSpeed"), 0.0f);
		_animator->SetBool(TEXT("IsWalkRight"), false);
		_animator->SetBool(TEXT("IsWalkLeft"), false);
		_animator->SetBool(TEXT("IsWalkBack"), false);

		_ai->SetSuperArmor(true);
		return BT::NodeStatus::RUNNING;
	}
	else
		return BT::NodeStatus::FAILURE;
}

BT::NodeStatus DUOLClient::Action_RushPattern::onRunning()
{
	if (_particleRenderer)
	{
		if (!_particleRenderer->GetIsPlay())
			_particleRenderer = nullptr;
	}

	if (TEXT("Idle") == _animator->GetCurrentStateName())
	{
		if (_particleRenderer)
		{
			_particleRenderer->Stop();
			_particleRenderer = nullptr;
		}
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
		if (!_particleRenderer)
		{
			_particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterDashF);
			auto particleTr = _particleRenderer->GetTransform();
			particleTr->SetParent(_ai->GetTransform());
			particleTr->SetPosition(DUOLMath::Vector3(0.0f, 2.0f, 2.5f), DUOLGameEngine::Space::Self);
			particleTr->LookAt(particleTr->GetWorldPosition() + _ai->GetTransform()->GetRight());

		}

		if (!_particleRenderer->GetIsPlay())
			_particleRenderer->Play();

		_timer += DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

		if (_isIdle)
		{
			_isIdle = false;
			_rushCount++;
		}
	}
	else
	{
		if (_particleRenderer)
		{
			_particleRenderer->Stop();
			_particleRenderer = nullptr;
		}
	}

	if (_animator->GetBool(TEXT("IsGroggy")))
	{
		if (_particleRenderer)
		{
			_particleRenderer->Stop();
			_particleRenderer = nullptr;
		}
		_animator->SetBool(TEXT("IsRush"), false);
		_timer = 0;
		_ai->SetSuperArmor(false, _ai->GetSuperArmorTime());
		return BT::NodeStatus::SUCCESS;
	}

	auto distance = DUOLMath::Vector3::Distance(_transform->GetWorldPosition(), _targetTransform->GetWorldPosition());

	auto enemyToTarget = _targetTransform->GetWorldPosition() - _transform->GetWorldPosition();
	if (distance >= 5 && 0 < _transform->GetLook().Dot(enemyToTarget))
		_ai->LerpLookTarget();
	if (0 > _transform->GetLook().Dot(enemyToTarget)) // 플레이어를 지나쳤다면,
	{
		if (_timer > 3 || distance >= 8) // 일정 시간이 지났거나 거리가 멀어지면 종료한다.
		{
			_animator->SetBool(TEXT("IsRush"), false);
			_timer = 0;

			if (_particleRenderer)
			{
				_particleRenderer->Stop();
				_particleRenderer = nullptr;
			}

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

void DUOLClient::Action_RushPattern::onHalted()
{
}

BT::PortsList DUOLClient::Action_RushPattern::providedPorts()
{
	BT::PortsList result = {
BT::InputPort<DUOLClient::AI_EnemyBasic* >("AI")
	};

	return result;
}
