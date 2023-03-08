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
		// 노드의 생성자 호출은 트리를 생성하고 바로 호출한다.

	}

	BT::NodeStatus Action_MoveDirectlyToward::onRunning()
	{
		if (_gameObject == nullptr)
		{
			_gameObject = getInput<GameObject*>("GameObject").value();

			// 값이 할당되어 있는 경우에 생성자에서 사용 가능.
			_targetPos = getInput<DUOLMath::Vector3>("TargetPosition").value();

			// 값이 할당되어 있는 경우에 생성자에서 사용 가능.
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
