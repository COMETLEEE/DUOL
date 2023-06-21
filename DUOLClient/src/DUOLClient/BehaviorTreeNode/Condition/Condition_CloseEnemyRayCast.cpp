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

	// ������Ʈ�� �ǹ��� �߿� �־ ���̰� �߹ٴڿ��� ������...! ���� �ɼ� ���� ���� ����..!
	DUOLMath::Vector3 offset;
	offset.y = 1.5f;

	DUOLGameEngine::PhysicsManager::GetInstance()->RaycastAll(tr->GetWorldPosition() + offset, look, distance, hit);

	for (auto& iter : hit) // �浹ü �߿� ���� ��밡 �ƴ� ���� �ִٸ� ������ �ִٴ� ��.
	{
		if (static_cast<DUOLGameEngine::GameObject*>(iter._userData) == _ai->GetTarget()) // ��ǥ�ϴ� Ÿ���̶��,
			continue;
		else if (static_cast<DUOLGameEngine::GameObject*>(iter._userData) == _ai->GetGameObject()) // �����
			continue;
		else if (static_cast<DUOLGameEngine::GameObject*>(iter._userData) == _ai->GetParentGameObject()) // �����
			continue;
		else // ��� �ش����� �ʴ´ٸ�
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
