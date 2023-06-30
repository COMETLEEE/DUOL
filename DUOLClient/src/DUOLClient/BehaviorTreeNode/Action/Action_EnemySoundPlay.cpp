#include "DUOLClient/BehaviorTreeNode/Action/Action_EnemySoundPlay.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/Component/AudioSource.h"
#include "DUOLClient/Manager/EnemyAudioManager.h"
#include "DUOLGameEngine/ECS/Object/AudioClip.h"

namespace DUOLClient
{
	DUOLClient::Action_EnemySoundPlay::Action_EnemySoundPlay(const std::string& name, const BT::NodeConfig& config) :
		SyncActionNode(name, config)
	{
		_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();

		_audioEnum = static_cast<EnemyAudioEnum>(getInput<int>("AudioEnumNumber").value());

		if (_audioEnum == EnemyAudioEnum::None)
			_audioEnum = static_cast<EnemyAudioEnum>(static_cast<int>(_ai->GetParameter<float>(TEXT("LockOnSound"))));

		_isOverride = getInput<bool>("IsOverride").value();

		_offset = getInput<int>("Offset").value();

	}

	BT::NodeStatus DUOLClient::Action_EnemySoundPlay::tick()
	{
		_ai->PlaySound(_audioEnum, _isOverride, _offset);

		return BT::NodeStatus::SUCCESS;
	}

	BT::PortsList DUOLClient::Action_EnemySoundPlay::providedPorts()
	{
		BT::PortsList result = {
			BT::InputPort<DUOLClient::AI_EnemyBasic*>("AI"),
			BT::InputPort<int>("AudioEnumNumber"),
			BT::InputPort<bool>("IsOverride"),
			BT::InputPort<int>("Offset")
		};
		return result;
	}
}
