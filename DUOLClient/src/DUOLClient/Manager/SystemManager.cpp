#include "DUOLClient/Manager/SystemManager.h"

#include <rttr/registration>

#include "DUOLClient/Manager/EnemyManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/CameraEventManager.h"
#include "DUOLClient/Manager/UIDataManager.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLClient/ECS/Component/Map/Portal.h"
#include "DUOLClient/Manager/GameManager.h"
#include "DUOLGameEngine/Manager/SoundManager.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/FadeInOut.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLClient/Player/Player.h"
#include "DUOLGameEngine/ECS/Component/AudioListener.h"
#include "DUOLGameEngine/ECS/Component/AudioSource.h"
#include "DUOLClient/Script/InteractiveScript.h"

namespace  DUOLClient
{

	DUOLClient::SystemManager* DUOLClient::SystemManager::_systemInstance = nullptr;

	bool DUOLClient::SystemManager::_isFirstMonster = false;

	bool DUOLClient::SystemManager::_isMiddleEvent = false;

	bool DUOLClient::SystemManager::_isAStageClear = false;

	bool DUOLClient::SystemManager::_isBStageClear = false;

	bool DUOLClient::SystemManager::_isCStageClear = false;

	using namespace rttr;

	RTTR_REGISTRATION
	{
		rttr::registration::class_<DUOLClient::SystemManager>("SystemManager")
		.constructor()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		.method("Reset", &SystemManager::Reset);
		;
	}

		SystemManager::SystemManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
		, _isBStageAllMonsterKill(false)
		, _isDoorMonsterKill(false)
		, _isOpenDoor(false)
		, _currentTime(0.0f)
		, _rimLightObject(nullptr)
		, _doorObject(nullptr)
		, _moveDoor(0.0f)
		, _rimPower(0.f)
		, _mainCameraController(nullptr)
		, _isCameraSequenceMode(false)
		, _currentGameScene(GameScene::Main)
		, _isStartScript(false)
		, _scriptIndex(0)
		, _isEnemyAIPlay(true)
		, _isShowScript(false)
		, _isShowInfo(false)
		, _cameraInstance(nullptr)
		, _isFirstMonsterAction(false)
		, _isBStageDoorAction(false)
		, _isBStageFactoryAction(false)
		, _isBStageMonsterWaveAction(false)
		, _isTotalSceneEnd(false)
		, _preScriptIndex(0)
		, _isPlayerUltra(true)
		, _isPlayerOverdrive(true)
		, _infoObject(nullptr)
		, _isBWaveScriptCheck(false)
		, _isCreatePortal(false)
		, _isStartCameraAction(false)
		, _isFirstMonsterCheck(false)
		, _cCurrentTime(0.f)
		, _isInfoChcek(false)
		, _isInfo(false)
		, _isEliteMonsterActionCheck(false)
		, _cFadeOutTime(0.f)
		, _isFadeOut(false)
		, _isFadeIn(false)
		, _isLastScript(false)
	{
	}

	DUOLClient::SystemManager::~SystemManager()
	{
		_systemInstance = nullptr;

		delete _interactiveScript;
	}

	SystemManager* DUOLClient::SystemManager::GetInstance()
	{
		if (_systemInstance == nullptr)
		{
			auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

			for (auto gameObject : allGameObjects)
			{
				if (gameObject->GetName() == TEXT("SystemManager"))
				{
					_systemInstance = gameObject->GetComponent<SystemManager>();
					return _systemInstance;
				}
			}

			if (!_systemInstance)
			{
				_systemInstance = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty()->AddComponent<SystemManager>();
			}
		}

		return _systemInstance;
	}

	void SystemManager::SetAstageClear(bool value)
	{
		_isAStageClear = value;
	}

	void SystemManager::InitializeMiddle()
	{
		_currentGameScene = GameScene::Middle;
		_middleSceneClips.clear();

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{

			if (gameObject->GetTag() == TEXT("Camera"))
			{
				// Main Camera Controller 는 여기에 달려있습니다.
				_mainCameraController = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
			}
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
			}
			if (gameObject->GetName() == TEXT("Player"))
			{
				_player = gameObject->GetComponent<DUOLClient::Player>();
			}
			if ((gameObject->GetName() == TEXT("Combo")) || (gameObject->GetName() == TEXT("ComboNum")))
			{
				gameObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("UIAudioSource"))
			{
				_uiAudioSource = gameObject->AddComponent<DUOLGameEngine::AudioSource>();;
			}

		}

		// Dialogue
		_middleSceneClips.insert(std::make_pair(L"DialogueText_04.png", _soundManager->GetAudioClip(TEXT("NPC_23"))));
		_middleSceneClips.insert(std::make_pair(L"DialogueText_05.png", _soundManager->GetAudioClip(TEXT("NPC_24"))));
		_middleSceneClips.insert(std::make_pair(L"DialogueText_06.png", _soundManager->GetAudioClip(TEXT("NPC_25"))));
		_middleSceneClips.insert(std::make_pair(L"DialogueText_11.png", _soundManager->GetAudioClip(TEXT("NPC_08"))));
		_middleSceneClips.insert(std::make_pair(L"DialogueText_12.png", _soundManager->GetAudioClip(TEXT("NPC_09"))));
		_middleSceneClips.insert(std::make_pair(L"DialogueText_07.png", _soundManager->GetAudioClip(TEXT("NPC_26"))));


