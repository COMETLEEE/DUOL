#include <cassert>

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/Manager/SerializeManager.h"

namespace DUOLGameEngine
{
	SceneManager::SceneManager()
	{
		
	}

	SceneManager::~SceneManager()
	{
		UnInitialize();
	}

	void SceneManager::ChangeScene()
	{
		_currentScene = _reservedScene;

		_reservedScene.reset();

		_isReservedChangeScene = false;

		// ���� ����Ų��.
		_currentScene->Awake();

#pragma region PHYSICS_SCENE_INIT
		PhysicsManager::GetInstance()->InitializeCurrentGameScene(_currentScene->_rootObjectsInScene);
#pragma endregion

		// ���� ��ŸƮ�Ѵ�.
		_currentScene->Start();
	}

	void SceneManager::SaveCurrentScene()
	{
		if (DUOLGameEngine::SerializeManager::GetInstance()->SerializeScene(_currentScene.get()))
		{
			DUOL_ENGINE_INFO(DUOL_CONSOLE, "Succeeded in saving the current scene.");
		}
		else
		{
			DUOL_ENGINE_CRITICAL("Failed in saving current scene.");
		}
	}

	void SceneManager::LoadSceneFile(const DUOLCommon::tstring& sceneName)
	{
		DUOLCommon::tstring filePath = TEXT("Asset/Scene/") + sceneName + TEXT(".dscene");

		std::shared_ptr<DUOLGameEngine::Scene> loadedScene = nullptr;

		if ((loadedScene = DUOLGameEngine::SerializeManager::GetInstance()->DeserializeScene(filePath)) != nullptr)
		{
			DUOL_ENGINE_INFO(DUOL_CONSOLE, "Succeeded in loading the scene.");

			auto&& iter = _scenesInGame.find(sceneName);

			// ����� �� ����Ʈ�� �ش��ϴ� �̸��� ���� �̹� �ֽ��ϴ�. �����ϰ� �ε�� ������ �ٽ� �־��ݴϴ�.
			if (iter != _scenesInGame.end())
			{
				// ������ ���� ����Ʈ�մϴ�.
				iter->second.reset();

				iter->second = loadedScene;
			}
			else
			{
				_scenesInGame.insert({ { sceneName, loadedScene} });
			}

			_reservedScene = _scenesInGame.at(sceneName);

			_isReservedChangeScene = true;
		}
		else
		{
			DUOL_ENGINE_CRITICAL("Failed in loading the scene.");
		}
	}

	void SceneManager::Initialize()
	{
		_isReservedChangeScene = false;

		DUOL_INFO(DUOL_FILE, "SceneManager Initialize Success !");
	}

	void SceneManager::UnInitialize()
	{
		for (auto& scene : _scenesInGame)
			scene.second.reset();

		_currentScene.reset();

		_reservedScene.reset();
	}

	void SceneManager::Update(float deltaTime)
	{
		// ����Ƽ �����ֱ�� ���� ������ ���� ���� ������ ������Ʈ�մϴ�.
		if (_currentScene != nullptr)
		{
			// ���� ��û�� ������Ʈ���� �����մϴ�.
			_currentScene->CreateGameObjects();

			_currentScene->FixedUpdate(deltaTime);

			_currentScene->Update(deltaTime);

			_currentScene->InvokeUpdate(deltaTime);

			_currentScene->CoroutineUpdate(deltaTime);

			_currentScene->LateUpdate(deltaTime);

			// �̹� �����ӿ� ��Ȱ��ȭ�� ���� ������Ʈ���� ��Ȱ��ȭ�մϴ�.
			_currentScene->InActiveGameObjects();

			// �̹� �����ӿ� Ȱ��ȭ�� ���� ������Ʈ���� Ȱ��ȭ�մϴ�.
			_currentScene->ActiveGameObjects();

			// �ı� ��û�� ���� ������Ʈ���� �ı��մϴ�.
			_currentScene->DestroyGameObjects();
		}

		if (_isReservedChangeScene)
		{
			// ���� ���� ������ �ʿ��ϴٸ� ..?
			// Ŭ���̾�Ʈ���� ���� �ڷᱸ���� �� ¥�� Json���� ���� Load
			ChangeScene();
		}
	}

	void SceneManager::UpdateEditAndPauseMode(float deltaTime)
	{
		if (_currentScene != nullptr)
		{
			_currentScene->CreateGameObjects();

			_currentScene->InActiveGameObjects();

			_currentScene->ActiveGameObjects();

			_currentScene->DestroyGameObjects();
		}

		if (_isReservedChangeScene)
		{
			ChangeScene();
		}
	}

	Scene* SceneManager::GetCurrentScene()
	{
		return _currentScene.get();
	}

	void SceneManager::LoadScene(const DUOLCommon::tstring& sceneName, LoadSceneMode mode)
	{
		auto&& iter = _scenesInGame.find(sceneName);

		// ����� �� ����Ʈ�� �ش��ϴ� �̸��� ���� �����ϴ� ..!
		if (iter == _scenesInGame.end())
		{
			assert(false);
		}

		_reservedScene = (*iter).second;

		_isReservedChangeScene = true;
	}

	void SceneManager::AddGameScene(const std::shared_ptr<DUOLGameEngine::Scene>& scene)
	{
		if (scene != nullptr)
		{
			_scenesInGame.insert({ scene->GetName(), scene });
		}
	}
}
