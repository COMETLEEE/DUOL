#include "DUOLClient/BehaviorTreeNode/Action/Action_AroundPatrol.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

BT::NodeStatus DUOLClient::Action_AroundPatrol::onStart()
{
	if (!_gameObject)
	{
		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_targetTransform = getInput<DUOLGameEngine::Transform*>("TargetTransform").value();

		_randomOffset = getInput<float>("RandomOffset").value();

		_distance = getInput<float>("Distance").value();

		_animator = getInput<DUOLGameEngine::Animator*>("Animator").value();

		_navMeshAgent = _gameObject->GetComponent<DUOLGameEngine::NavMeshAgent>();
	}

	if (_targetTransform == nullptr || _navMeshAgent == nullptr) return BT::NodeStatus::FAILURE;

	const DUOLMath::Vector3& targetPos = _targetTransform->GetWorldPosition();

	auto tr = _gameObject->GetTransform();

	auto dir = tr->GetWorldPosition() - targetPos;

	dir.y = 0;

	dir.Normalize();

	auto rad = DUOLMath::MathHelper::RandF(-0.7f, 0.7f);

	const float _cos = cosf(rad);
	const float _sin = sinf(rad);

	const float x = _cos * dir.x + (-_sin * dir.z);
	const float y = _sin * dir.x + _cos * dir.z;

	dir.x = x;
	dir.z = y;

	_dest = targetPos + dir * (_distance + DUOLMath::MathHelper::RandF(-_randomOffset, _randomOffset));

	const auto lookDotDir = tr->GetLook().Dot(dir);

	if (abs(lookDotDir) > 0.4f) // ÁÂ¿ì °ÉÀ½.
	{
		const auto isRight = tr->GetRight().Dot(dir);

		isRight > 0 ?
			_animator->SetBool(TEXT("IsWalkRight"), true)
			: _animator->SetBool(TEXT("IsWalkLeft"), true);
	}
	else if (lookDotDir > 0) // Á¤¸é
		_animator->SetFloat(TEXT("MoveSpeed"), 0.5f);
	else // µÞ°ÉÀ½
		_animator->SetBool(TEXT("IsWalkBack"), true);

	_navMeshAgent->SetDestination(_dest);

	_navMeshAgent->SetMaxSpeed(2.0f);



	return BT::NodeStatus::RUNNING;
}

BT::NodeStatus DUOLClient::Action_AroundPatrol::onRunning()
{
	auto distance = DUOLMath::Vector3::Distance(_targetTransform->GetWorldPosition(), _gameObject->GetTransform()->GetWorldPosition());

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
		_navMeshAgent->SetMaxSpeed(3.5f);
		_navMeshAgent->SetVelocity(DUOLMath::Vector3(0, 0, 0));
	}
}

BT::PortsList DUOLClient::Action_AroundPatrol::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
		BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform"),
		BT::InputPort<float>("RandomOffset"),
		BT::InputPort<float>("Distance"),
		BT::InputPort<DUOLGameEngine::Animator*>("Animator")
	};

	return result;

}
