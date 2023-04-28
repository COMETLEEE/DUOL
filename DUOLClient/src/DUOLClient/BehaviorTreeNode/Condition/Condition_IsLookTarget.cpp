#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsLookTarget.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLClient
{
	BT::NodeStatus Condition_IsLookTarget::tick()
	{
		if (!_gameObject)
		{
			_gameObject = getInput<DUOLGameEngine::GameObject*>("GameObject").value();

			_targetTransform = getInput<DUOLGameEngine::Transform*>("TargetTransform").value();

			_range = getInput<float>("Range").value();

			_ai = getInput<AI_EnemyBasic*>("AI").value();
		}

		auto tr = _gameObject->GetTransform();

		if (_targetTransform)
		{
			if (DUOLMath::Vector3::Distance(_targetTransform->GetWorldPosition(), tr->GetWorldPosition()) > _range) // �Ÿ� ���̶��..
				return BT::NodeStatus::FAILURE;

			auto look = _targetTransform->GetWorldPosition() - tr->GetWorldPosition();

			float distance = look.Length();

			look.Normalize();

			if (look.Dot(tr->GetLook()) > 0) // Ÿ���� �þ߿� ���´ٸ�... ������ �þ߿� ���� ���̴�..!
			{
				std::vector<DUOLPhysics::RaycastHit> hit;

				// ������Ʈ�� �ǹ��� �߿� �־ ���̰� �߹ٴڿ��� ������...! ���� �ɼ� ���� ���� ����..!
				DUOLMath::Vector3 offset;
				offset.y = 1.5f;

				DUOLGameEngine::PhysicsManager::GetInstance()->RaycastAll(tr->GetWorldPosition() + offset, look, distance, hit);

				bool isBlock = false;

				for (auto& iter : hit) // �浹ü �߿� ���� ��밡 �ƴ� ���� �ִٸ� ������ �ִٴ� ��.
				{
					if (static_cast<DUOLGameEngine::GameObject*>(iter._userData) != _gameObject &&
						static_cast<DUOLGameEngine::GameObject*>(iter._userData) != _targetTransform->GetGameObject() &&
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

			if (distance <= 5) // �ʹ� ������
			{
				std::vector<DUOLPhysics::RaycastHit> hit;

				// ������Ʈ�� �ǹ��� �߿� �־ ���̰� �߹ٴڿ��� ������...! ���� �ɼ� ���� ���� ����..!
				DUOLMath::Vector3 offset;
				offset.y = 1.5f;

				DUOLGameEngine::PhysicsManager::GetInstance()->RaycastAll(tr->GetWorldPosition() + offset, look, distance, hit);

				bool isBlock = false;

				for (auto& iter : hit) // �浹ü �߿� ���� ��밡 �ƴ� ���� �ִٸ� ������ �ִٴ� ��.
				{
					if (static_cast<DUOLGameEngine::GameObject*>(iter._userData) != _gameObject &&
						static_cast<DUOLGameEngine::GameObject*>(iter._userData) != _targetTransform->GetGameObject() &&
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
		BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
		BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform"),
		BT::InputPort<float>("Range"),
		BT::InputPort<AI_EnemyBasic*>("AI")
		};

		return result;
	}
}
