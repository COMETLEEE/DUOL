#include "DUOLClient/BehaviorTreeNode/Action/Action_UseTokenAttack.h"

#include "DUOLClient/ECS/Component/AI_Enemy.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

BT::NodeStatus DUOLClient::Action_UseTokenAttack::onStart()
{
	if (!_gameObject)
	{
		_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

		_targetTransform = getInput<DUOLGameEngine::Transform*>("TargetTransform").value();

		_navMeshAgent = _gameObject->GetComponent<DUOLGameEngine::NavMeshAgent>();

		_ai = _gameObject->GetComponent<AI_Enemy>();
	}

	return BT::NodeStatus::RUNNING;
}

BT::NodeStatus DUOLClient::Action_UseTokenAttack::onRunning()
{
	DUOL_TRACE(DUOL_CONSOLE, " ���� ");

	_ai->UseToken();

	// ���� �ִϸ��̼��� ������ Success ��ȯ.
	// ���� �ִϸ��̼� ��Ʈ�ѷ� ������ �� �𸣰ڴ�.
	return BT::NodeStatus::SUCCESS;
}

void DUOLClient::Action_UseTokenAttack::onHalted()
{
}

BT::PortsList DUOLClient::Action_UseTokenAttack::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
	BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform")
	};
	return result;
}
