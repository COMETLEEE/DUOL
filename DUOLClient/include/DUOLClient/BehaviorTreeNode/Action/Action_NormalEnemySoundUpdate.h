/**

	@file      Action_NormalEnemySoundUpdate.h
	@brief     Action_NormalEnemySoundUpdate 몬스터의 사운드를 업데이트 시켜주는 노드.
	@details   ~ 특정 프레임에서 사운드를 재생시켜야하는 경우는 애니메이션 이벤트를 통해 처리한다.
	@author    SinSeongHyeon
	@date      26.06.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/

#pragma once

#include "behaviortree_cpp/action_node.h"

namespace DUOLGameEngine
{
	class Animator;
	class AudioSource;
}

namespace DUOLClient
{
	class AI_EnemyBasic;
	class Action_NormalEnemySoundUpdate : public BT::SyncActionNode
	{
	public:
		Action_NormalEnemySoundUpdate(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_NormalEnemySoundUpdate() override = default;

	private:
		DUOLClient::AI_EnemyBasic* _ai;

		DUOLGameEngine::AudioSource* _audioSource;

		DUOLGameEngine::AudioSource* _voiceAudioSource;

		DUOLGameEngine::Animator* _animator;

		float _soundCoolTime;

		const float _initSoundCoolTime;
	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}
