#include "DUOLClient/Manager/GameManager.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"

#include <rttr/registration>

#include "DUOLClient/Camera/MainCameraController.h"
#include "DUOLGameEngine/ECS/Component/FadeInOut.h"
#include "DUOLClient/ECS/Component/Map/Portal.h"
#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLClient/Manager/UIDataManager.h"
#include "DUOLClient/Player/Player.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Component/AudioSource.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/Manager/CameraEventManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/SoundManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSecondsRealtime.h"
#include "DUOLGameEngine/ECS/Object/AudioClip.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/ECS/Component/Text.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::GameManager>("GameManager")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("BGMClipName", &DUOLClient::GameManager::_audioClipName)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::String)
	);
}

namespace DUOLClient
{
	DUOLClient::GameManager* DUOLClient::GameManager::_instance = nullptr;

	GameManager::GameManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
		, _isFirstStart(true)
		, _currentGameMode(GameMode::DEFAULT)
		, _canPausable(false)
		, _isOutInGameUIMode(false)
		, _isCursorShowing(true)
		, _audioClipName(TEXT(""))
		, _currentGameScene(GameScene::Main)
		, _playerDie(false)
		, _totalPoint(0)
		, _isCredit(false)
	{

	}

	GameManager::~GameManager()
	{
		if (_instance == this)
			_instance = nullptr;
	}

	void GameManager::SetStartPlayerData()
	{
		// TODO
	}

	void GameManager::SaveEndPlayerData()
	{
		// TODO
	}

	void GameManager::ChangeScene(const GameMessage<DUOLCommon::tstring>& message)
	{
		if (_fadeInOut != nullptr)
		{
			_fadeInOut->StartFadeOut(SCENE_END_FADE_OUT, [this, sceneName = message._parameter]()
				{
					if (_currentGameMode == DUOLClient::GameMode::UI_MODE)
						_isOutInGameUIMode = true;

					DUOLGameEngine::TimeManager::GetInstance()->SetTimeScale(1.f);
					DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(sceneName);
				});
		}
		else
		{
			DUOLGameEngine::TimeManager::GetInstance()->SetTimeScale(1.f);
			DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(message._parameter);
		}
	}

	void GameManager::MouseLock()
	{
		//DUOLGameEngine::InputManager::GetInstance()->SetGameLockMode(true);
		//DUOLGameEngine::InputManager::GetInstance()->ShowCursorI(false);

		//_isCursorShowing = false;
	}

	void GameManager::MouseUnLock()
	{
		DUOLGameEngine::InputManager::GetInstance()->SetGameLockMode(false);

		DUOLGameEngine::InputManager::GetInstance()->ShowCursorI(true);
		_isCursorShowing = true;
	}

	DUOLGameEngine::CoroutineHandler GameManager::StartBulletTimeAll(float duration)
	{
		DUOLGameEngine::TimeManager::GetInstance()->SetTimeScale(BULLET_TIME_SCALE);

		_currentGameMode = GameMode::IN_BULLET_TIME_ALL;

		co_yield std::make_shared<DUOLGameEngine::WaitForSecondsRealtime>(duration);

		DUOLGameEngine::TimeManager::GetInstance()->SetTimeScale(1.f);

		_currentGameMode = GameMode::DEFAULT;
	}

	DUOLGameEngine::CoroutineHandler GameManager::StartBulletTimePlayer(float duration)
	{
		_currentGameMode = GameMode::IN_BULLET_TIME_PLAYER;

		co_yield std::make_shared<DUOLGameEngine::WaitForSecondsRealtime>(duration);

		_currentGameMode = GameMode::DEFAULT;
	}

