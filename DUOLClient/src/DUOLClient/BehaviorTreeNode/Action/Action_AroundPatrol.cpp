#include "DUOLClient/BehaviorTreeNode/Action/Action_AroundPatrol.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

BT::NodeStatus DUOLClient::Action_AroundPatrol::onStart()
{
	if (!_gameObject)
	{
		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_targetTransform = getInput<DUOLGameEngine::Transform*>("TargetTransform").value();

		_randomOffset = getInput<float>("RandomOffset").value();

		_distance = getInput<float>("Distance").value();

		_navMeshAgent = _gameObject->GetComponent<DUOLGameEngine::NavMeshAgent>();
	}

	if (_targetTransform == nullptr || _navMeshAgent == nullptr) return BT::NodeStatus::FAILURE;

	const DUOLMath::Vector3& targetPos = _targetTransform->GetWorldPosition();

	auto tr = _gameObject->GetTransform();

	auto dir = tr->GetWorldPosition() - targetPos;

	dir.y = 0;

	dir.Normalize();

	auto rad = DUOLMath::MathHelper::RandF(-0.5f, 0.5f);

	float _cos = cosf(rad);
	float _sin = sinf(rad);

	float x = _cos * dir.x + (-_sin * dir.z);
	float y = _sin * dir.x + _cos * dir.z;

	dir.x = x;
	dir.z = y;

	dir *= (_distance + DUOLMath::MathHelper::RandF(-_randomOffset, _randomOffset));

	_dest = targetPos + dir;

	_navMeshAgent->SetDestination(_dest);


	return BT::NodeStatus::RUNNING;
}

BT::NodeStatus DUOLClient::Action_AroundPatrol::onRunning()
{
	auto distance = DUOLMath::Vector3::Distance(_targetTransform->GetWorldPosition(), _gameObject->GetTransform()->GetWorldPosition());

	if (distance >= _distance * 2)
		return BT::NodeStatus::SUCCESS;

	distance = DUOLMath::Vector3::Distance(_dest, _gameObject->GetTransform()->GetWorldPosition());

	if (distance <= 2.0f)
		return BT::NodeStatus::SUCCESS;
	else
		return BT::NodeStatus::RUNNING;
}

void DUOLClient::Action_AroundPatrol::onHalted()
{
	if (getInput<DUOLGameEngine::GameObject*>("GameObject").value())
		_navMeshAgent->SetDestination(DUOLMath::Vector3(0, 0, 0));
}

BT::PortsList DUOLClient::Action_AroundPatrol::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
		BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform"),
		BT::InputPort<float>("RandomOffset"),
		BT::InputPort<float>("Distance")
	};

	return result;

}
