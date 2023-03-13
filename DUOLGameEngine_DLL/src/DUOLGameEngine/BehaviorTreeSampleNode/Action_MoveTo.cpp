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

		// �Ź� �ʱ�ȭ�� �ϴ� ������ Ÿ���� ����� ���ɼ��� �ֱ� �����̴�.
		if (getInput<Transform*>("TargetTransform").has_value())
			_targetTransform = getInput<Transform*>("TargetTransform").value();
		else
			DUOL_TRACE(DUOL_CONSOLE, "not data in black borad");

		if (_navMeshAgent)
			_navMeshAgent->SetIsEnabled(true);

		return BT::NodeStatus::RUNNING;
	}

	BT::NodeStatus Action_MoveTo::onRunning()
	{
		if (_targetTransform == nullptr || _navMeshAgent == nullptr) return BT::NodeStatus::SUCCESS;

		const DUOLMath::Vector3& targetPos = _targetTransform->GetWorldPosition(); //!< 

		_navMeshAgent->SetDestination(targetPos);

		// �ٶ󺸴� ������ �׺���̼� �޽��� �̵� �������� ..!
		DUOLMath::Vector3 velo = _navMeshAgent->GetVelocity();

		velo.Normalize(velo);

		auto myPos = _gameObject->GetTransform()->GetWorldPosition();

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
		_navMeshAgent->SetVelocity(Vector3(0, 0, 0));

		//_navMeshAgent->SetDestination(_gameObject->GetTransform()->GetWorldPosition() + _gameObject->GetTransform()->GetLook());
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