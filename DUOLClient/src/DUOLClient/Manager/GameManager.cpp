#include "DUOLClient/Manager/GameManager.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"

#include <rttr/registration>

#include "DUOLClient/ECS/Component/Map/FadeInOut.h"
#include "DUOLClient/ECS/Component/Map/Portal.h"
#include "DUOLClient/Manager/UIDataManager.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/CameraEventManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSecondsRealtime.h"

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
	);
}

namespace DUOLClient
{
	DUOLClient::GameManager* DUOLClient::GameManager::_instance = nullptr;

	GameManager::GameManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
		, _isFirstStart(true)
		, _currentGameMode(GameMode::DEFAULT)
		, _isMainScene(false)
		, _isOutInGameUIMode(false)
		, _isCursorShowing(true)
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
	}

	void GameManager::MouseLock()
	{
		DUOLGameEngine::InputManager::GetInstance()->SetGameLockMode(true);

		if (_isCursorShowing)
		{
			ShowCursor(false);

			_isCursorShowing = false;
		}
	}

	void GameManager::MouseUnLock()
	{
		DUOLGameEngine::InputManager::GetInstance()->SetGameLockMode(false);

		if (!_isCursorShowing)
		{
			ShowCursor(true);

			_isCursorShowing = true;
		}
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

	DUOLGameEngine::CoroutineHandler GameManager::StartUIMode()
	{
		MouseUnLock();

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

		co_yield std::make_shared<DUOLGameEngine::WaitForFrames>(2);

		// UI Mode
		while (true)
		{
			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Escape) ||
				_isOutInGameUIMode)
				break;

			co_yield nullptr;
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
	}

	DUOLGameEngine::CoroutineHandler GameManager::StartFadeIn()
	{
		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(0.5f);

		if (_fadeInOut != nullptr)
		{
			_fadeInOut->StartFadeIn(SCENE_START_FADE_IN, nullptr);
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
#pragma region PORTALS
		CreatePortal(middle, TEXT("Portal_A"), TEXT("StageA"), MIDDLE_PORTAL_TO_A_POSITION);
		CreatePortal(middle, TEXT("Portal_B"), TEXT("StageB"), MIDDLE_PORTAL_TO_B_POSITION);
		CreatePortal(middle, TEXT("Portal_C"), TEXT("StageC"), MIDDLE_PORTAL_TO_C_POSITION);
#pragma endregion

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLClient::FadeInOut>();
			}
			if(gameObject->GetTag() == TEXT("Player"))
			{
				UIDataManager::GetInstance()->InitializeMiddle(gameObject);
			}
		}

		StartCoroutine(&DUOLClient::GameManager::StartFadeIn);

	}

	void GameManager::InitializeStageTotal()
	{
		std::vector<int> _sequenceCamera;
		_sequenceCamera.emplace_back(0);
		_sequenceCamera.emplace_back(2);
		_sequenceCamera.emplace_back(1);

		// Camera Action Start
		DUOLGameEngine::CameraEventManager::GetInstance()->SetSequenceList(_sequenceCamera);
		DUOLGameEngine::CameraEventManager::GetInstance()->SetSequenceMode(true);
	}

	void GameManager::InitializeStageA(DUOLGameEngine::Scene* stageA)
	{
		CreatePortal(stageA, TEXT("Portal_Middle"), TEXT("Middle"), A_PORTAL_TO_MIDDLE_POSITION);

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLClient::FadeInOut>();
			}
		}

		StartCoroutine(&DUOLClient::GameManager::StartFadeIn);

		UIDataManager::GetInstance()->InitializeStageA();
	}

	void GameManager::InitializeStageB(DUOLGameEngine::Scene* stageB)
	{
		CreatePortal(stageB, TEXT("Portal_Middle"), TEXT("Middle"), B_PORTAL_TO_MIDDLE_POSITION);

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLClient::FadeInOut>();
			}
		}

		StartCoroutine(&DUOLClient::GameManager::StartFadeIn);

		UIDataManager::GetInstance()->InitializeStageB();
	}

	void GameManager::InitializeStageC(DUOLGameEngine::Scene* stageC)
	{
		CreatePortal(stageC, TEXT("Portal_Middle"), TEXT("Middle"), C_PORTAL_TO_MIDDLE_POSITION);

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLClient::FadeInOut>();
			}
		}

		StartCoroutine(&DUOLClient::GameManager::StartFadeIn);

		UIDataManager::GetInstance()->InitializeStageC();
	}

	void GameManager::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		if (_instance != nullptr && _instance != this)
		{
			// 유일성 보장
			Destroy(this);

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

		if (DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetName() == TEXT("Main"))
		{
			_isMainScene = true;

			MouseUnLock();
		}
		else
		{
			_isMainScene = false;

			MouseLock();
		}

		DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		const DUOLCommon::tstring& currentSceneName = currentScene->GetName();

		if (currentSceneName == TEXT("Middle"))
			InitializeMiddle(currentScene);
		else if (currentSceneName == TEXT("TotalScene"))
			InitializeStageTotal();
		else if (currentSceneName == TEXT("BattleTest"))
			InitializeStageA(currentScene);
		else if (currentSceneName == TEXT("StageB"))
			InitializeStageB(currentScene);
		else if (currentSceneName == TEXT("StageC"))
			InitializeStageC(currentScene);

		// 어차피 시작은 무조건 디폴트다.
		_currentGameMode = GameMode::DEFAULT;
	}

	void GameManager::OnUpdate(float deltaTime)
	{

		if (DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetName() == TEXT("TotalScene"))
		{
			if(DUOLGameEngine::CameraEventManager::GetInstance()->IsSequencePlay())
				auto scene = DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(L"Stage3Test0615");
		}
		// UI_MODE
		if (_currentGameMode != GameMode::UI_MODE && !_isMainScene &&
			DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Escape))
		{
			// TODO : UI Mode ..?
			std::function<DUOLGameEngine::CoroutineHandler()> routine  = std::bind(&DUOLClient::GameManager::StartUIMode, this);

			StartCoroutine(routine);
		}
		if(_fadeInOut ==nullptr && _isMainScene==false)
		{
			auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

			for (auto gameObject : gameObjects)
			{
				if (gameObject->GetTag() == TEXT("Fade"))
				{
					_fadeInOut = gameObject->GetComponent<DUOLClient::FadeInOut>();
				}
			}

			StartCoroutine(&DUOLClient::GameManager::StartFadeIn);

		}

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

	DUOLClient::GameManager* GameManager::GetInstance()
	{
		// TODO : 의도된것이나 UI작업으로 작성함
		if (_instance == nullptr)
			_instance = new GameManager();

		return _instance;
	}
}