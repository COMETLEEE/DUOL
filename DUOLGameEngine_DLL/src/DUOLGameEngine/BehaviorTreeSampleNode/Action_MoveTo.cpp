#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_MoveTo.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

#include "DUOLGameEngine/Util/BehaviorTreeTypeConvert.h"

namespace DUOLGameEngine
{
	Action_MoveTo::Action_MoveTo(const std::string& name, const BT::NodeConfig& config) :
		StatefulActionNode(name, config), _gameObject(nullptr), _targetTransform(nullptr),
		_navMeshAgent(nullptr)
	{
		_gameObject = getInput<GameObject*>("GameObject").value();

		_navMeshAgent = getInput<NavMeshAgent*>("NavMeshAgent").value();

		_targetTransform = getInput<Transform*>("TargetTransform").value();
	}

	BT::NodeStatus Action_MoveTo::onStart()
	{
		return BT::NodeStatus::RUNNING;
	}

	BT::NodeStatus Action_MoveTo::onRunning()
	{
		if (_targetTransform == nullptr || _navMeshAgent == nullptr) return BT::NodeStatus::FAILURE;

		if (!_navMeshAgent->GetIsEnabled()) return BT::NodeStatus::FAILURE;

		const DUOLMath::Vector3& targetPos = _targetTransform->GetWorldPosition();

		_navMeshAgent->SetDestination(targetPos);

		// �ٶ󺸴� ������ �׺���̼� �޽��� �̵� �������� ..!
		DUOLMath::Vector3 velo = _navMeshAgent->GetVelocity();

		velo.Normalize(velo);

		auto myPos = _gameObject->GetTransform()->GetWorldPosition();

		velo.y = 0;

		DUOLMath::Vector3 lookWay = myPos + velo;

		// ���� ����
		if (lookWay != myPos)
			_gameObject->GetTransform()->LookAt(lookWay);

		if (DUOLMath::Vector3::Distance(myPos, targetPos) >= 2.0f)
			return BT::NodeStatus::RUNNING;

		return BT::NodeStatus::SUCCESS;
	}

	void Action_MoveTo::onHalted()
	{
		if (getInput<GameObject*>("GameObject").value() != nullptr)
		{
			if (_navMeshAgent->GetIsEnabled())
				_navMeshAgent->SetVelocity(Vector3(0, 0, 0));
		}
	}

	BT::PortsList Action_MoveTo::providedPorts()
	{
		BT::PortsList result = {
		BT::InputPort<GameObject*>("GameObject"),
		BT::InputPort<Transform*>("TargetTransform"),
		BT::InputPort<NavMeshAgent*>("NavMeshAgent")
		};

		return result;
	}
}
