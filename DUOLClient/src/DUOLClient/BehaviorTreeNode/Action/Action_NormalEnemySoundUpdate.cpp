#include "DUOLClient/BehaviorTreeNode/Action/Action_NormalEnemySoundUpdate.h"

#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLClient/Manager/EnemyAudioManager.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/AudioSource.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"

DUOLClient::Action_NormalEnemySoundUpdate::Action_NormalEnemySoundUpdate(const std::string& name, const BT::NodeConfig& config) :
	SyncActionNode(name, config)
{
	_ai = getInput<DUOLClient::AI_EnemyBasic*>("AI").value();

	_audioSource = _ai->GetAudioSource();

	_voiceAudioSource = _ai->GetVoiceAudioSource();

	_animator = _ai->GetAnimator();
}

BT::NodeStatus DUOLClient::Action_NormalEnemySoundUpdate::tick()
{
	bool isPlay = false;

	_audioSource->IsPlaying(isPlay);

	if (isPlay)
		return BT::NodeStatus::SUCCESS;

	_voiceAudioSource->IsPlaying(isPlay);

	if (isPlay)
		return BT::NodeStatus::SUCCESS;

	float timer = _ai->GetGroupController()->GetIdleSoundTimer();

	if (timer > 0)
		return BT::NodeStatus::SUCCESS;

	auto distance = DUOLMath::Vector3::Distance(_ai->GetTargetTransform()->GetWorldPosition(), _ai->GetParentTransform()->GetWorldPosition());

	if (distance > 30.0f)
		return BT::NodeStatus::SUCCESS;

	_ai->GetGroupController()->ResetIdleSoundTimer();
	// �ִϸ��̼� ������Ʈ�� �˻��� �ʿ�� ����...!
	// �Ϲ� ������ ��� �̵�, ���̵�, ������ ���尡 ���� ����,.

	int randInt = DUOLMath::MathHelper::Rand(0, 1);

	int randOffset = DUOLMath::MathHelper::Rand(0, 500);

	if (randInt == 0)
		_ai->PlaySound(EnemyAudioEnum::Monster01, false, randOffset);
	else
		_ai->PlaySound(EnemyAudioEnum::Monster02, false, randOffset);

	return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLClient::Action_NormalEnemySoundUpdate::providedPorts()
{
	BT::PortsList result = {
	BT::InputPort<DUOLClient::AI_EnemyBasic*>("AI")
	};

	return result;
}
