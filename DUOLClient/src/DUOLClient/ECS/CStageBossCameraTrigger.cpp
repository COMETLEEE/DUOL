#include "DUOLClient/ECS/Component/CStageBossCameraTrigger.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLClient/Script/AnimationScript/BossMonsterScript.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/FadeInOut.h"
#include "DUOLGameEngine/Manager/CameraEventManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::CStageBossCameraTrigger>("CStageBossCameraTrigger")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

DUOLClient::CStageBossCameraTrigger::CStageBossCameraTrigger(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name)

{
}

DUOLClient::CStageBossCameraTrigger::~CStageBossCameraTrigger()
{
}

void DUOLClient::CStageBossCameraTrigger::OnAwake()
{

	auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

	auto& allGameObjects = scene->GetAllGameObjects();


	for (auto gameObject : allGameObjects)
	{
		if (gameObject->GetName() == TEXT("BossMonsterSpawnPoint"))
		{
			_bossMonsterController = gameObject;
			_bossMonsterControllerComp = _bossMonsterController->GetComponent<EnemyGroupController>();
		}
		if (gameObject->GetName() == TEXT("TriggerBox_BossMonster"))
		{
			_bossTrigger = gameObject;
			_bossTrigger->SetIsActiveSelf(false);
		}

		if (gameObject->GetTag() == TEXT("Fade"))
		{
			_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
		}
		if (gameObject->GetTag() == TEXT("Camera"))
		{
			// Main Camera Controller 는 여기에 달려있습니다.
			_mainCameraController = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
		}
		if (gameObject->GetTag() == TEXT("BossScript"))
		{
			// Main Camera Controller 는 여기에 달려있습니다.
			_bossScript = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::BossMonsterScript>();
		}
	}

	//시퀀스 로드
	UINT64 key;
	key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("Final_Boss_Part1");
	_sequenceCamera.emplace_back(key);
	key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("Final_Boss_Part2");
	_sequenceCamera.emplace_back(key);
	key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("Final_Boss_Part3");
	_sequenceCamera.emplace_back(key);
	key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("Final_Boss_Part4_a");
	_sequenceCamera.emplace_back(key);
	key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("Final_Boss_Part4_b");
	_sequenceCamera.emplace_back(key);
	key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("Final_Boss_Part4_c");
	_sequenceCamera.emplace_back(key);
}

void DUOLClient::CStageBossCameraTrigger::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
{
	DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(trigger->_other);

	if (gameObject->GetTag() == TEXT("Player") && !_isPlay)
	{
		DUOL_INFO(DUOL_CONSOLE, "C Stage Boss Camera Action");

		DUOLClient::SystemManager::GetInstance()->SetUiObject(false);
		StartCoroutine(&DUOLClient::CStageBossCameraTrigger::ScripteRun, gameObject);
	}
}

DUOLGameEngine::CoroutineHandler DUOLClient::CStageBossCameraTrigger::ScripteRun(DUOLGameEngine::GameObject* player)
{
	auto cameraEventManager = DUOLGameEngine::CameraEventManager::GetInstance();
	auto systemManager = SystemManager::GetInstance();
	_isPlay = true;
	_isFadeOut = false;
	//플레이어를 꺼둔다
	player->SetIsActiveSelf(false);
	//첫번째 연출
	//보스 등장

	_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);

	//todo
	//bossPart01 스크립트 및 다이얼로그 작성할것. 여기다하던가 Boss script에 넣던가.. 
	cameraEventManager->SetPlayKey(_sequenceCamera[0]);
	_bossScript->BossPart01();
	systemManager->SetScript(L"DialogueText_22.png");

	//한프레임 대기
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode())
	{
		//끝날때까지 대기
		co_yield nullptr;
	}

	//todo
	//bossPart02 스크립트 및 다이얼로그 작성할것. 여기다하던가 Boss script에 넣던가..
	systemManager->SetScript(L"FinalBossCutscene01.png");
	cameraEventManager->SetPlayKey(_sequenceCamera[1]);
	_bossScript->BossPart02();

	//한프레임 대기
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode())
	{
		//끝날때까지 대기
		co_yield nullptr;
	}

	//todo
	//bossPart03 스크립트 및 다이얼로그 작성할것. 여기다하던가 Boss script에 넣던가..
	systemManager->SetScript(L"FinalBossCutscene02.png");
	cameraEventManager->SetPlayKey(_sequenceCamera[2]);

	//한프레임 대기
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode())
	{
		//끝날때까지 대기
		co_yield nullptr;
	}

	//todo
	//bossPart04_a 스크립트 및 다이얼로그 작성할것. 여기다하던가 Boss script에 넣던가..
	systemManager->SetScript(L"FinalBossCutscene03.png");
	cameraEventManager->SetPlayKey(_sequenceCamera[3]);

	//한프레임 대기
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode())
	{
		//끝날때까지 대기
		co_yield nullptr;
	}

	//todo
//bossPart04_b 스크립트 및 다이얼로그 작성할것. 여기다하던가 Boss script에 넣던가..
	cameraEventManager->SetPlayKey(_sequenceCamera[4]);

	//한프레임 대기
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode())
	{
		//끝날때까지 대기
		co_yield nullptr;
	}

	//todo
//bossPart04_c 스크립트 및 다이얼로그 작성할것. 여기다하던가 Boss script에 넣던가..
	cameraEventManager->SetPlayKey(_sequenceCamera[5]);
	_bossScript->BossPart04C();

	//한프레임 대기
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode() || _fadeInOut->GetFadeMode() != DUOLGameEngine::FadeInOutMode::DONE)
	{
		if (cameraEventManager->GetCurrentTime1() * 60.f > 45.f && !_isFadeOut)
		{
			//연출하면서 페이드인.
			_fadeInOut->StartFadeOut(2, []() {});
			_isFadeOut = true;
		}

		if (_fadeInOut->GetFadeMode() == DUOLGameEngine::FadeInOutMode::DONE)
		{
			_fadeInOut->SetUIOption(true);
		}

		//끝날때까지 대기
		co_yield nullptr;
	}

	//보스 연출 완-

	//잠깐암전상태 대기
	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(1.f);

	_bossScript->EndScript();
	//페이드인 시작
	player->SetIsActiveSelf(true);

	_fadeInOut->StartFadeIn(2, []() {});
	//페이드인완
	_mainCameraController->SetCameraState(DUOLClient::MainCameraState::FOLLOW_PLAYER);
	player->SetIsActiveSelf(true);
	DUOLClient::SystemManager::GetInstance()->SetUiObject(true);
	DUOLClient::SystemManager::GetInstance()->SetBossUI(L"05_ingame_thejudicis", 1200.f);

	while (_fadeInOut->GetFadeMode() != DUOLGameEngine::FadeInOutMode::DONE)
	{
		co_yield nullptr;
	}


	_bossTrigger->SetIsActiveSelf(true);
	GetGameObject()->GetComponent<DUOLGameEngine::BoxCollider>()->SetIsEnabled(false);
}
