#include "DUOLClient/Script/AnimationScript/EliteMonsterScript.h"

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
#include "DUOLGameEngine/Manager/TimeManager.h"

using namespace rttr;
RTTR_PLUGIN_REGISTRATION
{
rttr::registration::class_<DUOLClient::EliteMonsterScript>("EliteMonsterScript")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
			.property("_startHeight", &DUOLClient::EliteMonsterScript::_startHeight)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
		.property("_jumpAttackTime", &DUOLClient::EliteMonsterScript::_jumpAttackTime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_jumpWaitTime", &DUOLClient::EliteMonsterScript::_jumpWaitTime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_roarStartTime", &DUOLClient::EliteMonsterScript::_roarStartTime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_roarEndTime", &DUOLClient::EliteMonsterScript::_roarEndTime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_generationTime", &DUOLClient::EliteMonsterScript::_generationTime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	//		.property("_playerLocalPosition", &DUOLClient::EliteMonsterScript::_playerLocalPosition)
	//(
	//	metadata(DUOLCommon::MetaDataType::Inspectable, true)
	//	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	//	, metadata(DUOLCommon::MetaDataType::Serializable, true)
	//)
		.property("_playerAnimStart", &DUOLClient::EliteMonsterScript::_playerAnimStart)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
		.property("_playerAnimEnd", &DUOLClient::EliteMonsterScript::_playerAnimEnd)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_playerGenerateTime", &DUOLClient::EliteMonsterScript::_playerGenerateTime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
		.property("_startAnimation", &DUOLClient::EliteMonsterScript::_isAnimPlayed)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	);
}

DUOLClient::EliteMonsterScript::EliteMonsterScript(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name)
	, _startHeight(12.f)
	, _jumpAttackTime(2.f)
	, _jumpWaitTime(0.75f)
	, _roarStartTime(2.f)
	, _roarEndTime(2.f)
	, _generationTime(3.f)
	//, _playerLocalPosition()
	, _playerAnimStart(2.f)
	, _playerAnimEnd(2.f)
	, _playerGenerateTime(2.f)
	, _isAnimPlayed(false)
{

}

DUOLClient::EliteMonsterScript::~EliteMonsterScript()
{
}

void DUOLClient::EliteMonsterScript::OnAwake()
{
	auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();
	_cachedEliteMonsterDummy = scene->CreateFromFBXModel(TEXT("monster_elite"));
	_cachedEliteMonsterDummy->GetTransform()->SetParent(this->GetTransform(), false);
	auto eventHandle = _cachedEliteMonsterDummy->AddComponent<DummyMonoBehavior>();
	_cachedEliteMonsterAnimator = _cachedEliteMonsterDummy->GetComponent<DUOLGameEngine::Animator>();
	_audioSource = _cachedEliteMonsterDummy->AddComponent<DUOLGameEngine::AudioSource>();
	_cachedEliteMonsterAnimator->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(_T("Elite_Monster_Generate_Animator")));
	_cachedEliteMonsterDummy->SetIsActiveSelf(false);

	eventHandle->AddEventFunction(TEXT("JumpAttackStart"), [this]()
		{
			_isJumping = true;
		});

	eventHandle->AddEventFunction(TEXT("JumpAttackEnd"), [this]()
		{
			_cachedEliteMonsterAnimator->SetBool(TEXT("IsJumpAttack"), false);

	auto particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterBigCrack, 5.f);
	auto particleTranform = particleRenderer->GetTransform();
	particleTranform->SetPosition(GetTransform()->GetWorldPosition());
	_audioSource->SetAudioClip(_groundAttack);
	_audioSource->Play();
	return;
		});

	_audioSource->SetIsLoop(false);

	_groundAttack = DUOLGameEngine::SoundManager::GetInstance()->GetAudioClip(TEXT("MidBossCrack"));
	_roarClip = DUOLGameEngine::SoundManager::GetInstance()->GetAudioClip(TEXT("MiddleBossRoar"));

	///////////// Player
	_cachedPlayerDummy = scene->CreateFromFBXModel(TEXT("player"));
	_cachedPlayerDummy->GetTransform()->SetParent(this->GetTransform(), false);
	eventHandle = _cachedPlayerDummy->AddComponent<DummyMonoBehavior>();
	_cachedPlayerAnimator = _cachedPlayerDummy->GetComponent<DUOLGameEngine::Animator>();
	_cachedPlayerAnimator->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(_T("Player_CutScene_Animator")));

	_cachedPlayerDummy->SetIsActiveSelf(false);

	auto children = GetTransform()->GetChildGameObjects();
	_playerPos = children[0];
}

