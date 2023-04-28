#include "DUOLClient/BehaviorTreeNode/Action/Action_AroundPatrol.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"

BT::NodeStatus DUOLClient::Action_AroundPatrol::onStart()
{
	if (!_gameObject)
	{
		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_targetTransform = getInput<DUOLGameEngine::Transform*>("TargetTransform").value();

		_randomOffset = getInput<float>("RandomOffset").value();

		_distance = getInput<float>("Distance").value();

		_animator = getInput<DUOLGameEngine::Animator*>("Animator").value();

		_navMeshAgent = getInput<DUOLGameEngine::NavMeshAgent*>("NavMeshAgent").value();

		_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();
	}

	if (_targetTransform == nullptr || _navMeshAgent == nullptr || !_navMeshAgent->GetIsEnabled()) return BT::NodeStatus::FAILURE;

	const DUOLMath::Vector3& targetPos = _targetTransform->GetWorldPosition();

	auto tr = _gameObject->GetTransform();

	auto dir = tr->GetWorldPosition() - targetPos;

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


	if (_ai && _ai->GetGroupController()) // 일정 호의 크기를 넘어가지 않도록 제한.
	{
		auto targetToCenter = _ai->GetGroupController()->GetGroupCenterPos() - targetPos;
		auto targetToDest = _dest - targetPos;

		targetToCenter.Normalize();
		targetToDest.Normalize();

		if (targetToCenter.Dot(targetToDest) <= 0)
		{
			const float _cos = cosf(-rad);
			const float _sin = sinf(-rad);
			const float x = _cos * dir.x + (-_sin * dir.z);
			const float y = _sin * dir.x + _cos * dir.z;
			dir.x = x;
			dir.z = y;
			_dest = targetPos + dir * (_distance + DUOLMath::MathHelper::RandF(-_randomOffset, _randomOffset));
		}
	}
	const auto lookDotDir = tr->GetLook().Dot(dir);

	if (abs(lookDotDir) > 0.4f) // 좌우 걸음.
	{
		const auto isRight = tr->GetRight().Dot(dir);

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

	auto distance = DUOLMath::Vector3::Distance(_targetTransform->GetWorldPosition(), _gameObject->GetTransform()->GetWorldPosition());

	DUOLMath::Vector3 myPosition = _gameObject->GetTransform()->GetWorldPosition();
	DUOLMath::Vector3 targetPos = DUOLMath::Vector3(_targetTransform->GetWorldPosition().x, _gameObject->GetTransform()->GetWorldPosition().y, _targetTransform->GetWorldPosition().z);

	auto look = targetPos - myPosition;

	look.Normalize();

	look = DUOLMath::Vector3::Lerp(_gameObject->GetTransform()->GetLook(), look, 0.5f);

	_gameObject->GetTransform()->LookAt(myPosition + look);

	if (distance >= _distance * 2)
	{
		_navMeshAgent->SetMaxSpeed(3.5f);
		_navMeshAgent->SetVelocity(DUOLMath::Vector3(0, 0, 0));

		_animator->SetFloat(TEXT("MoveSpeed"), 0);
		_animator->SetBool(TEXT("IsWalkRight"), false);
		_animator->SetBool(TEXT("IsWalkLeft"), false);
		_animator->SetBool(TEXT("IsWalkBack"), false);

		return BT::NodeStatus::SUCCESS;
	}

	distance = DUOLMath::Vector3::Distance(_dest, _gameObject->GetTransform()->GetWorldPosition());

	if (distance <= 2.0f)
	{
		_navMeshAgent->SetMaxSpeed(3.5f);
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
	if (getInput<DUOLGameEngine::GameObject*>("GameObject").value())
	{
		if (!_navMeshAgent->GetIsEnabled()) return;

		_navMeshAgent->SetMaxSpeed(3.5f);
		_navMeshAgent->SetVelocity(DUOLMath::Vector3(0, 0, 0));
	}
}

BT::PortsList DUOLClient::Action_AroundPatrol::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<DUOLClient::AI_EnemyBasic* >("AI"),
		BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
		BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform"),
		BT::InputPort<float>("RandomOffset"),
		BT::InputPort<float>("Distance"),
		BT::InputPort<DUOLGameEngine::Animator*>("Animator"),
		BT::InputPort<DUOLGameEngine::NavMeshAgent*>("NavMeshAgent")
	};

	return result;

}
