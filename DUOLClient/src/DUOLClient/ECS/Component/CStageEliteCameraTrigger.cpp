#include "DUOLClient/ECS/Component/CStageEliteCameraTrigger.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>

#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLGameEngine/ECS/Component/FadeInOut.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/CameraEventManager.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"
#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLClient/Script/AnimationScript/EliteMonsterScript.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::CStageEliteCameraTrigger>("CStageEliteCameraTrigger")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

DUOLClient::CStageEliteCameraTrigger::CStageEliteCameraTrigger(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name)
	, _isPlay(false)
	, _isFadeOut(false)
{
}

DUOLClient::CStageEliteCameraTrigger::~CStageEliteCameraTrigger()
{
}

void DUOLClient::CStageEliteCameraTrigger::OnAwake()
{
	auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

	for (auto gameObject : allGameObjects)
	{
		if (gameObject->GetName() == TEXT("EliteBossMonsterSpawnPoint"))
		{
			_eliteBossMonsterController = gameObject;
			_eliteBossMonsterControllerComp = _eliteBossMonsterController->GetComponent<EnemyGroupController>();
		}
		if (gameObject->GetName() == TEXT("TriggerBox_EliteBossMonster"))
		{
			_eliteBossTrigger = gameObject;
			_eliteBossTrigger->SetIsActiveSelf(false);
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
		if (gameObject->GetTag() == TEXT("EliteScript"))
		{
			// Main Camera Controller 는 여기에 달려있습니다.
			_eliteScript = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::EliteMonsterScript>();
		}
	}

	UINT64 key;
	key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("Elite_Boss_Part1");
	_sequenceCamera.emplace_back(key);
	key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("Elite_Boss_Part2");
	_sequenceCamera.emplace_back(key);
	key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("Elite_Boss_Part3");
	_sequenceCamera.emplace_back(key);
}

void DUOLClient::CStageEliteCameraTrigger::OnUpdate(float deltaTime)
{
	/*if (_eliteBossMonsterControllerComp->GetIsClearGroup())
	{
		/_bossZoneTrigger->SetIsActiveSelf(true);
		Destroy(GetGameObject());
	}*/
}

void DUOLClient::CStageEliteCameraTrigger::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
{
	DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(trigger->_other);

	if (_fadeInOut == nullptr)
	{
		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
			}
		}
	}

	if (gameObject->GetTag() == TEXT("Player") && !_isPlay)
	{
		DUOL_INFO(DUOL_CONSOLE, "C Stage Elite Action");

		DUOLClient::SystemManager::GetInstance()->SetUiObject(true);
		StartCoroutine(&DUOLClient::CStageEliteCameraTrigger::ScripteRun, gameObject);
	}
}

DUOLGameEngine::CoroutineHandler DUOLClient::CStageEliteCameraTrigger::ScripteRun(DUOLGameEngine::GameObject* player)
{
	//트리거박스후 연출 시작. 카메라 연출 On
	//스크립트하기전 사용하는 변수를 초기화하자!
	auto cameraEventManager = DUOLGameEngine::CameraEventManager::GetInstance();

	_isPlay = true;
	_isFadeOut = false;
	//플레이어를 꺼둔다
	player->SetIsActiveSelf(false);
	//첫번째 연출
	//보스 등장

	_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
	cameraEventManager->SetPlayKey(_sequenceCamera[0]);

	//업데이트가 겹치지 않도록 차등을 둔다.
	co_yield nullptr;

	_eliteScript->EliteBossAnimateRun();

	while(cameraEventManager->IsPlayMode())
	{
		//끝날때까지 대기
		co_yield nullptr;
	}

	//두번째연출
	//플레이어 뒷걸음질

	cameraEventManager->SetPlayKey(_sequenceCamera[1]);
	_eliteScript->PlayerAnimateRun();

	//업데이트가 겹치지 않도록 차등을 둔다.
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode())
	{
		//끝날때까지 대기
		co_yield nullptr;
	}

	//세번째연출
	//Roar
	cameraEventManager->SetPlayKey(_sequenceCamera[2]);

	//업데이트가 겹치지 않도록 차등을 둔다.
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode() || _fadeInOut->GetFadeMode() != DUOLGameEngine::FadeInOutMode::DONE)
	{
		if(cameraEventManager->GetCurrentTime1() * 60.f > 45.f && !_isFadeOut)
		{
			//연출하면서 페이드인.
			_fadeInOut->StartFadeOut(2, []() {});
			_isFadeOut = true;
		}

		if(_fadeInOut->GetFadeMode() == DUOLGameEngine::FadeInOutMode::DONE)
		{
			_fadeInOut->SetUIOption(true);
		}

		//끝날때까지 대기
		co_yield nullptr;
	}


	//잠깐암전상태 대기
	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(1.f);

	//페이드아웃 시작
	_mainCameraController->SetCameraState(DUOLClient::MainCameraState::FOLLOW_PLAYER);
	player->SetIsActiveSelf(true);

	_fadeInOut->StartFadeIn(2, []() {});

	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(2);
	                                                                         
	DUOLClient::SystemManager::GetInstance()->SetUiObject(false);
	_isFadeOut = false;

	_eliteBossTrigger->SetIsActiveSelf(true);
	GetGameObject()->GetComponent<DUOLGameEngine::BoxCollider>()->SetIsEnabled(false);
	////UIChange
	//_fadeInOut->StartFadeIn(2, []() {});

	//co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(_playerAnimEnd);


	//if (_isFadeOut && _isFadeIn && _fadeInOut->GetFadeMode() == DUOLGameEngine::FadeInOutMode::DONE && _isLastScript)
	//{
	//	_interactiveScript->SetPlayInfoKey(L"DialogueText_17.png");
	//}

	//

	//co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(_playerAnimEnd);
	////FADEOUT이 끝난후 
}