void DUOLClient::EliteMonsterScript::OnStart()
{
}

void DUOLClient::EliteMonsterScript::OnDestroy()
{
	}

void DUOLClient::EliteMonsterScript::OnUpdate(float deltaTime)
{
	if (_isAnimPlayed)
	{
		StopAllCoroutines();

		StartCoroutine(&DUOLClient::EliteMonsterScript::EliteBossGenerate);
		StartCoroutine(&DUOLClient::EliteMonsterScript::PlayerGenerate);
		_isAnimPlayed = false;
	}

}

void DUOLClient::EliteMonsterScript::EliteBossAnimateRun()
{
	StartCoroutine(&DUOLClient::EliteMonsterScript::EliteBossGenerate);
}

DUOLGameEngine::CoroutineHandler DUOLClient::EliteMonsterScript::EliteBossGenerate()
{
	auto transform = _cachedEliteMonsterDummy->GetTransform();

	_cachedEliteMonsterAnimator->SetBool(TEXT("IsRoar"), false);
	_cachedEliteMonsterAnimator->SetBool(TEXT("IsJumpAttack"), false);
	_isJumping = false;

	//로컬포지션을 위쪽으로..

	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(_jumpAttackTime);

	//중간프레임부터 떨어트린다.
	transform->SetLocalPosition({ 0.f, _startHeight, 0.f });
	_cachedEliteMonsterAnimator->SetBool(TEXT("IsJumpAttack"), true);
	_cachedEliteMonsterDummy->SetIsActiveSelf(true);

	//중간점프애니메이션 진행중.
	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(_jumpWaitTime);

	while (_cachedEliteMonsterAnimator->GetBool(TEXT("IsJumpAttack")))
	{
		if (_cachedEliteMonsterDummy->GetTransform()->GetLocalPosition().y > 0.f && _isJumping)
			_cachedEliteMonsterDummy->GetTransform()->Translate({ 0.f, -_startHeight * DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime(), 0.f });

		co_yield nullptr;
	}

	transform->SetLocalPosition({ 0.f, 0.f, 0.f });

	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(_roarStartTime);

	_cachedEliteMonsterAnimator->SetBool(TEXT("IsRoar"), true);
	auto particle = ParticleManager::GetInstance()->Pop(ParticleEnum::Shouting, 2.0f);
	auto particleTr = particle->GetTransform();
	particleTr->SetPosition(transform->GetWorldPosition());
	_audioSource->SetAudioClip(_roarClip);
	_audioSource->Play();

	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(_roarEndTime);

	particle->Stop();
	_audioSource->Stop();
	_cachedEliteMonsterAnimator->SetBool(TEXT("IsRoar"), false);

	co_yield  std::make_shared<DUOLGameEngine::WaitForSeconds>(_generationTime);

	_cachedEliteMonsterDummy->SetIsActiveSelf(false);

	co_return;
}

DUOLGameEngine::CoroutineHandler DUOLClient::EliteMonsterScript::PlayerGenerate()
{
	auto transform = _cachedPlayerDummy->GetTransform();
	auto currentPos = _playerPos->GetTransform()->GetWorldPosition();
	auto lookatPos = GetTransform()->GetWorldPosition();
	lookatPos.y = currentPos.y;
	transform->SetPosition(currentPos);
	transform->LookAt(lookatPos);

	_cachedPlayerAnimator->SetBool(TEXT("IsBackWalk"), false);
	_cachedPlayerAnimator->SetFloat(TEXT("AnimationSpeed"), 0.5f);
	_cachedPlayerDummy->SetIsActiveSelf(true);

	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(_playerAnimStart);

	_cachedPlayerAnimator->SetBool(TEXT("IsBackWalk"), true);

	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(_playerAnimEnd);

	_cachedPlayerAnimator->SetFloat(TEXT("AnimationSpeed"), 1.f);
	_cachedPlayerAnimator->SetBool(TEXT("IsBackWalk"), false);

	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(_playerGenerateTime);

	_cachedPlayerDummy->SetIsActiveSelf(false);
}

void DUOLClient::EliteMonsterScript::PlayerAnimateRun()
{
	StartCoroutine(&DUOLClient::EliteMonsterScript::PlayerGenerate);
}

void DUOLClient::EliteMonsterScript::PlayStart()
{
	_isAnimPlayed = true;
}
