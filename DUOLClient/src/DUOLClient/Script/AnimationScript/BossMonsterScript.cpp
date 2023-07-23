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

	//child�� ���� position�� �ε��Ѵ�.
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

	//�÷��̾� ���� ����
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


	//���д�
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
	//�ִϸ��̼��� �ϴ� �ʱ�ȭ����.
	auto bossTransform = _cachedBossDummy->GetTransform();
	auto playerTransform = _cachedPlayerDummy->GetTransform();
	auto scriptPos = GetTransform();

	bossTransform->SetPosition(_bossTransform->GetWorldPosition());
	playerTransform->SetPosition(_playerTransform->GetWorldPosition());

	//�ʱ���� �̴ϼȶ�����
	_cachedPlayerAnimator->SetBool(TEXT("IsDie"), true);
	_cachedPlayerAnimator->SetBool(TEXT("IsBackWalk"), false);

	_cachedBossAnimator->SetBool(TEXT("IsWalk"), false);
	_cachedBossAnimator->SetBool(TEXT("IsOverdrive"), false);

	//�ٷν���
	_cachedPlayerDummy->SetIsActiveSelf(true);
}

void DUOLClient::BossMonsterScript::BossPart02()
{
	//�÷��̾� idle��
	_cachedPlayerAnimator->SetBool(TEXT("IsDie"), false);
	//���� on DUOLClient::BossMonsterScript::BossPart04C()
	 _cachedBossDummy->SetIsActiveSelf(true);
}

void DUOLClient::BossMonsterScript::BossPart04C()
{
	//�÷��̾� idle��
	_cachedBossAnimator->SetBool(TEXT("IsOverdrive"), true);
	//���� on DUOLClient::BossMonsterScript::BossPart04C()
}

void DUOLClient::BossMonsterScript::EndScript()
{
	//�ʱ���� �̴ϼȶ�����
	_cachedPlayerAnimator->SetBool(TEXT("Die"), true);
	_cachedPlayerAnimator->SetBool(TEXT("IsBackWalk"), false);
	_cachedBossAnimator->SetBool(TEXT("IsWalk"), false);
	_cachedBossAnimator->SetBool(TEXT("IsOverdrive"), false);

	_cachedBossDummy->SetIsActiveSelf(false);
	_cachedPlayerDummy->SetIsActiveSelf(false);
}

//DUOLGameEngine::CoroutineHandler DUOLClient::BossMonsterScript::AnimationStart()
//{
//	//�ִϸ��̼��� �ϴ� �ʱ�ȭ����.
//	auto bossTransform = _cachedBossDummy->GetTransform();
//	auto playerTransform = _cachedPlayerDummy->GetTransform();
//	auto scriptPos = GetTransform();
//
//	bossTransform->SetPosition(_bossTransform->GetWorldPosition());
//	playerTransform->SetPosition(_playerTransform->GetWorldPosition());
//
//	//�ʱ���� �̴ϼȶ�����
//	_cachedPlayerAnimator->SetBool(TEXT("Die"), true);
//	_cachedPlayerAnimator->SetBool(TEXT("IsBackWalk"), false);
//	_cachedBossAnimator->SetBool(TEXT("IsWalk"), false);
//
//	//�ٷν���
//	_cachedPlayerDummy->SetIsActiveSelf(true);
//
//	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(9.f);
//
//	//�÷��̾� idle��
//	_cachedPlayerAnimator->SetBool(TEXT("Die"), false);
//	//���� on
//	_cachedBossDummy->SetIsActiveSelf(true);
//
//	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(30.f);
//
//	co_return;
//
//}

