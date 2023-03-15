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
	DUOL_TRACE(DUOL_CONSOLE, " 공격 ");

	_ai->UseToken();

	// 공격 애니메이션이 끝나면 Success 반환.
	// 아직 애니메이션 컨트롤러 사용법을 잘 모르겠다.
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