	void GameManager::StartUIMode()
	{
		MouseUnLock();

		SystemManager::GetInstance()->PlayUISound(L"Window_Active", true);

		_gameModePrevUIMode = _currentGameMode;

		_currentGameMode = GameMode::UI_MODE;

		_timeScalePrevUIMode = DUOLGameEngine::TimeManager::GetInstance()->GetTimeScale();

		DUOLGameEngine::TimeManager::GetInstance()->SetTimeScale(0.f);

		auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		DUOLGameEngine::GameObject* pauseUI = nullptr;

		for (auto gameObject : allGameObjects)
		{
			if (gameObject->GetName() == TEXT("Pause"))
			{
				pauseUI = gameObject;

				pauseUI->SetIsActiveSelf(true);

				DUOLGameEngine::InputManager::GetInstance()->SetUIMouseMode(true);
			}
			if (gameObject->GetName() == TEXT("OptionUI"))
			{
				gameObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("PauseMain"))
			{
				gameObject->SetIsActiveSelf(true);
			}
		}

		_isOutInGameUIMode = false;
	}

	void GameManager::EndUIMode()
	{
		SystemManager::GetInstance()->PlayUISound(L"Window_Inactive", false);

		auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		DUOLGameEngine::GameObject* pauseUI = nullptr;

		for (auto gameObject : allGameObjects)
		{
			if (gameObject->GetName() == TEXT("Pause"))
			{
				pauseUI = gameObject;

				pauseUI->SetIsActiveSelf(true);

				DUOLGameEngine::InputManager::GetInstance()->SetUIMouseMode(true);
			}
			if (gameObject->GetName() == TEXT("OptionUI"))
			{
				gameObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("PauseMain"))
			{
				gameObject->SetIsActiveSelf(false);
			}
		}

		if (pauseUI != nullptr)
		{
			pauseUI->SetIsActiveSelf(false);
			DUOLGameEngine::InputManager::GetInstance()->SetUIMouseMode(false);
		}

		_currentGameMode = _gameModePrevUIMode;
		_isOutInGameUIMode = false;
		_timeScalePrevUIMode == 0.f
			? DUOLGameEngine::TimeManager::GetInstance()->SetTimeScale(1.f)
			: DUOLGameEngine::TimeManager::GetInstance()->SetTimeScale(_timeScalePrevUIMode);

		MouseLock();
		_isOutInGameUIMode = false;

	}

	void GameManager::FadeOut()
	{

		if (_fadeInOut != nullptr)
		{
			_fadeInOut->StartFadeOut(SCENE_END_FADE_OUT, [this]()
				{
					if (_currentGameMode == DUOLClient::GameMode::UI_MODE)
						_isOutInGameUIMode = true;

					DUOLGameEngine::TimeManager::GetInstance()->SetTimeScale(1.f);
					MouseUnLock();
				});
		}
	}

	void GameManager::OpenCredit()
	{
		if (_isCredit)
			return;

		_creditTime = 0.f;
		if (_isCredit == false)
			_creditObject->SetIsActiveSelf(true);
		_isCredit = true;

	}

	void GameManager::Reset()
	{
		if (!_isReset)
			return;

		UIDataManager::_koCount = 0;

		SystemManager::_isAStageClear = false;
		SystemManager::_isBStageClear = false;
		SystemManager::_isCStageClear = false;
		SystemManager::_isBossClear = false;
		SystemManager::_isFirstMonster = false;
		SystemManager::_isBossClear = false;

		Player::SetCurrentOverdrivePoint(0);

	}

	void GameManager::Skip()
	{
		if (_skipFade == nullptr)
			return;

		if (_isCredit)
		{
			_skipFade->GetGameObject()->SetIsActiveSelf(false);
			return;
		}

		if (_skipFade->GetFadeMode() == DUOLGameEngine::FadeInOutMode::DONE || _skipFade->GetFadeMode() == DUOLGameEngine::FadeInOutMode::NOT)
		{
			if (_skilFadeMode == DUOLGameEngine::FadeInOutMode::DONE || _skilFadeMode == DUOLGameEngine::FadeInOutMode::FADE_IN)
			{
				_skipFade->StartBlinkIn(1, [this]()
					{
						_skilFadeMode = DUOLGameEngine::FadeInOutMode::FADE_OUT;
					});
			}
			else if (_skilFadeMode == DUOLGameEngine::FadeInOutMode::FADE_OUT)
			{
				_skipFade->StartBlinkOut(1, [this]()
					{
						_skilFadeMode = DUOLGameEngine::FadeInOutMode::FADE_IN;
					});
			}
		}
	}

