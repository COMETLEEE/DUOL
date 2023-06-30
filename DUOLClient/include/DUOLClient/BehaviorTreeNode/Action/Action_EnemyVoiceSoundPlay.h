/**

	@file      Action_EnemySoundPlay.h
	@brief     Action_EnemySoundPlay 몬스터의 특정 사운드를 재생하기 위한 노드
	@details   ~
	@author    ShinSeongHyeon
	@date      30.06.2023
	@copyright © ShinSeongHyeon, 2023. All right reserved.

**/

#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLClient
{
	enum class EnemyAudioEnum;

	class AI_EnemyBasic;

	class Action_EnemyVoiceSoundPlay : public BT::SyncActionNode
	{
	public:
		Action_EnemyVoiceSoundPlay(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_EnemyVoiceSoundPlay() override = default;

	private:
		DUOLClient::AI_EnemyBasic* _ai;

		EnemyAudioEnum _audioEnum;

		bool _isOverride;

		int _offset;
	public:
		BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}

