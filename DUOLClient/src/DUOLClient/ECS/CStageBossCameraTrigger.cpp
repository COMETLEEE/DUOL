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
			// Main Camera Controller �� ���⿡ �޷��ֽ��ϴ�.
			_mainCameraController = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
		}
		if (gameObject->GetTag() == TEXT("BossScript"))
		{
			// Main Camera Controller �� ���⿡ �޷��ֽ��ϴ�.
			_bossScript = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::BossMonsterScript>();
		}
	}

	//������ �ε�
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
	//�÷��̾ ���д�
	player->SetIsActiveSelf(false);
	//ù��° ����
	//���� ����

	_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);

	//todo
	//bossPart01 ��ũ��Ʈ �� ���̾�α� �ۼ��Ұ�. ������ϴ��� Boss script�� �ִ���.. 
	cameraEventManager->SetPlayKey(_sequenceCamera[0]);
	_bossScript->BossPart01();
	systemManager->SetScript(L"DialogueText_22.png");

	//�������� ���
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode())
	{
		//���������� ���
		co_yield nullptr;
	}

	//todo
	//bossPart02 ��ũ��Ʈ �� ���̾�α� �ۼ��Ұ�. ������ϴ��� Boss script�� �ִ���..
	systemManager->SetScript(L"FinalBossCutscene01.png");
	cameraEventManager->SetPlayKey(_sequenceCamera[1]);
	_bossScript->BossPart02();

	//�������� ���
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode())
	{
		//���������� ���
		co_yield nullptr;
	}

	//todo
	//bossPart03 ��ũ��Ʈ �� ���̾�α� �ۼ��Ұ�. ������ϴ��� Boss script�� �ִ���..
	systemManager->SetScript(L"FinalBossCutscene02.png");
	cameraEventManager->SetPlayKey(_sequenceCamera[2]);

	//�������� ���
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode())
	{
		//���������� ���
		co_yield nullptr;
	}

	//todo
	//bossPart04_a ��ũ��Ʈ �� ���̾�α� �ۼ��Ұ�. ������ϴ��� Boss script�� �ִ���..
	systemManager->SetScript(L"FinalBossCutscene03.png");
	cameraEventManager->SetPlayKey(_sequenceCamera[3]);

	//�������� ���
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode())
	{
		//���������� ���
		co_yield nullptr;
	}

	//todo
//bossPart04_b ��ũ��Ʈ �� ���̾�α� �ۼ��Ұ�. ������ϴ��� Boss script�� �ִ���..
	cameraEventManager->SetPlayKey(_sequenceCamera[4]);

	//�������� ���
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode())
	{
		//���������� ���
		co_yield nullptr;
	}

	//todo
//bossPart04_c ��ũ��Ʈ �� ���̾�α� �ۼ��Ұ�. ������ϴ��� Boss script�� �ִ���..
	cameraEventManager->SetPlayKey(_sequenceCamera[5]);
	_bossScript->BossPart04C();

	//�������� ���
	co_yield nullptr;

	while (cameraEventManager->IsPlayMode() || _fadeInOut->GetFadeMode() != DUOLGameEngine::FadeInOutMode::DONE)
	{
		if (cameraEventManager->GetCurrentTime1() * 60.f > 45.f && !_isFadeOut)
		{
			//�����ϸ鼭 ���̵���.
			_fadeInOut->StartFadeOut(2, []() {});
			_isFadeOut = true;
		}

		if (_fadeInOut->GetFadeMode() == DUOLGameEngine::FadeInOutMode::DONE)
		{
			_fadeInOut->SetUIOption(true);
		}

		//���������� ���
		co_yield nullptr;
	}

	//���� ���� ��-

	//���������� ���
	co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(1.f);

	_bossScript->EndScript();
	//���̵��� ����
	player->SetIsActiveSelf(true);

	_fadeInOut->StartFadeIn(2, []() {});
	//���̵��ο�
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
