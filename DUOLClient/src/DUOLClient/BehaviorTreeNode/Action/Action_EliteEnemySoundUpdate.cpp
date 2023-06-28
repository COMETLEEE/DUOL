#include "DUOLClient/BehaviorTreeNode/Action/Action_EliteEnemySoundUpdate.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLClient/Manager/EnemyAudioManager.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/AudioSource.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Object/AudioClip.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

DUOLClient::Action_EliteEnemySoundUpdate::Action_EliteEnemySoundUpdate(const std::string& name,
                                                                       const BT::NodeConfig& config) :
	SyncActionNode(name, config),
	_initSoundCoolTime(3.0f)
{
	_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();

	_audioSource = _ai->GetAudioSource();

	_voiceAudioSource = _ai->GetVoiceAudioSource();

	_animator = _ai->GetAnimator();

	_soundCoolTime = _initSoundCoolTime;
}

BT::NodeStatus DUOLClient::Action_EliteEnemySoundUpdate::tick()
{
	_soundCoolTime -= DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

	if (_soundCoolTime > 0)
		return BT::NodeStatus::SUCCESS;

	bool isPlay = false;

	if (!_ai->GetGroupController())
		return BT::NodeStatus::SUCCESS;

	const float timer = _ai->GetGroupController()->GetIdleSoundTimer();

	_audioSource->IsPlaying(isPlay);

	if (isPlay)
		return BT::NodeStatus::SUCCESS;

	_voiceAudioSource->IsPlaying(isPlay);

	if (_ai->GetGroupController()->GetIsPrevHit())
	{
		if (isPlay)
		{
			auto audioName = _voiceAudioSource->GetAudioClip()->GetName();

			if (audioName == TEXT("EliteMonster_Idle01") || audioName == TEXT("EliteMonster_Idle02"))
				_voiceAudioSource->Stop();
		}

		return BT::NodeStatus::SUCCESS;
	}

	if (isPlay)
		return BT::NodeStatus::SUCCESS;

	if (timer > 0)
		return BT::NodeStatus::SUCCESS;

	auto distance = DUOLMath::Vector3::Distance(_ai->GetTargetTransform()->GetWorldPosition(), _ai->GetParentTransform()->GetWorldPosition());

	if (distance > 30.0f)
		return BT::NodeStatus::SUCCESS;

	_ai->GetGroupController()->ResetIdleSoundTimer();
	// 애니메이션 스테이트를 검사할 필요는 없다...!
	// 일반 몬스터의 경우 이동, 아이들, 백점프 사운드가 같기 때문,.

	int randInt = DUOLMath::MathHelper::Rand(0, 1);

	int randOffset = DUOLMath::MathHelper::Rand(0, 500);

	if (randInt == 0)
		_ai->PlaySound(EnemyAudioEnum::EliteMonster_idle01, false, randOffset);
	else
		_ai->PlaySound(EnemyAudioEnum::EliteMonster_idle02, false, randOffset);

	_soundCoolTime = _initSoundCoolTime;

	return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLClient::Action_EliteEnemySoundUpdate::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLClient::AI_EnemyBasic*>("AI")
	};

	return result;
}
