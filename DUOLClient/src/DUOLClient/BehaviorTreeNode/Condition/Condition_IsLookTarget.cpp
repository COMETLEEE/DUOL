#include "DUOLClient/BehaviorTreeNode/Condition/Condition_IsLookTarget.h"

#include "DUOLClient/ECS/Component/AI_Enemy.h"
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

			_ai = _gameObject->GetComponent<AI_Enemy>();
		}

		auto tr = _gameObject->GetTransform();

		if (_targetTransform)
		{
			if (DUOLMath::Vector3::Distance(_targetTransform->GetWorldPosition(), tr->GetWorldPosition()) > _range) // �Ÿ� ���̶��..
				return BT::NodeStatus::FAILURE;

			auto look = _targetTransform->GetWorldPosition() - tr->GetWorldPosition();
			look.Normalize();

			if (look.Dot(tr->GetLook()) > 0) // Ÿ���� �þ߿� ���´ٸ�... ������ �þ߿� ���� ���̴�..!
			{
				_ai->SetIsGroupCheck();
				return BT::NodeStatus::SUCCESS;
			}
			else // ������ �ʴ´ٸ�
				return BT::NodeStatus::FAILURE;
		}
		return BT::NodeStatus::FAILURE;
	}

	BT::PortsList Condition_IsLookTarget::providedPorts()
	{
		BT::PortsList result = {
		BT::InputPort<DUOLGameEngine::GameObject*>("GameObject"),
		BT::InputPort<DUOLGameEngine::Transform*>("TargetTransform"),
		BT::InputPort<float>("Range")
		};

		return result;
	}
}
