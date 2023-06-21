#include "DUOLClient/BehaviorTreeNode/Condition/Condition_CloseEnemyRayCast.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/GameObject.h"

DUOLClient::Condition_CloseEnemyRayCast::Condition_CloseEnemyRayCast(const std::string& name,
	const BT::NodeConfig& config) : ConditionNode(name, config), _ai(nullptr)
{
	_ai = getInput<AI_EnemyBasic*>("AI").value();
}


BT::NodeStatus DUOLClient::Condition_CloseEnemyRayCast::tick()
{
	auto tr = _ai->GetTransform();

	auto targetTr = _ai->GetTarget()->GetTransform();

	auto look = targetTr->GetWorldPosition() - tr->GetWorldPosition();

	float distance = look.Length();

	look.Normalize();

	std::vector<DUOLPhysics::RaycastHit> hit;

	// 오브젝트의 피벗이 발에 있어서 레이가 발바닥에서 나간다...! 위로 옵셋 값을 조금 주자..!
	DUOLMath::Vector3 offset;
	offset.y = 1.5f;

	DUOLGameEngine::PhysicsManager::GetInstance()->RaycastAll(tr->GetWorldPosition() + offset, look, distance, hit);

	for (auto& iter : hit) // 충돌체 중에 나와 상대가 아닌 것이 있다면 가리고 있다는 것.
	{
		if (static_cast<DUOLGameEngine::GameObject*>(iter._userData) == _ai->GetTarget()) // 목표하는 타겟이라면,
			continue;
		else if (static_cast<DUOLGameEngine::GameObject*>(iter._userData) == _ai->GetGameObject()) // 나라면
			continue;
		else if (static_cast<DUOLGameEngine::GameObject*>(iter._userData) == _ai->GetParentGameObject()) // 나라면
			continue;
		else // 모두 해당하지 않는다면
			return BT::NodeStatus::FAILURE;
	}

	return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLClient::Condition_CloseEnemyRayCast::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<AI_EnemyBasic*>("AI")
	};
	return result;
}
