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

		_ai = getInput<AI_Enemy*>("AI").value();

		_navMeshAgent = _ai->GetNavMeshAgent();
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
	BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform"),
	BT::InputPort<AI_Enemy*>("AI")
	};
	return result;
}