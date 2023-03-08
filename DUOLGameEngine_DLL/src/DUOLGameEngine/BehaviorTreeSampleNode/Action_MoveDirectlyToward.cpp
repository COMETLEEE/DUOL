#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_MoveDirectlyToward.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLMath/DUOLMath.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

#include "DUOLGameEngine/Util/BehaviorTreeTypeConvert.h"

namespace DUOLGameEngine
{
	Action_MoveDirectlyToward::Action_MoveDirectlyToward(const std::string& name, const BT::NodeConfig& config) :
		StatefulActionNode(name, config), _gameObject(nullptr)
	{
		// ����� ������ ȣ���� Ʈ���� �����ϰ� �ٷ� ȣ���Ѵ�.

	}

	BT::NodeStatus Action_MoveDirectlyToward::onRunning()
	{
		if (_gameObject == nullptr)
		{
			_gameObject = getInput<GameObject*>("GameObject").value();

			// ���� �Ҵ�Ǿ� �ִ� ��쿡 �����ڿ��� ��� ����.
			_targetPos = getInput<DUOLMath::Vector3>("TargetPosition").value();

			// ���� �Ҵ�Ǿ� �ִ� ��쿡 �����ڿ��� ��� ����.
			_speed = getInput<float>("Speed").value();
		}

		float distance = DUOLMath::Vector3::Distance(_gameObject->GetTransform()->GetWorldPosition(), _targetPos);

		if (abs(distance) < 0.1f)
			return BT::NodeStatus::SUCCESS;

		auto dir = _targetPos - _gameObject->GetTransform()->GetWorldPosition();
		dir.Normalize();

		auto newPos = _gameObject->GetTransform()->GetWorldPosition() + _speed * dir * TimeManager::GetInstance()->GetDeltaTime();

		_gameObject->GetTransform()->SetPosition(newPos);

		return BT::NodeStatus::RUNNING;
	}

	BT::PortsList Action_MoveDirectlyToward::providedPorts()
	{
		BT::PortsList result = {
			BT::InputPort<GameObject*>("GameObject"),
			BT::InputPort<float>("Speed"),
			BT::InputPort<DUOLMath::Vector3>("TargetPosition")
		};

		return result;
	}
}
