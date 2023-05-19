#include "DUOLClient/Manager/GameManager.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>

#include "DUOLClient/ECS/Component/Map/FadeInOut.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
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
			_fadeInOut->StartFadeOut(SCENE_END_FADE_OUT, [sceneName = message._parameter]()
				{
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
			}
		}

		co_yield std::make_shared<DUOLGameEngine::WaitForFrames>(2);

		// UI Mode
		while (true)
		{
			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Escape))
				break;

			co_yield nullptr;
		}

		if (pauseUI != nullptr)
			pauseUI->SetIsActiveSelf(false);

		_currentGameMode = _gameModePrevUIMode;
		
		DUOLGameEngine::TimeManager::GetInstance()->SetTimeScale(_timeScalePrevUIMode);

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

		//// Initialize Scene 사용하는 경우 ..!
		//if (_isFirstStart)
		//{
		//	DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(TEXT("Asset/Scene/Main.dscene"));
		//	_isFirstStart = false;
		//}

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLClient::FadeInOut>();
			}
		}

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

		StartCoroutine(&DUOLClient::GameManager::StartFadeIn);
	}

	void GameManager::OnUpdate(float deltaTime)
	{
		// UI_MODE
		if (_currentGameMode != GameMode::UI_MODE && !_isMainScene &&
			DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Escape))
		{
			// TODO : UI Mode ..?
			std::function<DUOLGameEngine::CoroutineHandler()> routine  = std::bind(&DUOLClient::GameManager::StartUIMode, this);

			StartCoroutine(routine);
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

	DUOLClient::GameManager* GameManager::GetInstance()
	{
		return _instance;
	}
}