		if (_cameraInstance == nullptr)
		{
			_cameraInstance = DUOLGameEngine::CameraEventManager::GetInstance();
		}

		if (_uiAudioSource == nullptr)
		{
			auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
			object->GetTransform()->SetParent(this->GetTransform());
			object->SetName(TEXT("UIAudioSource"));
			auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
			_uiAudioSource = comp;
		}

		_isStartScript = true;

		if (!_isMiddleEvent)
		{
			_isPlayerOverdrive = false;
			_isPlayerUltra = false;
		}
		else
		{
			_isPlayerOverdrive = true;
			_isPlayerUltra = true;
		}

	}

	void SystemManager::InitializeStageTotal()
	{
		_currentGameScene = GameScene::Total;

		_totalSceneClips.clear();

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Camera"))
			{
				// Main Camera Controller 는 여기에 달려있습니다.
				_mainCameraController = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
				_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
			}
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
				_fadeInOut->StartFadeIn(SCENE_START_FADE_IN, nullptr);
			}
		}

		if (_cameraInstance == nullptr)
		{
			_cameraInstance = DUOLGameEngine::CameraEventManager::GetInstance();
		}

		std::vector<UINT64> _sequenceCamera;
		UINT64 key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("Camera_Area_A");
		_sequenceCamera.emplace_back(key);
		key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("Camera_Area_C");
		_sequenceCamera.emplace_back(key);
		key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("Camera_Area_B");
		_sequenceCamera.emplace_back(key);

		// Camera Action Start
		DUOLGameEngine::CameraEventManager::GetInstance()->SetSequenceList(_sequenceCamera);
		DUOLGameEngine::CameraEventManager::GetInstance()->SetSequenceMode(true);

		_audioSource = this->GetGameObject()->GetComponent<DUOLGameEngine::AudioSource>();

		_audioListener = this->GetGameObject()->GetComponent<DUOLGameEngine::AudioListener>();

		_totalSceneClips.insert(std::make_pair(L"DialogueText_01.png", _soundManager->GetAudioClip(TEXT("NPC_20"))));
		_totalSceneClips.insert(std::make_pair(L"DialogueText_02.png", _soundManager->GetAudioClip(TEXT("NPC_21"))));
		_totalSceneClips.insert(std::make_pair(L"DialogueText_03.png", _soundManager->GetAudioClip(TEXT("NPC_22"))));

		// DONT REMOVE!!!
		_preScriptIndex = -1;
	}

	void SystemManager::InitializeStageA()
	{
		_currentGameScene = GameScene::StageA;

		_isMiddleEvent = true;

		_isFirstMonsterAction = false;

		_isStartCameraAction = false;

		_isFirstMonsterCheck = false;

		_isInfo = false;
		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("MainCamera"))
			{
				// Main Camera Controller �� ���⿡ �޷��ֽ��ϴ�.
				_mainCameraController = gameObject->GetTransform()->GetParent()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();

			}
			if (gameObject->GetTag() == TEXT("Camera"))
			{
				// Main Camera Controller �� ���⿡ �޷��ֽ��ϴ�.
				_mainCameraController = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();

			}
			if (gameObject->GetName() == TEXT("Player"))
			{
				_player = gameObject->GetComponent<DUOLClient::Player>();
			}
			if (gameObject->GetName() == TEXT("Combo") || gameObject->GetName() == TEXT("ComboNum"))
			{
				gameObject->SetIsActiveSelf(false);
				gameObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("UIAudioSource"))
			{
				_uiAudioSource = gameObject->AddComponent<DUOLGameEngine::AudioSource>();;
			}
			if (gameObject->GetName() == TEXT("Minimap"))
			{
				_miniMapImage = gameObject->AddComponent<DUOLGameEngine::Image>();;
			}

		}


		if (_cameraInstance == nullptr)
		{
			_cameraInstance = DUOLGameEngine::CameraEventManager::GetInstance();
		}

		// first action
		if (!_isFirstMonster)
		{
			_aSceneClips.insert(std::make_pair(L"DialogueText_08.png", _soundManager->GetAudioClip(TEXT("NPC_05"))));
			_aSceneClips.insert(std::make_pair(L"DialogueText_09.png", _soundManager->GetAudioClip(TEXT("NPC_06"))));
			_aSceneClips.insert(std::make_pair(L"DialogueText_10.png", _soundManager->GetAudioClip(TEXT("NPC_07"))));

		}

		// StageB
		_aSceneClips.insert(std::make_pair(L"DialogueText_13.png", _soundManager->GetAudioClip(TEXT("NPC_10"))));
		_aSceneClips.insert(std::make_pair(L"DialogueText_14.png", _soundManager->GetAudioClip(TEXT("NPC_01"))));

		_aSceneClips.insert(std::make_pair(L"DialogueText_19.png", _soundManager->GetAudioClip(TEXT("NPC_01"))));

		if (_uiAudioSource == nullptr)
		{
			auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
			object->GetTransform()->SetParent(this->GetTransform());
			object->SetName(TEXT("UIAudioSource"));
			auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
			_uiAudioSource = comp;
		}
	}

	void SystemManager::InitializeStageB()
	{
		_currentGameScene = GameScene::StageB;

		_isMiddleEvent = true;

		_isFirstMonsterAction = false;
		_isBStageDoorAction = false;
		_isBStageFactoryAction = false;
		_isBStageMonsterWaveAction = false;
		_isBWaveScriptCheck = false;
		_isCreatePortal = false;
		_isFirstMonsterCheck = false;

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("RimObject"))
			{
				_rimLightObject = gameObject;
			}
			if (gameObject->GetTag() == TEXT("OpenDoor"))
			{
				_doorObject = gameObject;
			}
			if (gameObject->GetTag() == TEXT("MainCamera"))
			{
				// Main Camera Controller 는 여기에 달려있습니다.
				_mainCameraController = gameObject->GetTransform()->GetParent()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
			}
			if (gameObject->GetTag() == TEXT("Camera"))
			{
				// Main Camera Controller 는 여기에 달려있습니다.
				_mainCameraController = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();

			}
			if (gameObject->GetName() == TEXT("Player"))
			{
				_player = gameObject->GetComponent<DUOLClient::Player>();
			}
			if (gameObject->GetName() == TEXT("TutorialWin"))
			{
				_infoObject = gameObject;
			}
			if (gameObject->GetName() == TEXT("B_Danger"))
			{
				_dangerObject = gameObject;
				_dangerObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("B_Danger_Collider"))
			{
				_dangerColliderObject = gameObject;
				_dangerColliderObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("Combo") || gameObject->GetName() == TEXT("ComboNum"))
			{
				gameObject->SetIsActiveSelf(false);
				gameObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("UIAudioSource"))
			{
				_uiAudioSource = gameObject->AddComponent<DUOLGameEngine::AudioSource>();;
			}
			if (gameObject->GetName() == TEXT("Minimap"))
			{
				_miniMapImage = gameObject->AddComponent<DUOLGameEngine::Image>();;
			}
		}
		// first action
		if (!_isFirstMonster)
		{
			_bSceneClips.insert(std::make_pair(L"DialogueText_08.png", _soundManager->GetAudioClip(TEXT("NPC_05"))));
			_bSceneClips.insert(std::make_pair(L"DialogueText_09.png", _soundManager->GetAudioClip(TEXT("NPC_06"))));
			_bSceneClips.insert(std::make_pair(L"DialogueText_10.png", _soundManager->GetAudioClip(TEXT("NPC_07"))));

		}

		// StageB
		_bSceneClips.insert(std::make_pair(L"DialogueText_14.png", _soundManager->GetAudioClip(TEXT("NPC_11"))));
		_bSceneClips.insert(std::make_pair(L"DialogueText_15.png", _soundManager->GetAudioClip(TEXT("NPC_12"))));
		_bSceneClips.insert(std::make_pair(L"DialogueText_15A.png", _soundManager->GetAudioClip(TEXT("NPC_12A"))));
		_bSceneClips.insert(std::make_pair(L"DialogueText_16.png", _soundManager->GetAudioClip(TEXT("NPC_13"))));


		_bSceneClips.insert(std::make_pair(L"DialogueText_19.png", _soundManager->GetAudioClip(TEXT("NPC_01"))));

		_uiClips.insert(std::make_pair(L"door_sound_effect", _soundManager->GetAudioClip(TEXT("door_sound_effect"))));

		if (_uiAudioSource == nullptr)
		{
			auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
			object->GetTransform()->SetParent(this->GetTransform());
			object->SetName(TEXT("UIAudioSource"));
			auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
			_uiAudioSource = comp;
		}
	}

	void SystemManager::InitializeStageC()
	{
		_currentGameScene = GameScene::StageC;

		_cCurrentTime = 0.f;

		_isMiddleEvent = true;

		_isInfoChcek = false;

		_isEliteMonsterActionCheck = false;

		_cFadeOutTime = 0.f;

		_isFadeOut = false;

		_isFadeIn = false;

		_isLastScript = false;

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("MainCamera"))
			{
				// Main Camera Controller 는 여기에 달려있습니다.
				_mainCameraController = gameObject->GetTransform()->GetParent()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();

			}
			if (gameObject->GetTag() == TEXT("Camera"))
			{
				// Main Camera Controller 는 여기에 달려있습니다.
				_mainCameraController = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();

			}
			if (gameObject->GetName() == TEXT("Player"))
			{
				_player = gameObject->GetComponent<DUOLClient::Player>();
			}
			if (gameObject->GetName() == TEXT("Combo") || gameObject->GetName() == TEXT("ComboNum"))
			{
				gameObject->SetIsActiveSelf(false);
				gameObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("UIAudioSource"))
			{
				_uiAudioSource = gameObject->AddComponent<DUOLGameEngine::AudioSource>();;
			}
			if (gameObject->GetName() == TEXT("Minimap"))
			{
				_miniMapImage = gameObject->AddComponent<DUOLGameEngine::Image>();;
			}
			if (gameObject->GetName() == TEXT("EilteMonsterPoint"))
			{
				_uiMiniMapObject = gameObject;;
				gameObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
				_fadeInOut->StartFadeIn(SCENE_START_FADE_IN, nullptr);
			}
			if (gameObject->GetName() == TEXT("Canvas"))
			{
				_uiObject = gameObject;
			}
		}

		if (_cameraInstance == nullptr)
		{
			_cameraInstance = DUOLGameEngine::CameraEventManager::GetInstance();
		}

		_cSceneClips.insert(std::make_pair(L"DialogueText_17.png", _soundManager->GetAudioClip(TEXT("NPC_14"))));
		_cSceneClips.insert(std::make_pair(L"DialogueText_18.png", _soundManager->GetAudioClip(TEXT("NPC_15"))));

		_cSceneClips.insert(std::make_pair(L"DialogueText_19.png", _soundManager->GetAudioClip(TEXT("NPC_01"))));

		if (_uiAudioSource == nullptr)
		{
			auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
			object->GetTransform()->SetParent(this->GetTransform());
			object->SetName(TEXT("UIAudioSource"));
			auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
			_uiAudioSource = comp;
		}

	}

	void SystemManager::InitializeStageBoss()
	{
		_currentGameScene = GameScene::StageBoss;

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("MainCamera"))
			{
				// Main Camera Controller �� ���⿡ �޷��ֽ��ϴ�.
				_mainCameraController = gameObject->GetTransform()->GetParent()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();

			}
			if (gameObject->GetTag() == TEXT("Camera"))
			{
				// Main Camera Controller �� ���⿡ �޷��ֽ��ϴ�.
				_mainCameraController = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();

			}
			if (gameObject->GetName() == TEXT("Player"))
			{
				_player = gameObject->GetComponent<DUOLClient::Player>();
			}
			if (gameObject->GetName() == TEXT("UIAudioSource"))
			{
				_uiAudioSource = gameObject->AddComponent<DUOLGameEngine::AudioSource>();;
			}
			if (gameObject->GetName() == TEXT("Minimap"))
			{
				_miniMapImage = gameObject->AddComponent<DUOLGameEngine::Image>();;
			}
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
				_fadeInOut->StartFadeIn(SCENE_START_FADE_IN, nullptr);
			}
			if (gameObject->GetName() == TEXT("Canvas"))
			{
				_uiObject = gameObject;
			}
		}

		if (_cameraInstance == nullptr)
		{
			_cameraInstance = DUOLGameEngine::CameraEventManager::GetInstance();
		}

		if (_uiAudioSource == nullptr)
		{
			auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
			object->GetTransform()->SetParent(this->GetTransform());
			object->SetName(TEXT("UIAudioSource"));
			auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
			_uiAudioSource = comp;
		}

	}

	void SystemManager::InitializeStage()
	{
		_currentGameScene = GameScene::ETC;

		_isBStageDoorAction = false;

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("MainCamera"))
			{
				_mainCameraController = gameObject->GetTransform()->GetParent()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();

			}
			if (gameObject->GetTag() == TEXT("Camera"))
			{
				_mainCameraController = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();

			}
			if (gameObject->GetName() == TEXT("Player"))
			{
				_player = gameObject->GetComponent<DUOLClient::Player>();
			}
			if (gameObject->GetName() == TEXT("UIAudioSource"))
			{
				_uiAudioSource = gameObject->AddComponent<DUOLGameEngine::AudioSource>();;
			}
		}

		if (_uiAudioSource == nullptr)
		{
			auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
			object->GetTransform()->SetParent(this->GetTransform());
			object->SetName(TEXT("UIAudioSource"));
			auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
			_uiAudioSource = comp;
		}
	}

	void SystemManager::MiddleUpdate(float deltaTime)
	{
		if (_isMiddleEvent)
			return;

		if (_fadeInOut->GetFadeMode() == DUOLGameEngine::FadeInOutMode::DONE && _isStartScript)
		{
			if (_interactiveScript != nullptr)
			{
				_interactiveScript->ShowMiddleSceneScript();
				_interactiveScript->ScriptPlay(deltaTime);
				_interactiveScript->InfoPlay(deltaTime);

				if (_interactiveScript->GetNowInfoKey() == L"Info05.png" && !_isPlayerOverdrive)
				{
					_isPlayerOverdrive = true;
					_player->AddOverdrivePoint(100.f);
				}
				if (_player->IsOverdriveMode())
					_interactiveScript->SetPlayInfoKey(L"Info06.png");
			}
		}

		if (!_cameraInstance->IsPlayMode())
		{
			_mainCameraController->SetCameraState(DUOLClient::MainCameraState::FOLLOW_PLAYER);
		}
	}

	void SystemManager::StageAUpdate(float deltaTime)
	{

		// 처음 몬스터 만나고 처음 연출이 들어가할때
		// false true
		if (!_isFirstMonster && _isFirstMonsterAction)
		{
			_isFirstMonsterAction = false;
			_isFirstMonsterCheck = true;
			UINT64 key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("MonsterFirstSpawn_AreaA");
			DUOLGameEngine::CameraEventManager::GetInstance()->SetPlayKey(key);
			_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
			_interactiveScript->SetPlayScriptKey(L"DialogueText_08.png");
			_isCameraSequenceMode = true;

			_isFirstMonster = true;
		}
		// true true
		// No Scipt
		else if (_isFirstMonster && _isFirstMonsterAction)
		{
			_isFirstMonsterAction = false;
			_isFirstMonsterCheck = true;
			UINT64 key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("MonsterFirstSpawn_AreaA");
			DUOLGameEngine::CameraEventManager::GetInstance()->SetPlayKey(key);
			_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
			
			_isCameraSequenceMode = true;
		}

		if (_isStartCameraAction && !_cameraInstance->IsPlayMode() && _isInfo)
		{
			_interactiveScript->SetPlayInfoKey(L"Info08.png");
			_isInfo = false;
		}

		if (_interactiveScript)
		{
			_interactiveScript->ShowASceneScript();
			_interactiveScript->ScriptPlay(deltaTime);
			_interactiveScript->InfoPlay(deltaTime);
		}

		if (_isCameraSequenceMode && !_cameraInstance->IsPlayMode())
		{
			_isCameraSequenceMode = false;
			_mainCameraController->SetCameraState(DUOLClient::MainCameraState::FOLLOW_PLAYER);
		}

	}

	void SystemManager::StageBUpdate(float deltaTime)
	{

#pragma region FirstMonster Action
		// 몬스터랑 처음으로 만남
		if (!_isFirstMonster && _isFirstMonsterAction)
		{
			UINT64 key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("MonsterFirstSpawn_AreaB");
			DUOLGameEngine::CameraEventManager::GetInstance()->SetPlayKey(key);
			_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
			_interactiveScript->SetPlayScriptKey(L"DialogueText_08.png");
			_isCameraSequenceMode = true;

			_isFirstMonster = true;
			_isFirstMonsterAction = false;
			_isFirstMonsterCheck = true;

		}
		else if (_isFirstMonster && _isFirstMonsterAction)
		{
			UINT64 key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("MonsterFirstSpawn_AreaB");
			DUOLGameEngine::CameraEventManager::GetInstance()->SetPlayKey(key);
			_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
			_isCameraSequenceMode = true;
			_isFirstMonsterAction = false;
			_isFirstMonsterCheck = true;
		}

		if (_interactiveScript)
		{
			_interactiveScript->ShowBSceneScript();
			_interactiveScript->ScriptPlay(deltaTime);
			_interactiveScript->InfoPlay(deltaTime);

		}

		if (!_isCameraSequenceMode && _isBStageMonsterWaveAction && !_isBWaveScriptCheck)
		{
			_interactiveScript->SetPlayScriptKey(L"DialogueText_15A.png");
			_isBWaveScriptCheck = true;
		}
		if (_isBStageMonsterWaveAction && _isBWaveScriptCheck)
		{
			auto control1 = EnemyManager::GetInstance()->GetEnemyGroupController(L"Wave_MonsterSpawnPoint_B_04_Wave_WithCameraAction_WeakElite02(lastwavespawn)_Gun05*3_Normal30*3");

			if (control1)
			{
				if (control1->GetIsClearGroup()  && !_isCreatePortal)
				{
					_isCreatePortal = true;
					_isBStageClear = true;
					_interactiveScript->SetPlayScriptKey(L"DialogueText_16.png");
					// Create Portal
					this->CreatePortal(TEXT("Portal_B"), TEXT("Middle"), B_CLEAR_PORTAL_TO_MIDDLE_POSITION);
					MiniMapChange(L"StageBMiniMapPortal");
				}
			}
		}
#pragma endregion

		if (_isCameraSequenceMode && !_cameraInstance->IsPlayMode())
		{
			_isCameraSequenceMode = false;
			_mainCameraController->SetCameraState(DUOLClient::MainCameraState::FOLLOW_PLAYER);
		}
	}

	void SystemManager::StageCUpdate(float deltaTime)
	{
		_cCurrentTime += deltaTime;

		// 몬스터랑 처음으로 만남
		if (!_isFirstMonster && _isFirstMonsterAction)
		{
			UINT64 key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("MonsterFirstSpawn_AreaC");
			DUOLGameEngine::CameraEventManager::GetInstance()->SetPlayKey(key);
			_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
			_interactiveScript->SetPlayScriptKey(L"DialogueText_08.png");
			_isCameraSequenceMode = true;

			_isFirstMonster = true;
			_isFirstMonsterAction = false;
			_isFirstMonsterCheck = true;
		}
		else if (_isFirstMonster && _isFirstMonsterAction)
		{
			UINT64 key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("MonsterFirstSpawn_AreaC");
			DUOLGameEngine::CameraEventManager::GetInstance()->SetPlayKey(key);
			_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);

			_isCameraSequenceMode = true;
			_isFirstMonsterAction = false;
			_isFirstMonsterCheck = true;
		}

		if (240.0f <= _cCurrentTime && !_isInfoChcek)
		{
			_interactiveScript->SetPlayInfoKey(L"DialogueText_14.png");
			_uiMiniMapObject->SetIsActiveSelf(true);
			_isInfoChcek = true;
		}

		if (_interactiveScript)
		{
			_interactiveScript->ShowCSceneScript();
			_interactiveScript->ScriptPlay(deltaTime);
			_interactiveScript->InfoPlay(deltaTime);
		}

		if (_isCameraSequenceMode && !_cameraInstance->IsPlayMode())
		{
			_isCameraSequenceMode = false;
			_mainCameraController->SetCameraState(DUOLClient::MainCameraState::FOLLOW_PLAYER);
		}
	}

	void SystemManager::BossUI()
	{
	}


	void SystemManager::PlayTotalScene(DUOLCommon::tstring sound)
	{
		PlaySound(_totalSceneClips[sound]);
	}

	void SystemManager::PlayMiddleScene(DUOLCommon::tstring sound)
	{
		_player->PlayScriptSoundClip(_middleSceneClips[sound], false);
	}

	void SystemManager::PlayStageAScene(DUOLCommon::tstring sound)
	{
		_player->PlayScriptSoundClip(_aSceneClips[sound], false);
	}

	void SystemManager::PlayStageBScene(DUOLCommon::tstring sound)
	{
		_player->PlayScriptSoundClip(_bSceneClips[sound], false);
	}

	void SystemManager::PlayStageCScene(DUOLCommon::tstring sound)
	{
		_player->PlayScriptSoundClip(_cSceneClips[sound], false);
	}

	void SystemManager::PlaySound(DUOLGameEngine::AudioClip* soundClip)
	{
		_audioSource->SetAudioClip(soundClip);
		_audioSource->SetIsLoop(false);
		_audioSource->Play();
	}

	void SystemManager::PlayUISound(DUOLCommon::tstring name, bool istrue)
	{
		if (istrue)
			_player->PlayScriptPause();
		else
			_player->PlayScriptPlay();


		if (_uiClips[name] == nullptr)
			return;

		_uiAudioSource->SetAudioClip(_uiClips[name]);
		_uiAudioSource->SetIsLoop(false);
		_uiAudioSource->Play();

	}


	void DUOLClient::SystemManager::OnAwake()
	{
		if (!_systemInstance)
		{
			_systemInstance = this;

		}
		else if (_systemInstance == this)
			return;
		else
			Destroy(this);

	}

	void DUOLClient::SystemManager::OnStart()
	{
		DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		const DUOLCommon::tstring& currentSceneName = currentScene->GetName();

		_cameraManager = DUOLGameEngine::CameraEventManager::GetInstance();

		_soundManager = DUOLGameEngine::SoundManager::GetInstance();

		if (_cameraInstance == nullptr)
		{
			_cameraInstance = DUOLGameEngine::CameraEventManager::GetInstance();
			_cameraInstance->LoadScene();
		}

		if (_interactiveScript == nullptr)
			_interactiveScript = new InteractiveScript();

		_interactiveScript->OnStart(currentSceneName);

		// UI
		_uiClips.insert(std::make_pair(L"ButtonClickSound", _soundManager->GetAudioClip(TEXT("ButtonClickSound"))));
		_uiClips.insert(std::make_pair(L"NoClickButtonSound", _soundManager->GetAudioClip(TEXT("NoClickButtonSound"))));
		_uiClips.insert(std::make_pair(L"Window_Active", _soundManager->GetAudioClip(TEXT("Window_Active"))));
		_uiClips.insert(std::make_pair(L"Window_Inactive", _soundManager->GetAudioClip(TEXT("Window_Inactive"))));

		if (currentSceneName == TEXT("TotalScene"))
			InitializeStageTotal();
		else if (currentSceneName == TEXT("Middle"))
			InitializeMiddle();
		else if (currentSceneName == TEXT("StageA"))
			InitializeStageA();
		else if (currentSceneName == TEXT("StageB"))
			InitializeStageB();
		else if (currentSceneName == TEXT("StageC"))
			InitializeStageC();
		else if(currentSceneName ==TEXT("StageBoss"))
			InitializeStageBoss();

		else
			InitializeStage();
	}

	void DUOLClient::SystemManager::OnUpdate(float deltaTime)
	{

		switch (_currentGameScene)
		{
		case GameScene::Main:
		{
			break;
		}
		// 여긴 항상 카메라가 고정되있어야한다. 
		case GameScene::Total:
		{
			_scriptIndex = _cameraManager->GetSequenceIndex() - 1;
			if (!_cameraManager->IsPlayMode() && !_isTotalSceneEnd)
				FinishTotalScene();

			if (_interactiveScript != nullptr)
			{
				if (_preScriptIndex != _scriptIndex)
				{
					_interactiveScript->ShowTotalSceneScript();
					_preScriptIndex = _scriptIndex;
				}
				_interactiveScript->Play(deltaTime);
			}

			break;
		}
		case GameScene::Middle:
		{
			MiddleUpdate(deltaTime);

			break;
		}
		case GameScene::StageA:
		{
			StageAUpdate(deltaTime);

			break;
		}
		case GameScene::StageB:
		{
			StageBUpdate(deltaTime);
			BSystem(deltaTime);

			break;
		}
		case GameScene::StageC:
		{
			StageCUpdate(deltaTime);

			break;
		}
		case GameScene::ETC:
		{
			if (_isCameraSequenceMode && !_cameraInstance->IsPlayMode())
			{
				_isCameraSequenceMode = false;
				_mainCameraController->SetCameraState(DUOLClient::MainCameraState::FOLLOW_PLAYER);
			}
		}
		}
	}


	void SystemManager::MiniMapChange(DUOLCommon::tstring path)
	{
		_miniMapImage->LoadTexture(path);
	}

	void SystemManager::Reset()
	{
		_isMiddleEvent = false;
		_isFirstMonster = false;
		_isAStageClear = false;
		_isBStageClear = false;
		_isCStageClear = false;
	}

	void SystemManager::FirstMonsterActionSet()
	{
		if (!_isFirstMonsterCheck)
			_isFirstMonsterAction = true;
	}

	void DUOLClient::SystemManager::PlayerCameraAction(std::string name, DUOLGameEngine::Transform* playertransform, bool rot)
	{
		// Camera Mode Change
		_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);

		_cameraInstance->PlayerAction(name, playertransform, rot);

		_isCameraSequenceMode = true;

	}

	void SystemManager::AAreaCameraAction()
	{
		if (_isStartCameraAction)
			return;

		std::vector<UINT64> _sequenceCamera;
		UINT64 key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("MapGimick_AreaA_Part1");
		_sequenceCamera.emplace_back(key);
		key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("MapGimick_AreaA_Part2");
		_sequenceCamera.emplace_back(key);

		// Camera Action Start
		DUOLGameEngine::CameraEventManager::GetInstance()->SetSequenceList(_sequenceCamera);
		DUOLGameEngine::CameraEventManager::GetInstance()->SetSequenceMode(true);

		_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
		_isCameraSequenceMode = true;

		_interactiveScript->SetPlayScriptKey(L"DialogueText_13.png");

		_isStartCameraAction = true;
		_isInfo = true;

	}

	void DUOLClient::SystemManager::DoorCollisionStay()
	{
		//// UI 떠야함
		_infoObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(L"Info10.png");
		_infoObject->SetIsActiveSelf(true);

		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::F))
		{
			_isOpenDoor = true;
			PlayUISound(L"door_sound_effect", false);
			UINT64 key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("MapGimick_AreaB_DoorOpen");
			DUOLGameEngine::CameraEventManager::GetInstance()->SetPlayKey(key);
			_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
			_isCameraSequenceMode = true;
		}
	}

	void SystemManager::CloseDoorUI()
	{
		_infoObject->SetIsActiveSelf(false);
	}

	void SystemManager::BStageCameraDoorAction()
	{
		if (_isBStageDoorAction)
			return;

		std::vector<UINT64> _sequenceCamera;
		UINT64 key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("StageB_DoorGimick_Part1");
		_sequenceCamera.emplace_back(key);
		key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("StageB_DoorGimick_Part2");
		_sequenceCamera.emplace_back(key);

		// Camera Action Start
		DUOLGameEngine::CameraEventManager::GetInstance()->SetSequenceList(_sequenceCamera);
		DUOLGameEngine::CameraEventManager::GetInstance()->SetSequenceMode(true);

		_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
		_isCameraSequenceMode = true;

		_interactiveScript->SetPlayScriptKey(L"DialogueText_14.png");

		_isBStageDoorAction = true;
	}

	void SystemManager::BStageCameraFactoryAction()
	{
		if (_isBStageFactoryAction)
			return;

		_interactiveScript->SetPlayScriptKey(L"DialogueText_15.png");

		_isBStageFactoryAction = true;
	}

	void SystemManager::BStageCameraMonsterWaveAction()
	{
		if (_isBStageMonsterWaveAction)
			return;

		std::vector<UINT64> _sequenceCamera;
		UINT64 key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("StageB_WaveGimick_Part1");
		_sequenceCamera.emplace_back(key);
		key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("StageB_WaveGimick_Part2");
		_sequenceCamera.emplace_back(key);

		// Camera Action Start
		DUOLGameEngine::CameraEventManager::GetInstance()->SetSequenceList(_sequenceCamera);
		DUOLGameEngine::CameraEventManager::GetInstance()->SetSequenceMode(true);

		_dangerObject->SetIsActiveSelf(true);
		_dangerColliderObject->SetIsActiveSelf(true);

		_isBStageMonsterWaveAction = true;
	}

	void SystemManager::SetUiObject(bool value)
	{
		// UI Close
		_uiObject->SetIsActiveSelf(value);
	}

	void SystemManager::HideMiniMapIcon()
	{
		_uiMiniMapObject->SetIsActiveSelf(false);
	}


	void DUOLClient::SystemManager::BSystem(float deltaTime)
	{
		// Door Monster Trigger active
		auto control1 = EnemyManager::GetInstance()->GetEnemyGroupController(L"MonsterSpawnPoint_B_01_Gun05_Normal40");
		auto control2 = EnemyManager::GetInstance()->GetEnemyGroupController(L"Spawn1_MonsterSpawnPoint_B_02_Gun06_Normal40 // Spawn2_WeakEliteMonster2");

		if (control1 && control2)
		{
			if (control1->GetIsClearGroup() && control2->GetIsClearGroup())
				_isDoorMonsterKill = true;
			else
				return;
		}

		// RimLight 
		if (_isDoorMonsterKill)
			RimLightFromDeltaTime(deltaTime);

		// Open Door
		// CollisionStay Check

		if (_isOpenDoor)
			OpenDoor(deltaTime);
	}

	void DUOLClient::SystemManager::RimLightFromDeltaTime(float deltaTime)
	{
		_currentTime += deltaTime;

		if (_rimLightObject == nullptr)
		{
			auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

			for (auto gameObject : gameObjects)
			{
				if (gameObject->GetTag() == TEXT("RimObject"))
				{
					_rimLightObject = gameObject;

					continue;
				}
			}
			if (_rimLightObject == nullptr)
				return;
		}

		auto meshrender = _rimLightObject->GetComponent<DUOLGameEngine::MeshRenderer>();
		meshrender->SetEmissive(DUOLMath::Vector3(253, 253, 0));

		// Dark
		if (3.f <= _currentTime)
		{
			if (6.f <= _currentTime)
				_currentTime = 0.f;

			_rimPower -= (deltaTime * 5);

			if (_rimPower <= 1.5f)
				_rimPower = 1.5f;

			meshrender->SetEmissivePower(_rimPower);
		}
		else
		{
			_rimPower += (deltaTime * 5);

			if (_rimPower <= 1.5f)
				_rimPower = 1.5f;

			meshrender->SetEmissivePower(_rimPower);
		}

	}

	void DUOLClient::SystemManager::OpenDoor(float deltaTime)
	{
		_moveDoor += deltaTime;

		if (_doorObject == nullptr)
		{
			auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

			for (auto gameObject : gameObjects)
			{
				if (gameObject->GetTag() == TEXT("OpenDoor"))
				{
					_doorObject = gameObject;

					continue;
				}
			}

		}

		auto transform = _doorObject->GetComponent<DUOLGameEngine::Transform>();
		auto pos = transform->GetLocalPosition();

		if (1.2f * _moveDoor <= 7.0f)
		{
			pos.y += 1.2f * _moveDoor;

			transform->SetLocalPosition(pos);
		}
		else
			_isOpenDoor = false;
	}

	void SystemManager::CreatePortal(const DUOLCommon::tstring& portalName, const DUOLCommon::tstring& nextSceneName,
		const DUOLMath::Vector3& position)
	{
		DUOLGameEngine::GameObject* portal = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();

		portal->SetName(portalName);

		portal->GetTransform()->SetPosition(position);

		portal->AddComponent<DUOLGameEngine::BoxCollider>()->SetSize(DUOLMath::Vector3(3.f, 1.f, 3.f));

		portal->GetComponent<DUOLGameEngine::BoxCollider>()->SetIsTrigger(true);
		DUOLClient::Portal* portalCom = portal->AddComponent<DUOLClient::Portal>();

		if (_isAStageClear && _isBStageClear && _isCStageClear)
			portalCom->SetNextSceneName(L"StageBoss");
		else
			portalCom->SetNextSceneName(nextSceneName);
	}

	void SystemManager::Die()
	{
		_interactiveScript->SetPlayScriptKey(L"DialogueText_19.png");
	}

	void SystemManager::FinishTotalScene()
	{
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

		_fadeInOut->StartFadeOut(2, [this]()
			{
				DUOLGameEngine::TimeManager::GetInstance()->SetTimeScale(1.f);

				DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(TEXT("Asset/Scene/Middle.dscene"));

			});

		_isTotalSceneEnd = true;
	}


}
