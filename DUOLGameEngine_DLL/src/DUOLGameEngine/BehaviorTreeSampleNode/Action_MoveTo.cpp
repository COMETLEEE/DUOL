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

	}

	BT::NodeStatus Action_MoveTo::onStart()
	{
		if (_gameObject == nullptr)
		{
			_gameObject = getInput<GameObject*>("GameObject").value();

			_navMeshAgent = _gameObject->GetComponent<NavMeshAgent>();
		}

		// 매번 초기화를 하는 이유는 타겟이 변경될 가능성이 있기 때문이다.
		if (getInput<Transform*>("TargetTransform").has_value())
			_targetTransform = getInput<Transform*>("TargetTransform").value();
		else
			DUOL_TRACE(DUOL_CONSOLE, "has not data in black borad");

		return BT::NodeStatus::RUNNING;
	}

	BT::NodeStatus Action_MoveTo::onRunning()
	{
		if (_targetTransform == nullptr || _navMeshAgent == nullptr) return BT::NodeStatus::FAILURE;

		if (!_navMeshAgent->GetIsEnabled()) return BT::NodeStatus::FAILURE;

		const DUOLMath::Vector3& targetPos = _targetTransform->GetWorldPosition();

		_navMeshAgent->SetDestination(targetPos);

		// 바라보는 방향은 네비게이션 메쉬의 이동 방향으로 ..!
		DUOLMath::Vector3 velo = _navMeshAgent->GetVelocity();

		velo.Normalize(velo);

		auto myPos = _gameObject->GetTransform()->GetWorldPosition();

		velo.y = 0;

		DUOLMath::Vector3 lookWay = myPos + velo;

		// 에러 제거
		if (lookWay != myPos)
			_gameObject->GetTransform()->LookAt(lookWay);

		if (DUOLMath::Vector3::Distance(myPos, targetPos) >= 2.0f)
			return BT::NodeStatus::RUNNING;

		return BT::NodeStatus::SUCCESS;
	}

	void Action_MoveTo::onHalted()
	{
		if (getInput<GameObject*>("GameObject").value())
			_navMeshAgent->SetVelocity(Vector3(0, 0, 0));
	}

	BT::PortsList Action_MoveTo::providedPorts()
	{
		BT::PortsList result = {
		BT::InputPort<GameObject*>("GameObject"),
		BT::InputPort<Transform*>("TargetTransform")
		};

		return result;
	}
}
