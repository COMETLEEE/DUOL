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
	{
		if (_instance != nullptr)
			Destroy(this);
		else
			_instance = this;
	}

	GameManager::~GameManager()
	{

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

		// ���� �Ŵ����� ������ ���۰� ������ ���� �մϴ� ..!
		DontDestroyOnLoad(static_cast<DUOLGameEngine::GameObject*>(GetGameObject()));
	}

	void GameManager::OnStart()
	{
		MonoBehaviourBase::OnStart();

		DUOL_ENGINE_INFO(DUOL_CONSOLE, "GameManager 'OnStart' function called.")

		//// Initialize Scene ����ϴ� ��� ..!
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

		StartCoroutine(&DUOLClient::GameManager::StartFadeIn);
	}

	void GameManager::OnUpdate(float deltaTime)
	{
		// UI_MODE
		if (_currentGameMode != GameMode::UI_MODE && 
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