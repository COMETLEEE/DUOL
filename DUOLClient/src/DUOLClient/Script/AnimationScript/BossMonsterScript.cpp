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
#include "DUOLGameEngine/Manager/CameraEventManager.h"


using namespace rttr;
RTTR_PLUGIN_REGISTRATION
{
rttr::registration::class_<DUOLClient::BossMonsterScript>("BossMonsterScript")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

DUOLClient::BossMonsterScript::BossMonsterScript(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name)
	, _bossTransform(nullptr)
	, _playerTransform(nullptr)
{
}

DUOLClient::BossMonsterScript::~BossMonsterScript()
{
}

void DUOLClient::BossMonsterScript::OnAwake()
{
	auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

	//child를 통해 position을 로드한다.
	for (auto transform : GetTransform()->GetChildren())
	{
		if (transform->GetGameObject()->GetName() == TEXT("BossPosition"))
		{
			_bossTransform = transform;
		}
		if (transform->GetGameObject()->GetName() == TEXT("PlayerPosition"))
		{
			_playerTransform = transform;
		}
	}

	if (_playerTransform == nullptr)
		_playerTransform = GetTransform();
	if (_bossTransform == nullptr)
		_bossTransform = GetTransform();

	//플레이어 더미 생성
	_cachedPlayerDummy = scene->CreateFromFBXModel(TEXT("player"));
	_cachedPlayerDummy->GetTransform()->SetParent(this->GetTransform(), false);
	auto playerEventHandle = _cachedPlayerDummy->AddComponent<DummyMonoBehavior>();
	_cachedPlayerAudioSource = _cachedPlayerDummy->AddComponent<DUOLGameEngine::AudioSource>();
	_cachedPlayerAudioSource->SetIsLoop(false);

	_cachedPlayerAnimator = _cachedPlayerDummy->GetComponent<DUOLGameEngine::Animator>();
	_cachedPlayerAnimator->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(_T("Player_CutScene_Animator")));


	_cachedBossDummy = scene->CreateFromFBXModel(TEXT("monster_Boss"));
	_cachedBossDummy->GetTransform()->SetParent(this->GetTransform(), false);
	auto bossEventHandle = _cachedBossDummy->AddComponent<DummyMonoBehavior>();
	_cachedBossAudioSource = _cachedBossDummy->AddComponent<DUOLGameEngine::AudioSource>();
	_cachedBossAudioSource->SetIsLoop(false);

	_cachedBossAnimator = _cachedBossDummy->GetComponent<DUOLGameEngine::Animator>();
	_cachedBossAnimator->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(_T("Boss_CutScene_Animator")));


	//꺼둔다
	_cachedPlayerDummy->SetIsActiveSelf(false);
	_cachedBossDummy->SetIsActiveSelf(false);
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

void DUOLClient::BossMonsterScript::BossPart01()
{
	//애니메이션을 일단 초기화하자.
	auto bossTransform = _cachedBossDummy->GetTransform();
	auto playerTransform = _cachedPlayerDummy->GetTransform();
	auto scriptPos = GetTransform();

	bossTransform->SetPosition(_bossTransform->GetWorldPosition());
	playerTransform->SetPosition(_playerTransform->GetWorldPosition());

	//초기상태 이니셜라이즈
	_cachedPlayerAnimator->SetBool(TEXT("IsDie"), true);
	_cachedPlayerAnimator->SetBool(TEXT("IsBackWalk"), false);

	_cachedBossAnimator->SetBool(TEXT("IsWalk"), false);
	_cachedBossAnimator->SetBool(TEXT("IsOverdrive"), false);

	//바로시작
	_cachedPlayerDummy->SetIsActiveSelf(true);
}

void DUOLClient::BossMonsterScript::BossPart02()
{
	//플레이어 idle로
	_cachedPlayerAnimator->SetBool(TEXT("IsDie"), false);
	//보스 on DUOLClient::BossMonsterScript::BossPart04C()
	 _cachedBossDummy->SetIsActiveSelf(true);
}

void DUOLClient::BossMonsterScript::BossPart04C()
{
	//플레이어 idle로
	_cachedBossAnimator->SetBool(TEXT("IsOverdrive"), true);
	//보스 on DUOLClient::BossMonsterScript::BossPart04C()
}

void DUOLClient::BossMonsterScript::EndScript()
{
	//초기상태 이니셜라이즈
	_cachedPlayerAnimator->SetBool(TEXT("Die"), true);
	_cachedPlayerAnimator->SetBool(TEXT("IsBackWalk"), false);
	_cachedBossAnimator->SetBool(TEXT("IsWalk"), false);
	_cachedBossAnimator->SetBool(TEXT("IsOverdrive"), false);

	_cachedBossDummy->SetIsActiveSelf(false);
	_cachedPlayerDummy->SetIsActiveSelf(false);
}

//DUOLGameEngine::CoroutineHandler DUOLClient::BossMonsterScript::AnimationStart()
//{
//	//애니메이션을 일단 초기화하자.
//	auto bossTransform = _cachedBossDummy->GetTransform();
//	auto playerTransform = _cachedPlayerDummy->GetTransform();
//	auto scriptPos = GetTransform();
//
//	bossTransform->SetPosition(_bossTransform->GetWorldPosition());
//	playerTransform->SetPosition(_playerTransform->GetWorldPosition());
//
//	//초기상태 이니셜라이즈
//	_cachedPlayerAnimator->SetBool(TEXT("Die"), true);
//	_cachedPlayerAnimator->SetBool(TEXT("IsBackWalk"), false);
//	_cachedBossAnimator->SetBool(TEXT("IsWalk"), false);
//
//	//바로시작
//	_cachedPlayerDummy->SetIsActiveSelf(true);
//
//	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(9.f);
//
//	//플레이어 idle로
//	_cachedPlayerAnimator->SetBool(TEXT("Die"), false);
//	//보스 on
//	_cachedBossDummy->SetIsActiveSelf(true);
//
//	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(30.f);
//
//	co_return;
//
//}