	DUOLGameEngine::CoroutineHandler GameManager::StartFadeIn()
	{
		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(0.5f);

		if (_fadeInOut != nullptr)
		{
			_fadeInOut->StartFadeIn(SCENE_START_FADE_IN, [this]()
				{
					MouseLock();
				});
		}
	}

	void GameManager::CreatePortal(DUOLGameEngine::Scene* scene, const DUOLCommon::tstring& portalName, const DUOLCommon::tstring& nextSceneName,
		const DUOLMath::Vector3& position)
	{
		DUOLGameEngine::GameObject* portal = scene->CreateEmpty();

		portal->SetName(portalName);

		portal->GetTransform()->SetPosition(position);

		portal->AddComponent<DUOLGameEngine::BoxCollider>()->SetSize(DUOLMath::Vector3(3.f, 1.f, 3.f));

		portal->GetComponent<DUOLGameEngine::BoxCollider>()->SetIsTrigger(true);

		DUOLClient::Portal* portalCom = portal->AddComponent<DUOLClient::Portal>();

		portalCom->SetNextSceneName(nextSceneName);
	}

	void GameManager::InitializeMiddle(DUOLGameEngine::Scene* middle)
	{
		_currentGameScene = GameScene::Middle;

		SetBGM(L"MiddleScene");

#pragma region PORTALS
		if (!SystemManager::GetInstance()->IsAStage())
			CreatePortal(middle, TEXT("Portal_A"), TEXT("StageA"), MIDDLE_PORTAL_TO_A_POSITION);
		if (!SystemManager::GetInstance()->IsBStage())
			CreatePortal(middle, TEXT("Portal_B"), TEXT("StageB"), MIDDLE_PORTAL_TO_B_POSITION);
		if (!SystemManager::GetInstance()->IsCStage())
			CreatePortal(middle, TEXT("Portal_C"), TEXT("StageC"), MIDDLE_PORTAL_TO_C_POSITION);
		
#pragma endregion

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
			}
			if (gameObject->GetTag() == TEXT("Player"))
			{
				UIDataManager::GetInstance()->InitializeMiddle(gameObject);
			}
			if (gameObject->GetTag() == TEXT("MainCamera"))
			{
				// Main Camera Controller 는 여기에 달려있습니다.
				auto _mainCameraController = gameObject->GetTransform()->GetParent()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
				_mainCameraController->SetCameraState(DUOLClient::MainCameraState::FOLLOW_PLAYER);
				_mainCameraController->SetCameraInitPos();
			}
			if (gameObject->GetTag() == TEXT("MiniMap"))
			{
				if(SystemManager::GetInstance()->IsAStage() && !SystemManager::GetInstance()->IsBStage()&& !SystemManager::GetInstance()->IsCStage())
				gameObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(L"MinImap_Middle_BC.png");
				else if(!SystemManager::GetInstance()->IsAStage() && SystemManager::GetInstance()->IsBStage() && !SystemManager::GetInstance()->IsCStage())
					gameObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(L"MinImap_Middle_AC.png");
				else if (!SystemManager::GetInstance()->IsAStage() && !SystemManager::GetInstance()->IsBStage() && SystemManager::GetInstance()->IsCStage())
					gameObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(L"MinImap_Middle_AB.png");
				else if (SystemManager::GetInstance()->IsAStage() && SystemManager::GetInstance()->IsBStage() && !SystemManager::GetInstance()->IsCStage())
					gameObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(L"MinImap_Middle_C.png");
				else if (SystemManager::GetInstance()->IsAStage() && !SystemManager::GetInstance()->IsBStage() && SystemManager::GetInstance()->IsCStage())
					gameObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(L"MinImap_Middle_B.png");
				else if (!SystemManager::GetInstance()->IsAStage() && SystemManager::GetInstance()->IsBStage() && SystemManager::GetInstance()->IsCStage())
					gameObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(L"MinImap_Middle_A.png");
				else if (!SystemManager::GetInstance()->IsAStage() && !SystemManager::GetInstance()->IsBStage() && !SystemManager::GetInstance()->IsCStage())
					gameObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(L"MiddleMiniMap.png");

			}
		}

		StartCoroutine(&DUOLClient::GameManager::StartFadeIn);
	}

	void GameManager::InitializeStageTotal()
	{
		_currentGameScene = GameScene::Total;

		StopBGM();

		DUOLClient::MainCameraController* _mainCameraController;

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Camera"))
			{
				// Main Camera Controller 는 여기에 달려있습니다.
				_mainCameraController = gameObject->GetTransform()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
				_mainCameraController->SetCameraState(DUOLClient::MainCameraState::CAMERA_SEQUENCE);
			}
		}
	}

	void GameManager::InitializeStageA(DUOLGameEngine::Scene* stageA)
	{
		_currentGameScene = GameScene::StageA;

		//CreatePortal(stageA, TEXT("Portal_Middle"), TEXT("Middle"), A_PORTAL_TO_MIDDLE_POSITION);

		SetBGM(L"BattleScene");

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
			}
		}

		StartCoroutine(&DUOLClient::GameManager::StartFadeIn);

		UIDataManager::GetInstance()->InitializeStageA();

	}

	void GameManager::InitializeStageB(DUOLGameEngine::Scene* stageB)
	{
		_currentGameScene = GameScene::StageB;

		//CreatePortal(stageB, TEXT("Portal_Middle"), TEXT("Middle"), B_PORTAL_TO_MIDDLE_POSITION);

		SetBGM(L"BattleScene");

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
			}
		}

		StartCoroutine(&DUOLClient::GameManager::StartFadeIn);

		UIDataManager::GetInstance()->InitializeStageB();
	}

	void GameManager::InitializeStageC(DUOLGameEngine::Scene* stageC)
	{
		_currentGameScene = GameScene::StageC;

		//CreatePortal(stageC, TEXT("Portal_Middle"), TEXT("Middle"), C_PORTAL_TO_MIDDLE_POSITION);

		SetBGM(L"BattleScene");

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
			}
		}

		StartCoroutine(&DUOLClient::GameManager::StartFadeIn);

		UIDataManager::GetInstance()->InitializeStageC();
	}

	void GameManager::InitializeStageBoss(DUOLGameEngine::Scene* stageC)
	{
		_currentGameScene = GameScene::StageBoss;

		SetBGM(L"BossBattleScene");

		CreatePortal(stageC, TEXT("Portal_Middle"), TEXT("Middle"), BOSS_PORTAL_TO_MIDDLE_POSITION);

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
			}
			if (gameObject->GetTag() == TEXT("Player"))
			{
				UIDataManager::GetInstance()->InitializeMiddle(gameObject);
			}
			if (gameObject->GetTag() == TEXT("MainCamera"))
			{
				// Main Camera Controller 는 여기에 달려있습니다.
				auto _mainCameraController = gameObject->GetTransform()->GetParent()->GetGameObject()->GetComponent<DUOLClient::MainCameraController>();
				_mainCameraController->SetCameraState(DUOLClient::MainCameraState::FOLLOW_PLAYER);
				_mainCameraController->SetCameraInitPos();
			}
		}

		StartCoroutine(&DUOLClient::GameManager::StartFadeIn);

		// UIDataManager::GetInstance()->InitializeStageC();
	}

	void GameManager::InitializeStageClear(DUOLGameEngine::Scene* stageC)
	{
		_currentGameScene = GameScene::StageClear;

		_isCredit = false;

		_isReset = false;

		_creditTime = 0.f;

		SetBGM(L"ResultWindow");

		float totaltime = DUOLGameEngine::TimeManager::GetInstance()->GetRealtimeSinceStartup();

		int monstercount = DUOLClient::UIDataManager::_koCount;

		CalculatePoint(totaltime, monstercount);

		DUOLCommon::tstring monstercountstr = DUOLCommon::StringHelper::ToTString(std::to_string(monstercount));

		int total = (monstercount * 10) + (10000 - totaltime);

		DUOLCommon::tstring totalstr = DUOLCommon::StringHelper::ToTString(std::to_string(total));

		int hour; int minutes; int remainingsecond;

		convertSecondsToTime(totaltime, hour, minutes, remainingsecond);

		std::string time = std::to_string(hour) + " : " + std::to_string(minutes) + " : " + std::to_string(remainingsecond);

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetName() == TEXT("KOsText"))
			{
				gameObject->GetComponent<DUOLGameEngine::Text>()->SetText(monstercountstr);
			}
			if (gameObject->GetName() == TEXT("Time"))
			{
				gameObject->GetComponent<DUOLGameEngine::Text>()->SetText(DUOLCommon::StringHelper::ToTString(time));
			}
			if (gameObject->GetName() == TEXT("Rank"))
			{
				if (35 <= _totalPoint)
					gameObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(L"09_gameclear_rank_s.png");
				else if (25 <= _totalPoint)
					gameObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(L"09_gameclear_rank_a.png");
				else if (15 <= _totalPoint)
					gameObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(L"09_gameclear_rank_b.png");
				else
					gameObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(L"09_gameclear_rank_c.png");
			}
			if (gameObject->GetName() == TEXT("ScoreText"))
			{
				gameObject->GetComponent<DUOLGameEngine::Text>()->SetText(totalstr);
			}
			if (gameObject->GetName() == TEXT("Credit"))
			{
				_creditObject = gameObject;
				gameObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("Skip"))
			{
				gameObject->SetIsActiveSelf(true);
				_skipFade = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
			}
		}

		_skilFadeMode == DUOLGameEngine::FadeInOutMode::DONE;

		StartCoroutine(&DUOLClient::GameManager::StartFadeIn);
	}

	void GameManager::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		if (_instance != nullptr && _instance != this)
		{
			// 유일성 보장
			Destroy(GetGameObject());
			GetGameObject()->SetIsActiveSelf(false);

			return;
		}

		// 게임 매니저는 게임의 시작과 끝까지 같이 합니다 ..!
		DontDestroyOnLoad(static_cast<DUOLGameEngine::GameObject*>(GetGameObject()));

		_instance = this;

	}

	void GameManager::OnStart()
	{
		MonoBehaviourBase::OnStart();

		DUOL_ENGINE_INFO(DUOL_CONSOLE, "GameManager 'OnStart' function called.")

			_fadeInOut = nullptr;

		//if (DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetName() == TEXT("Main") || DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetName() == TEXT("CutScene"))
		//{
		//	_canPausable = false;
		//	MouseUnLock();
		//}
		//else
		//{
		//	_canPausable = true;
		//	MouseLock();
		//}

		_canPausable = true;
		MouseLock();

		DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		const DUOLCommon::tstring& currentSceneName = currentScene->GetName();

		if (_bgmAudioSource == nullptr)
			_bgmAudioSource = GetGameObject()->AddComponent<DUOLGameEngine::AudioSource>();

		if (currentSceneName == TEXT("Middle"))
			InitializeMiddle(currentScene);
		else if (currentSceneName == TEXT("TotalScene"))
			InitializeStageTotal();
		else if (currentSceneName == TEXT("BattleTest"))
			InitializeStageA(currentScene);
		else if (currentSceneName == TEXT("StageA"))
			InitializeStageA(currentScene);
		else if (currentSceneName == TEXT("StageB"))
			InitializeStageB(currentScene);
		else if (currentSceneName == TEXT("StageC"))
			InitializeStageC(currentScene);
		else if (currentSceneName == TEXT("StageBoss"))
			InitializeStageBoss(currentScene);
		else if (currentSceneName == TEXT("CutScene"))
		{
			SetBGM(L"CutScene");

			_currentGameScene = GameScene::Cut;
		}
		else if (currentSceneName == TEXT("main") || currentSceneName == TEXT("Main"))
		{
			Reset();
			SetBGM(L"MainTitleScene");
		}
		else if (currentSceneName == TEXT("ClearScene"))
			InitializeStageClear(currentScene);
		else
		{
			_canPausable = false;
			MouseUnLock();
		}

		// 어차피 시작은 무조건 디폴트다.
		_currentGameMode = GameMode::DEFAULT;


		//GraphicsSetting
		auto graphicsSetting = DUOLGameEngine::GraphicsManager::GetInstance()->GetGraphicsSetting();
		graphicsSetting._rimLight._clipPoint = 0.15f;
		graphicsSetting._rimLight._rimDecrease = 1.5f;
		DUOLGameEngine::GraphicsManager::GetInstance()->SetGraphicsSetting(graphicsSetting);
	}

	void GameManager::OnUpdate(float deltaTime)
	{
		//// LoadScene (개발자용)
		//if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::F5))
		//{
		//	DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(TEXT("Asset/Scene/Middle.dscene"));
		//}
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::End))
		{
			DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(TEXT("Asset/Scene/StageBoss.dscene"));
		}

		//if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::F7))
		//{
		//	DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(TEXT("Asset/Scene/StageB.dscene"));
		//}

		//if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::F8))
		//{
		//	DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(TEXT("Asset/Scene/StageC.dscene"));
		//}

		if (_currentGameScene == GameScene::StageClear)
		{
			_creditTime += deltaTime;

			Skip();

			if (_isCredit == true && 30.f <= _creditTime && _isReset == false)
			{
				_isReset = true;
				DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(TEXT("Asset/Scene/Main.dscene"));
			}

			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Space))
			{
				OpenCredit();
			}
		}

		// UI_MODE
		if (_currentGameMode != GameMode::UI_MODE && _canPausable && DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Escape)
			&& _currentGameScene != GameScene::Cut && _currentGameScene != GameScene::Total && !_playerDie && _currentGameScene != GameScene::Main)
		{
			// TODO : UI Mode ..?
			StartUIMode();
		}
		else if (_currentGameMode == GameMode::UI_MODE && _canPausable)
		{
			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Escape) || _isOutInGameUIMode)
			{
				EndUIMode();
			}
			else
			{

			}
		}


		if (_fadeInOut == nullptr && _canPausable == true)
		{
			auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

			for (auto gameObject : gameObjects)
			{
				if (gameObject->GetTag() == TEXT("Fade"))
				{
					_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
				}
			}

			StartCoroutine(&DUOLClient::GameManager::StartFadeIn);
		}

		/// <summary>
		/// message Update
		/// </summary>
		/// <param name="deltaTime"></param>
		if (_currentGameMode != GameMode::UI_MODE)
		{
			while (!_floatMessages.empty())
			{
				GameMessage<float> mes = std::move(_floatMessages.front()); _floatMessages.pop();

				switch (mes._messageType)
				{
				case GameMessageType::BULLET_TIME_ALL:
				{
					std::function<DUOLGameEngine::CoroutineHandler(float)> routine
						= std::bind(&DUOLClient::GameManager::StartBulletTimeAll, this, std::placeholders::_1);

					StartCoroutine(routine, mes._parameter);

					break;
				}

				case GameMessageType::BULLET_TIME_PLAYER:
				{
					std::function<DUOLGameEngine::CoroutineHandler(float)> routine
						= std::bind(&DUOLClient::GameManager::StartBulletTimePlayer, this, std::placeholders::_1);

					StartCoroutine(routine, mes._parameter);

					break;
				}

				case GameMessageType::SCENE_CHANGE:
				{
					break;
				}
				}
			}

			while (!_tstringMessages.empty())
			{
				GameMessage<DUOLCommon::tstring> mes = std::move(_tstringMessages.front()); _tstringMessages.pop();

				switch (mes._messageType)
				{
				case GameMessageType::SCENE_CHANGE:
				{
					//EndUIMode();
					SetCanPausable(false);
					ChangeScene(mes);
					DUOLClient::UIDataManager::GetInstance()->ChangeScene();
					break;
				}
				case GameMessageType::BULLET_TIME_ALL:
				{
					break;
				}

				case GameMessageType::BULLET_TIME_PLAYER:
				{
					break;
				}
				}
			}
		}
	}

	bool GameManager::IsInBulletTimeAll() const
	{
		return _currentGameMode == GameMode::IN_BULLET_TIME_ALL;
	}

	bool GameManager::IsInBulletTimePlayer() const
	{
		return _currentGameMode == GameMode::IN_BULLET_TIME_PLAYER;
	}

	bool GameManager::IsInUIMode() const
	{
		return _currentGameMode == GameMode::UI_MODE;
	}

	void GameManager::SetIsOutInGameUIMode(bool value)
	{
		_isOutInGameUIMode = value;
	}

	void GameManager::SetCanPausable(bool value)
	{
		_canPausable = value;
	}

	void GameManager::SetBGM(DUOLGameEngine::AudioClip* audioClip)
	{
		if (_bgmAudioSource->GetAudioClip() != nullptr)
			_bgmAudioSource->Stop();

		_bgmAudioSource->SetAudioClip(audioClip);

		_bgmAudioSource->SetIsLoop(true);

		_bgmAudioSource->Play();
	}

	void GameManager::SetBGM(DUOLCommon::tstring name)
	{
		SetBGM(DUOLGameEngine::SoundManager::GetInstance()->GetAudioClip(name));
	}

	void GameManager::StopBGM()
	{
		if (_bgmAudioSource->GetAudioClip() != nullptr)
			_bgmAudioSource->Stop();
	}

	void GameManager::GameOverUIMode()
	{
		MouseUnLock();

		// GameOver Sound
		SystemManager::GetInstance()->PlayUISound(L"GameOver", true);

		_gameModePrevUIMode = _currentGameMode;

		_currentGameMode = GameMode::UI_MODE;

		_timeScalePrevUIMode = DUOLGameEngine::TimeManager::GetInstance()->GetTimeScale();

		DUOLGameEngine::TimeManager::GetInstance()->SetTimeScale(0.f);

		auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : allGameObjects)
		{
			if (gameObject->GetName() == TEXT("GameOver"))
			{
				gameObject->SetIsActiveSelf(true);

				DUOLGameEngine::InputManager::GetInstance()->SetUIMouseMode(true);
			}
			if (gameObject->GetName() == TEXT("RestartBTN"))
			{
				gameObject->SetIsActiveSelf(true);
			}
			if (gameObject->GetName() == TEXT("QuitBTN"))
			{
				gameObject->SetIsActiveSelf(true);
			}
		}

		_isOutInGameUIMode = false;

		_playerDie = true;
	}

	void GameManager::CalculatePoint(float time, int count)
	{
		if (time <= 1620)
			_totalPoint += 40;
		else if (time <= 2040)
			_totalPoint += 30;
		else if (time <= 2220)
			_totalPoint += 20;
		else
			_totalPoint += 10;

		int calculatCount = count * 10;

		if (3600 <= calculatCount)
			_totalPoint += 40;
		else if (2700 <= calculatCount)
			_totalPoint += 30;
		else if (1800 <= calculatCount)
			_totalPoint += 20;
		else if (1200 <= calculatCount)
			_totalPoint += 10;
	}

	void GameManager::convertSecondsToTime(float second, int& hours, int& minutes, int& remainingSeconds)
	{
		// 소수 부분과 정수 부분을 분리
		int totalSeconds = static_cast<int>(second);
		float fractionalPart = second - static_cast<float>(totalSeconds);

		// 초 단위를 시간, 분, 초로 변환
		hours = totalSeconds / 3600;
		minutes = (totalSeconds % 3600) / 60;
		remainingSeconds = totalSeconds % 60;

		// 소수 부분을 이용하여 초 단위 보정
		if (fractionalPart >= 0.5) {
			// 반올림
			if (remainingSeconds == 59) {
				remainingSeconds = 0;
				if (minutes == 59) {
					minutes = 0;
					++hours;
				}
				else {
					++minutes;
				}
			}
			else {
				++remainingSeconds;
			}
		}
	}

	DUOLClient::GameManager* GameManager::GetInstance()
	{
		if (_instance == nullptr)
		{
			auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

			GameManager* gameManager = nullptr;

			for (auto gameObject : allGameObjects)
			{
				auto comp = gameObject->GetComponent<GameManager>();
				if (gameManager != nullptr)
				{
					_instance = gameManager;
					return _instance;
				}
			}

			//찾지 못했다면..
			if (!_instance)
			{
				auto obj = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
				_instance = obj->AddComponent<GameManager>();
			}
		}

		return _instance;
	}
}