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
#include "DUOLClient/Manager/GameManager.h"
#include "DUOLGameEngine/Manager/SoundManager.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/FadeInOut.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLClient/Player/Player.h"
#include "DUOLGameEngine/ECS/Component/AudioListener.h"
#include "DUOLGameEngine/ECS/Component/AudioSource.h"

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
		);
	}

		SystemManager::SystemManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
		, _isBStageAllMonsterKill(false)
		, _isDoorMonsterKill(false)
		, _isOpenDoor(false)
		, _currentScriptTime(0.0f)
		, _currentInfoTime(0.0f)
		, _currentTime(0.0f)
		, _rimLightObject(nullptr)
		, _doorObject(nullptr)
		, _moveDoor(0.0f)
		, _rimPower(0.f)
		, _mainCameraController(nullptr)
		, _isCameraSequenceMode(false)
		, _currentGameScene(GameScene::Main)
		, _isNextScript(false)
		, _scriptTime(0.f)
		, _infoTime(0.f)
		, _scriptIndex(0)
		, _isEnemyAIPlay(true)
		, _isNextInfo(false)
		, _isShowScript(false)
		, _isShowInfo(false)
		, _cameraInstance(nullptr)
		, _isFirstMonsterAction(false)
		, _isBStageDoorAction(false)
		, _isBStageFactoryAction(false)
		, _isBStageMonsterWaveAction(false)
	{
	}

	DUOLClient::SystemManager::~SystemManager()
	{
		_systemInstance = nullptr;
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

	void SystemManager::InitializeMiddle()
	{
		_currentGameScene = GameScene::Middle;
		_middleSceneClips.clear();
		_scriptList.clear();
		_infoList.clear();

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetName() == TEXT("NarrativeWin"))
			{
				_scriptObject = gameObject;
				_scriptObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("TutorialWin"))
			{
				_infoObject = gameObject;
				_infoObject->SetIsActiveSelf(false);
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
		}


		_scriptList.emplace_back(std::make_tuple(L"DialogueText_04.png", 8.f, false));
		_scriptList.emplace_back(std::make_tuple(L"DialogueText_05.png", 7.f, false));
		_scriptList.emplace_back(std::make_tuple(L"DialogueText_06.png", 6.f, false));
		_scriptList.emplace_back(std::make_tuple(L"DialogueText_07.png", 7.f, false));

		_infoList.emplace_back(std::make_tuple(L"info_01.png", 5.f, false));
		_infoList.emplace_back(std::make_tuple(L"info_02.png", 5.f, false));

		// Dialogue
		_middleSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_23")));
		_middleSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_24")));
		_middleSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_25")));
		_middleSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_26")));
		_middleSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_05")));
		_middleSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_06")));
		_middleSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_07")));
		_middleSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_08")));

		auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		object->GetTransform()->SetParent(this->GetTransform());
		object->SetName(TEXT("UIAudioSource"));
		auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
		_uiAudioSource = comp;


		_isShowScript = true;
		_isNextScript = true;
		_scriptIndex = 0;
		_infoIndex = 0;
	}

	void SystemManager::InitializeStageTotal()
	{
		_currentGameScene = GameScene::Total;

		_totalSceneClips.clear();

		_scriptList.clear();

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Camera"))
			{
				// Main Camera Controller �� ���⿡ �޷��ֽ��ϴ�.
				_mainCameraController = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
				_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
			}
			if (gameObject->GetName() == TEXT("NarrativeWin"))
			{
				_scriptObject = gameObject;
				_scriptObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
				_fadeInOut->StartFadeIn(SCENE_START_FADE_IN, nullptr);
			}
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

		_scriptList.emplace_back(std::make_tuple(L"DialogueText_01.png", 7.f, false));
		_scriptList.emplace_back(std::make_tuple(L"DialogueText_02.png", 12.f, false));
		_scriptList.emplace_back(std::make_tuple(L"DialogueText_03.png", 8.f, false));

		_audioSource = this->GetGameObject()->GetComponent<DUOLGameEngine::AudioSource>();

		_audioListener = this->GetGameObject()->GetComponent<DUOLGameEngine::AudioListener>();

		_totalSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_20")));
		_totalSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_21")));
		_totalSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_22")));

		_isShowScript = true;
		_scriptIndex = -1;
	}

	void SystemManager::InitializeStageA()
	{
		_currentGameScene = GameScene::StageA;

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
		}

		_aSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_10")));

		auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		object->GetTransform()->SetParent(this->GetTransform());
		object->SetName(TEXT("UIAudioSource"));
		auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
		_uiAudioSource = comp;
	}

	void SystemManager::InitializeStageB()
	{
		_currentGameScene = GameScene::StageB;

		_currentScriptTime = 0.f;
		_currentInfoTime = 0.f;

		_scriptIndex = -1;
		_infoIndex = 0;

		_scriptTime = 0.f;
		_infoTime = 0.f;

		_isNextScript = true;
		_isNextInfo = false;

		_isBStageDoorAction = false;
		_isBStageFactoryAction = false;
		_isBStageMonsterWaveAction = false;

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetName() == TEXT("NarrativeWin"))
			{
				_scriptObject = gameObject;
				_scriptObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("TutorialWin"))
			{
				_infoObject = gameObject;
				_infoObject->SetIsActiveSelf(false);
			}
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
		}

		if (_cameraInstance == nullptr)
		{
			_cameraInstance = DUOLGameEngine::CameraEventManager::GetInstance();
		}

		// StageB
		_bSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_11")));
		_bSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_12")));
		_bSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_12A")));
		_bSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_13")));

		_scriptList.emplace_back(std::make_tuple(L"DialogueText_14.png", 8.f, false));
		_scriptList.emplace_back(std::make_tuple(L"DialogueText_15.png", 10.f, false));
		_scriptList.emplace_back(std::make_tuple(L"DialogueText_15A.png", 9.f, false));
		_scriptList.emplace_back(std::make_tuple(L"DialogueText_14.png", 7.f, false));

		_infoList.emplace_back(std::make_tuple(L"info_01.png", 5.f, false));
		_infoList.emplace_back(std::make_tuple(L"info_01.png", 5.f, false));


		_infoList.emplace_back(std::make_tuple(L"info_01.png", 5.f, false));


		auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		object->GetTransform()->SetParent(this->GetTransform());
		object->SetName(TEXT("UIAudioSource"));
		auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
		_uiAudioSource = comp;

	}

	void SystemManager::InitializeStageC()
	{
		_currentGameScene = GameScene::StageC;

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
		}

		_cSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_14")));
		_cSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_15")));
		_cSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_02")));
		_cSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_03")));
		_cSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_04")));
		_cSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_16")));
		_cSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_17")));
		_cSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_18")));
		_cSceneClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_19")));

		auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		object->GetTransform()->SetParent(this->GetTransform());
		object->SetName(TEXT("UIAudioSource"));
		auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
		_uiAudioSource = comp;

	}

	void SystemManager::InitializeStage()
	{
		_scriptList.clear();

		_currentGameScene = GameScene::ETC;

		_isBStageDoorAction = false;

		_scriptIndex = 0;

		_infoIndex = 0;

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
				_mainCameraController = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();

			}
			if (gameObject->GetName() == TEXT("Player"))
			{
				_player = gameObject->GetComponent<DUOLClient::Player>();
			}
		}

		if (_cameraInstance == nullptr)
		{
			_cameraInstance = DUOLGameEngine::CameraEventManager::GetInstance();
		}

		auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		object->GetTransform()->SetParent(this->GetTransform());
		object->SetName(TEXT("UIAudioSource"));
		auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
		_uiAudioSource = comp;
	}

	void SystemManager::MiddleUpdate(float deltaTime)
	{
		if (_isMiddleEvent)
			return;

		if (_fadeInOut->GetFadeMode() == DUOLGameEngine::FadeInOutMode::DONE && _isNextScript)
		{
			if (_scriptList.size() <= _scriptIndex)
			{
				_isShowScript = false;
				_isNextScript = false;
				_scriptObject->SetIsActiveSelf(false);
				return;
			}
			_scriptObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(std::get<0>(_scriptList[_scriptIndex]));
			_scriptObject->SetIsActiveSelf(true);
			_isNextScript = false;
			_isShowScript = true;
			_currentScriptTime = 0.f;
			_scriptTime = std::get<1>(_scriptList[_scriptIndex]);
			_player->PlayScriptSoundClip(_middleSceneClips[_scriptIndex], false);

			if (_scriptIndex == 1)
			{
				_isShowInfo = true;
			}
		}

		if (_isShowInfo && _isNextInfo)
		{
			if (_infoList.size() <= _infoIndex)
			{
				_isShowInfo = false;
				_isNextInfo = false;
				_infoObject->SetIsActiveSelf(false);
				return;
			}
			_infoObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(std::get<0>(_infoList[_infoIndex]));
			_infoObject->SetIsActiveSelf(true);
			_isNextInfo = false;
			_isShowInfo = true;
			_currentInfoTime = 0.f;
			_infoTime = std::get<1>(_infoList[_infoIndex]);
		}


		ScriptCheck(deltaTime);
		Infocheck(deltaTime);

		// all script and info finish middle event no action
		if (_scriptList.size() <= _scriptIndex && _infoList.size() <= _infoIndex)
			_isMiddleEvent = true;

	}

	void SystemManager::StageBUpdate(float deltaTime)
	{
#pragma region FirstMonster Action
		// ���Ͷ� ó������ ����
		if (!_isFirstMonster && !_firstMonsterClips.empty() && _isFirstMonsterAction)
		{
			// Monster Action
			if (_scriptIndex == -1)
			{
				UINT64 key = DUOLGameEngine::CameraEventManager::GetInstance()->GetKey("MonsterFirstSpawn_AreaB");
				DUOLGameEngine::CameraEventManager::GetInstance()->SetPlayKey(key);
				_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
				_isCameraSequenceMode = true;
				_scriptIndex = 0;
			}

			if (_isNextScript)
			{
				if (_firstMonsterScriptCount <= _scriptIndex)
				{
					_isShowScript = false;
					_isNextScript = false;
					_scriptObject->SetIsActiveSelf(false);
					_isFirstMonster = true;
					return;
				}
				_scriptObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(std::get<0>(_scriptList[_scriptIndex]));
				_scriptObject->SetIsActiveSelf(true);
				_isNextScript = false;
				_isShowScript = true;
				_currentScriptTime = 0.f;
				_scriptTime = std::get<1>(_scriptList[_scriptIndex]);
				_player->PlayScriptSoundClip(_firstMonsterClips[_scriptIndex], false);
				if (_scriptIndex == 2)
				{
					_isShowInfo = true;
				}
			}

			if (_isShowInfo && _isNextInfo)
			{
				if (_infoList.size() <= _infoIndex)
				{
					_isShowInfo = false;
					_isNextInfo = false;
					_infoObject->SetIsActiveSelf(false);
					return;
				}
				_infoObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(std::get<0>(_infoList[_infoIndex]));
				_infoObject->SetIsActiveSelf(true);
				_isNextInfo = false;
				_isShowInfo = true;
				_currentInfoTime = 0.f;
				_infoTime = std::get<1>(_infoList[_infoIndex]);
			}

			ScriptCheck(deltaTime);
			Infocheck(deltaTime);


		}

#pragma endregion 

		if (_isBStageDoorAction)
		{
			ScriptCheck(deltaTime, true);
			Infocheck(deltaTime, true);

			if (_scriptIndex == 0)
			{
				if (std::get<2>(_scriptList[_scriptIndex]) == true && std::get<2>(_infoList[_infoIndex]) == false)
				{
					_infoObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(std::get<0>(_infoList[_infoIndex]));
					_infoObject->SetIsActiveSelf(true);
					_isNextInfo = false;
					_isShowInfo = true;
					_currentInfoTime = 0.f;
					_infoTime = std::get<1>(_infoList[_infoIndex]);
				}
			}
			else
			{
				if (std::get<2>(_scriptList[_scriptIndex - 1]) == true && std::get<2>(_infoList[_infoIndex - 1]) == false)
				{
					_infoObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(std::get<0>(_infoList[_infoIndex]));
					_infoObject->SetIsActiveSelf(true);
					_isNextInfo = false;
					_isShowInfo = true;
					_currentInfoTime = 0.f;
					_infoTime = std::get<1>(_infoList[_infoIndex]);
				}
			}
		}

		if (_isCameraSequenceMode && !_cameraInstance->IsPlayMode())
		{
			_isCameraSequenceMode = false;
			_mainCameraController->SetCameraState(DUOLClient::MainCameraState::FOLLOW_PLAYER);

			if(!_isBStageMonsterWaveAction)
			{
				//script
			}
		}
	}

	void SystemManager::ShowScript()
	{
	}

	void SystemManager::ShowInfoUI()
	{
	}

	void SystemManager::BossUI()
	{
	}

	void SystemManager::ScriptCheck(float deltaTime, bool check)
	{
		if (!_isShowScript)
			return;

		if (!_isNextScript)
		{
			_currentScriptTime += deltaTime;

			if (_scriptTime < _currentScriptTime)
			{
				if (!check)
					_isShowScript = true;
				else
				{
					_isShowScript = false;
					_scriptObject->SetIsActiveSelf(false);
				}
				_isNextScript = true;
				std::get<2>(_scriptList[_scriptIndex]) = true;
				_scriptIndex++;
			}
		}
	}

	void SystemManager::Infocheck(float deltaTime, bool check)
	{
		if (!_isShowInfo)
			return;

		if (!_isNextInfo)
		{
			_currentInfoTime += deltaTime;

			if (_infoTime < _currentInfoTime)
			{
				if (!check)
					_isShowInfo = true;
				else
				{
					_isShowInfo = false;
					_infoObject->SetIsActiveSelf(false);
				}

				_isNextInfo = true;
				std::get<2>(_scriptList[_infoIndex]) = true;
				_infoIndex++;
			}
		}
	}


	void SystemManager::PlaySound(DUOLGameEngine::AudioClip* soundClip)
	{
		_audioSource->SetAudioClip(soundClip);
		_audioSource->SetIsLoop(false);
		_audioSource->Play();
	}

	void SystemManager::PlayUISound(UISound uiindex, bool istrue)
	{
		if (istrue)
			_player->PlayScriptPause();
		else
			_player->PlayScriptPlay();


		int soundIdx = static_cast<int>(uiindex);
		if (0 > soundIdx || soundIdx > (static_cast<int>(PlayerSoundTable::NONE) - 1))
			return;

		_uiAudioSource->SetAudioClip(_uiClips[soundIdx]);
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
		_firstMonsterClips.clear();
		_scriptList.clear();
		_infoList.clear();

		DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		const DUOLCommon::tstring& currentSceneName = currentScene->GetName();

		_cameraManager = DUOLGameEngine::CameraEventManager::GetInstance();

		_soundManager = DUOLGameEngine::SoundManager::GetInstance();

		// UI
		_uiClips.push_back(_soundManager->GetAudioClip(TEXT("ButtonClickSound")));
		_uiClips.push_back(_soundManager->GetAudioClip(TEXT("NoClickButtonSound")));
		_uiClips.push_back(_soundManager->GetAudioClip(TEXT("Window_Active")));
		_uiClips.push_back(_soundManager->GetAudioClip(TEXT("Window_Inactive")));

		// first action
		if (!_isFirstMonster)
		{
			_firstMonsterClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_05")));
			_firstMonsterClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_06")));
			_firstMonsterClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_07")));
			_firstMonsterClips.push_back(_soundManager->GetAudioClip(TEXT("NPC_08")));

			_scriptList.emplace_back(std::make_tuple(L"DialogueText_08.png", 9.f, false));
			_scriptList.emplace_back(std::make_tuple(L"DialogueText_09.png", 14.f, false));
			_scriptList.emplace_back(std::make_tuple(L"DialogueText_10.png", 11.f, false));
			_scriptList.emplace_back(std::make_tuple(L"DialogueText_11.png", 11.f, false));

			_infoList.emplace_back(std::make_tuple(L"info_01.png", 5.f, false));
			_infoList.emplace_back(std::make_tuple(L"info_02.png", 5.f, false));
			_infoList.emplace_back(std::make_tuple(L"info_01.png", 5.f, false));

			_firstMonsterScriptCount = _scriptList.size();
		}


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
		// ���� �׻� ī�޶� �������־���Ѵ�. 
		case GameScene::Total:
		{
			if (_scriptIndex != _cameraManager->GetSequenceIndex() - 1)
			{
				_scriptIndex = _cameraManager->GetSequenceIndex() - 1;
				if (_scriptIndex < 0 && _totalSceneClips.size() <= _scriptIndex)
				{
					if (!_cameraManager->IsPlayMode())
						FinishTotalScene();
					_scriptIndex = -1;
					return;
				}
				ChangeScript(_scriptIndex);
				PlaySound(_totalSceneClips[_scriptIndex]);
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

	void DUOLClient::SystemManager::PlayerCameraAction(std::string name, DUOLGameEngine::Transform* playertransform)
	{
		// Camera Mode Change
		_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);

		_cameraInstance->PlayerAction(name, playertransform);

		_isCameraSequenceMode = true;

	}

	void DUOLClient::SystemManager::DoorCollisionStay()
	{
		//// UI ������
		_infoObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(std::get<0>(_infoList[_infoList.size() - 1]));
		_infoObject->SetIsActiveSelf(true);

		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::F))
		{
			_isOpenDoor = true;

			PlayerCameraAction("MapGimick_AreaB_DoorOpen", _player->GetTransform());
		}
	}

	void SystemManager::CloseDoorUI()
	{
		_infoObject->SetIsActiveSelf(false);
	}

	void SystemManager::BStageCameraFirstAction()
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

		_scriptObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(std::get<0>(_scriptList[_scriptIndex]));
		_scriptObject->SetIsActiveSelf(true);
		_isNextScript = false;
		_isShowScript = true;
		_currentScriptTime = 0.f;
		_scriptTime = std::get<1>(_scriptList[_scriptIndex]);
		_player->PlayScriptSoundClip(_bSceneClips[_scriptIndex- _firstMonsterScriptCount], false);

		_isBStageDoorAction = true;
	}

	void SystemManager::BStageCameraFactoryAction()
	{
		if (_isBStageFactoryAction)
			return;

		_scriptObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(std::get<0>(_scriptList[_scriptIndex]));
		_scriptObject->SetIsActiveSelf(true);
		_isNextScript = false;
		_isShowScript = true;
		_currentScriptTime = 0.f;
		_scriptTime = std::get<1>(_scriptList[_scriptIndex]);
		_player->PlayScriptSoundClip(_bSceneClips[_scriptIndex - _firstMonsterScriptCount], false);

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

		_isBStageMonsterWaveAction = false;
	}

	void SystemManager::ChangeScript(int index)
	{
		if (0 > index || index > _scriptList.size())
			return;

		if (_scriptObject == nullptr)
			return;

		_scriptObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(std::get<0>(_scriptList[index]));
		_scriptObject->SetIsActiveSelf(true);
		_isNextScript = false;
		_currentTime = 0.f;
		_scriptTime = std::get<1>(_scriptList[index]);
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
		if (5.f <= _currentTime)
		{
			if (10.f <= _currentTime)
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

		if (1.5f * _moveDoor <= 6.0f)
		{
			pos.y += 1.5f * _moveDoor;

			transform->SetLocalPosition(pos);
		}
		else
			_isOpenDoor = false;
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

	}
}
