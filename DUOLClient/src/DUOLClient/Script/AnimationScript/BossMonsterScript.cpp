#include "DUOLClient/Script/AnimationScript/BossMonsterScript.h"


#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

#include <rttr/registration>

#include "DUOLClient/Script/AnimationScript/DummyMonoBehavior.h"
#include "DUOLClient/ECS/Component/ParticleData.h"
#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Component/AudioSource.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

DUOLClient::BossMonsterScript::BossMonsterScript(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name):
	MonoBehaviourBase(owner, name)
{
}

DUOLClient::BossMonsterScript::~BossMonsterScript()
{
}

void DUOLClient::BossMonsterScript::OnAwake()
{
	auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

	//플레이어 더미 생성
	_cachedPlayerDummy = scene->CreateFromFBXModel(TEXT("player"));
	_cachedPlayerDummy->GetTransform()->SetParent(this->GetTransform(), false);
	auto playerEventHandle = _cachedPlayerDummy->AddComponent<DummyMonoBehavior>();
	_cachedPlayerAnimator = _cachedPlayerDummy->GetComponent<DUOLGameEngine::Animator>();
	_cachedPlayerAudioSource = _cachedPlayerDummy->AddComponent<DUOLGameEngine::AudioSource>();

	//_cashedEliteMonsterAnimator->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(_T("Elite_Monster_Generate_Animator")));
	//_cashedEliteMonsterDummy->SetIsActiveSelf(false);

	//eventHandle->AddEventFunction(TEXT("JumpAttackStart"), [this]()
	//	{
	//		_isJumping = true;
	//	});
	//eventHandle->AddEventFunction(TEXT("JumpAttackEnd"), [this]()
	//	{
	//		_cashedEliteMonsterAnimator->SetBool(TEXT("IsJumpAttack"), false);

	//auto particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterBigCrack, 5.f);
	//auto particleTranform = particleRenderer->GetTransform();
	//particleTranform->SetPosition(GetTransform()->GetWorldPosition());
	//_audioSource->SetAudioClip(_groundAttack);
	//_audioSource->Play();
	//return;
	//	});

	//_audioSource->SetIsLoop(false);

	//_groundAttack = DUOLGameEngine::SoundManager::GetInstance()->GetAudioClip(TEXT("MidBossCrack"));
	//_roarClip = DUOLGameEngine::SoundManager::GetInstance()->GetAudioClip(TEXT("MiddleBossRoar"));


}

void DUOLClient::BossMonsterScript::OnStart()
{
}

void DUOLClient::BossMonsterScript::OnDestroy()
{
}

void DUOLClient::BossMonsterScript::OnUpdate(float deltaTime)
{
}

DUOLGameEngine::CoroutineHandler DUOLClient::BossMonsterScript::AnimationStart()
{
}
