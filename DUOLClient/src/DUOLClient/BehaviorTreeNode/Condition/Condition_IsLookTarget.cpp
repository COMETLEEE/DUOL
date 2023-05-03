#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsLookTarget.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLClient
{
	BT::NodeStatus Condition_IsLookTarget::tick()
	{
		if (!_ai)
			_ai = getInput<AI_EnemyBasic*>("AI").value();

		auto tr = _ai->GetParentTransform();

		auto targetTranform = _ai->GetTargetTransform();

		if (targetTranform)
		{
			if (DUOLMath::Vector3::Distance(targetTranform->GetWorldPosition(), tr->GetWorldPosition()) > _ai->GetLookRange()) // 거리 밖이라면..
				return BT::NodeStatus::FAILURE;

			auto look = targetTranform->GetWorldPosition() - tr->GetWorldPosition();

			float distance = look.Length();

			look.Normalize();

			if (look.Dot(tr->GetLook()) > 0) // 타겟이 시야에 들어온다면... 양수라면 시야에 들어온 것이다..!
			{
				std::vector<DUOLPhysics::RaycastHit> hit;

				// 오브젝트의 피벗이 발에 있어서 레이가 발바닥에서 나간다...! 위로 옵셋 값을 조금 주자..!
				DUOLMath::Vector3 offset;
				offset.y = 1.5f;

				DUOLGameEngine::PhysicsManager::GetInstance()->RaycastAll(tr->GetWorldPosition() + offset, look, distance, hit);

				bool isBlock = false;

				for (auto& iter : hit) // 충돌체 중에 나와 상대가 아닌 것이 있다면 가리고 있다는 것.
				{
					if (
						static_cast<DUOLGameEngine::GameObject*>(iter._userData) != _ai->GetTarget() &&
						static_cast<DUOLGameEngine::GameObject*>(iter._userData)->GetLayer() != TEXT("EnemyRigidbody") &&
						static_cast<DUOLGameEngine::GameObject*>(iter._userData)->GetLayer() != TEXT("EnemyBottomCheck") &&
						static_cast<DUOLGameEngine::GameObject*>(iter._userData)->GetLayer() != TEXT("Enemy")
						)
					{
						isBlock = true;
						break;
					}
				}

				if (!isBlock)
				{
					_ai->SetIsGroupCheck();
					return BT::NodeStatus::SUCCESS;
				}
			}

			if (distance <= 5) // 너무 가까우면
			{
				std::vector<DUOLPhysics::RaycastHit> hit;

				// 오브젝트의 피벗이 발에 있어서 레이가 발바닥에서 나간다...! 위로 옵셋 값을 조금 주자..!
				DUOLMath::Vector3 offset;
				offset.y = 1.5f;

				DUOLGameEngine::PhysicsManager::GetInstance()->RaycastAll(tr->GetWorldPosition() + offset, look, distance, hit);

				bool isBlock = false;

				for (auto& iter : hit) // 충돌체 중에 나와 상대가 아닌 것이 있다면 가리고 있다는 것.
				{
					if (
						static_cast<DUOLGameEngine::GameObject*>(iter._userData) != _ai->GetTarget() &&
						static_cast<DUOLGameEngine::GameObject*>(iter._userData)->GetLayer() != TEXT("EnemyRigidbody") &&
						static_cast<DUOLGameEngine::GameObject*>(iter._userData)->GetLayer() != TEXT("EnemyBottomCheck") &&
						static_cast<DUOLGameEngine::GameObject*>(iter._userData)->GetLayer() != TEXT("Enemy")
						)
					{
						isBlock = true;
						break;
					}
				}

				if (!isBlock)
				{
					_ai->SetIsGroupCheck();
					return BT::NodeStatus::SUCCESS;
				}
			}
		}

		return BT::NodeStatus::FAILURE;
	}


	BT::PortsList Condition_IsLookTarget::providedPorts()
	{
		BT::PortsList result = {
		BT::InputPort<AI_EnemyBasic*>("AI")
		};

		return result;
	}
}
