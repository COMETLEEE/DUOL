#include "DUOLClient/BehaviorTreeNode/Action/Action_AroundPatrol.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"

DUOLClient::Action_AroundPatrol::Action_AroundPatrol(const std::string& name, const BT::NodeConfig& config) :
	StatefulActionNode(name, config),
	_enemyGroupController(nullptr)
{
	_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();

	_transform = _ai->GetParentTransform();

	_targetTransform = _ai->GetTargetTransform();

	_randomOffset = 2.0f;

	_distance = _ai->GetPatrolRange();

	_animator = _ai->GetAnimator();

	_navMeshAgent = _ai->GetNavMeshAgent();
}

BT::NodeStatus DUOLClient::Action_AroundPatrol::onStart()
{
	if (_targetTransform == nullptr || _navMeshAgent == nullptr || !_navMeshAgent->GetIsEnabled()) return BT::NodeStatus::FAILURE;

	if (!_enemyGroupController)
		_enemyGroupController = _ai->GetGroupController();

	const DUOLMath::Vector3& targetPos = _targetTransform->GetWorldPosition();

	auto dir = _transform->GetWorldPosition() - targetPos;

	dir.y = 0;

	dir.Normalize();

	auto rad = DUOLMath::MathHelper::RandF(-0.9f, 0.9f);

	const float _cos = cosf(rad);
	const float _sin = sinf(rad);
	const float x = _cos * dir.x + (-_sin * dir.z);
	const float y = _sin * dir.x + _cos * dir.z;
	dir.x = x;
	dir.z = y;
	_dest = targetPos + dir * (_distance + DUOLMath::MathHelper::RandF(-_randomOffset, _randomOffset));


	if (_ai && _ai->GetGroupController() && _enemyGroupController->GetGroupEnemys().size() > 1) // 일정 호의 크기를 넘어가지 않도록 제한.
	{
		auto targetToCenter = _ai->GetGroupController()->GetGroupCenterPos() - targetPos;
		auto targetToDest = _dest - targetPos;

		targetToCenter.Normalize();
		targetToDest.Normalize();
		float degree = _enemyGroupController->GetGroupEnemys().size() * 5;

		if (degree < 90)
		{
			float centerDotDest = targetToCenter.Dot(targetToDest);

			if (90 - acos(centerDotDest) > degree)
			{
				rad = DUOLMath::MathHelper::DegreeToRadian(degree);
				rad = DUOLMath::MathHelper::RandF(-rad, rad);
				const float cos = cosf(rad);
				const float sin = sinf(rad);
				const float x = cos * targetToCenter.x + (-sin * targetToCenter.z);
				const float y = sin * targetToCenter.x + cos * targetToCenter.z;
				targetToCenter.x = x;
				targetToCenter.z = y;

				_dest = targetPos + targetToCenter * (_distance + DUOLMath::MathHelper::RandF(-_randomOffset, _randomOffset));

				auto myPosToDest = _dest - _transform->GetWorldPosition();

				if (myPosToDest.Length() > 2.0f)
					_dest = _transform->GetWorldPosition() + myPosToDest.Normalized() * 2.5f;

			}
		}
	}

	_dest.y = _transform->GetWorldPosition().y;

	float distance = DUOLMath::Vector3::Distance(_dest, _transform->GetWorldPosition());

	if (distance <= 2.0f)
	{
		_navMeshAgent->SetMaxSpeed(_ai->GetMaxSpeed());
		_navMeshAgent->SetVelocity(DUOLMath::Vector3(0, 0, 0));

		_animator->SetFloat(TEXT("MoveSpeed"), 0);
		_animator->SetBool(TEXT("IsWalkRight"), false);
		_animator->SetBool(TEXT("IsWalkLeft"), false);
		_animator->SetBool(TEXT("IsWalkBack"), false);

		return BT::NodeStatus::SUCCESS;
	}

	const auto lookDotDir = _transform->GetLook().Dot(dir);

	if (90.0f - acos(lookDotDir) > 30.0f) // 좌우 걸음.
	{
		const auto isRight = _transform->GetRight().Dot(dir);

		isRight > 0 ?
			_animator->SetBool(TEXT("IsWalkRight"), true)
			: _animator->SetBool(TEXT("IsWalkLeft"), true);
	}
	else if (lookDotDir > 0) // 정면
		_animator->SetFloat(TEXT("MoveSpeed"), 0.5f);
	else // 뒷걸음
		_animator->SetBool(TEXT("IsWalkBack"), true);

	_navMeshAgent->SetDestination(_dest);

	_navMeshAgent->SetMaxSpeed(2.0f);

	return BT::NodeStatus::RUNNING;
}

BT::NodeStatus DUOLClient::Action_AroundPatrol::onRunning()
{
	if (!_navMeshAgent->GetIsEnabled()) return BT::NodeStatus::FAILURE;

	auto distance = DUOLMath::Vector3::Distance(_targetTransform->GetWorldPosition(), _transform->GetWorldPosition());

	DUOLMath::Vector3 myPosition = _transform->GetWorldPosition();
	DUOLMath::Vector3 targetPos = DUOLMath::Vector3(_targetTransform->GetWorldPosition().x, _transform->GetWorldPosition().y, _targetTransform->GetWorldPosition().z);

	auto look = targetPos - myPosition;

	look.Normalize();

	look = DUOLMath::Vector3::Lerp(_transform->GetLook(), look, 0.5f);

	_transform->LookAt(myPosition + look);

	if (distance >= _distance * 2)
	{
		_navMeshAgent->SetMaxSpeed(_ai->GetMaxSpeed());
		_navMeshAgent->SetVelocity(DUOLMath::Vector3(0, 0, 0));

		_animator->SetFloat(TEXT("MoveSpeed"), 0);
		_animator->SetBool(TEXT("IsWalkRight"), false);
		_animator->SetBool(TEXT("IsWalkLeft"), false);
		_animator->SetBool(TEXT("IsWalkBack"), false);

		return BT::NodeStatus::SUCCESS;
	}

	distance = DUOLMath::Vector3::Distance(_dest, _transform->GetWorldPosition());

	if (distance <= 2.0f)
	{
		_navMeshAgent->SetMaxSpeed(_ai->GetMaxSpeed());
		_navMeshAgent->SetVelocity(DUOLMath::Vector3(0, 0, 0));

		_animator->SetFloat(TEXT("MoveSpeed"), 0);
		_animator->SetBool(TEXT("IsWalkRight"), false);
		_animator->SetBool(TEXT("IsWalkLeft"), false);
		_animator->SetBool(TEXT("IsWalkBack"), false);

		return BT::NodeStatus::SUCCESS;
	}
	else
		return BT::NodeStatus::RUNNING;
}

void DUOLClient::Action_AroundPatrol::onHalted()
{
	if (getInput<AI_EnemyBasic*>("AI").value())
	{
		if (!_navMeshAgent->GetIsEnabled()) return;

		_navMeshAgent->SetMaxSpeed(_ai->GetMaxSpeed());
		_navMeshAgent->SetVelocity(DUOLMath::Vector3(0, 0, 0));

		_animator->SetFloat(TEXT("MoveSpeed"), 0);
		_animator->SetBool(TEXT("IsWalkRight"), false);
		_animator->SetBool(TEXT("IsWalkLeft"), false);
		_animator->SetBool(TEXT("IsWalkBack"), false);
	}
}

BT::PortsList DUOLClient::Action_AroundPatrol::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<DUOLClient::AI_EnemyBasic* >("AI")
	};

	return result;

}
