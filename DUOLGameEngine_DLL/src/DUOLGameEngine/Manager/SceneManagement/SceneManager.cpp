#include <cassert>

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include <filesystem>

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Light.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"

#include "DUOLGameEngine/Manager/EventManager.h"
#include "DUOLGameEngine/Manager/NavigationManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"
#include "DUOLGameEngine/Manager/UnityMigrator/UnityMigrator.h"

#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/Manager/SerializeManager.h"

namespace DUOLGameEngine
{
	SceneManager::SceneManager() :
		_scenesInGame({})
		, _currentScene(nullptr)
		, _reservedScene(nullptr)
		, _isReservedChangeScene(false)
		, _isCurrentSceneLoadedFromFile(false)
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

#pragma region NAVIGATION_MESH_INIT
		// ���� �ٲ������, ���� �׺���̼� �޽��� ����ϴ� ���̸� �ҷ��ɴϴ�.
		if (!_currentScene->GetNavMeshFileName().empty())
		{
			NavigationManager::GetInstance()->SetCurrentNavMesh(_currentScene->GetNavMeshFileName());

			NavigationManager::GetInstance()->InitializeCurrentGameScene(_currentScene->_rootObjectsInScene);
		}
		else
			NavigationManager::GetInstance()->SetCurrentNavMesh(TEXT(""));
#pragma endregion

#pragma region PHYSICS_SCENE_INIT
		// ���� �ٲ������ ���� ���� ���׵� �ʱ�ȭ�մϴ�.
		PhysicsManager::GetInstance()->InitializeCurrentGameScene(_currentScene->_rootObjectsInScene);
#pragma endregion

		// ���� ��ŸƮ�Ѵ�.
		_currentScene->Start();

		// �� ü���� �̺�Ʈ ��
		DUOLGameEngine::EventManager::GetInstance()->InvokeEvent(TEXT("SceneChanging"));
	}

	void SceneManager::SaveCurrentSceneTo(const DUOLCommon::tstring& filePath)
	{
		if (DUOLGameEngine::SerializeManager::GetInstance()->SerializeScene(_currentScene.get(), filePath))
		{
			DUOL_ENGINE_INFO(DUOL_CONSOLE, "Succeeded in saving the current scene.");
		}
		else
		{
			DUOL_ENGINE_CRITICAL("Failed in saving current scene.");
		}
	}

	DUOLGameEngine::Scene* SceneManager::LoadSceneFileFrom(const DUOLCommon::tstring& filePath)
	{
		std::filesystem::path p(filePath);

		DUOLCommon::tstring sceneName = p.stem();

		std::shared_ptr<DUOLGameEngine::Scene> loadedScene = nullptr;

		if ((loadedScene = DUOLGameEngine::SerializeManager::GetInstance()->DeserializeScene(filePath)) != nullptr)
		{
			DUOL_ENGINE_INFO(DUOL_CONSOLE, "Succeeded in loading the scene.");

			auto&& iter = _scenesInGame.find(sceneName);

			// �̸� ����, �׻� ���� �̸��� �� �̸��� �����ϰ� �����Դϴ�.
			loadedScene->_name = sceneName;

			// ����� �� ����Ʈ�� �ش��ϴ� �̸��� ���� �̹� �ֽ��ϴ�. �����ϰ� �ε�� ������ �ٽ� �־��ݴϴ�.
			if (iter != _scenesInGame.end())
			{
				// ������ ���� ����Ʈ�մϴ�.
				iter->second.reset();

				iter->second = loadedScene;
			}
			// �ƴ϶�� �־��ݴϴ�.
			else
			{
				// TODO : Hard Coding scene ������� .. ��� �Ǵ� �����̴�. (������ �ε��, ����ϰ� �ִ� ���� ���شٴ� ���̴ϱ� ..)
 				if ((_currentScene != nullptr) && (_scenesInGame.find(_currentScene->GetName()) != _scenesInGame.end()))
					_scenesInGame.erase(_scenesInGame.find(_currentScene->GetName()));

				_scenesInGame.insert({ { sceneName, loadedScene} });
			}

			_reservedScene = _scenesInGame.at(sceneName);

			_isReservedChangeScene = true;

			_isCurrentSceneLoadedFromFile = true;

			loadedScene->_path = filePath;

			return loadedScene.get();
		}
		else
		{
			DUOL_ENGINE_CRITICAL("Failed in loading the scene.");

			_isCurrentSceneLoadedFromFile = false;

			return nullptr;
		}
	}

	bool SceneManager::GetIsCurrentSceneLoadedFromFile() const
	{
		return _isCurrentSceneLoadedFromFile;
	}

	DUOLGameEngine::Scene* SceneManager::LoadEmptyScene()
	{
		std::shared_ptr<DUOLGameEngine::Scene> scene = std::make_shared<DUOLGameEngine::Scene>();

		// ---------------- Main Camera
		DUOLGameEngine::GameObject* mainCam = scene->CreateEmpty();

		mainCam->SetName(TEXT("Main Camera"));

		mainCam->AddComponent<DUOLGameEngine::Camera>();

		// ---------------- Directional Light
		DUOLGameEngine::GameObject* dirLight = scene->CreateEmpty();

		dirLight->SetName(TEXT("Directional Light"));

		dirLight->AddComponent<DUOLGameEngine::Light>();

		_reservedScene = scene;

		_isReservedChangeScene = true;

		_isCurrentSceneLoadedFromFile = false;

		// TODO : Hard Coding scene ������� .. ��� �Ǵ� �����̴�. (������ �ε��, ����ϰ� �ִ� ���� ���شٴ� ���̴ϱ� ..)
		if ((_currentScene != nullptr) && (_scenesInGame.find(_currentScene->GetName()) != _scenesInGame.end()))
			_scenesInGame.erase(_scenesInGame.find(_currentScene->GetName()));

		return scene.get();
	}

	DUOLGameEngine::Scene* SceneManager::LoadUnityScene(const DUOLCommon::tstring& filePath)
	{
		std::shared_ptr<DUOLGameEngine::Scene> scene =
			DUOLGameEngine::UnityMigrator::GetInstance()->MigrateUnitySceneFile(filePath);

		_reservedScene = scene;

		_isReservedChangeScene = true;

		_isCurrentSceneLoadedFromFile = false;

		if ((_currentScene != nullptr) && (_scenesInGame.find(_currentScene->GetName()) != _scenesInGame.end()))
			_scenesInGame.erase(_scenesInGame.find(_currentScene->GetName()));

		return scene.get();
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

			_currentScene->Update(deltaTime);

			_currentScene->InvokeUpdate(deltaTime);

			_currentScene->CoroutineUpdate(deltaTime);

			_currentScene->LateUpdate(deltaTime);

			// �̹� �����ӿ� ��Ȱ��ȭ�� ���� ������Ʈ���� ��Ȱ��ȭ�մϴ�.
			_currentScene->InActiveGameObjects();

			// �̹� �����ӿ� Ȱ��ȭ�� ���� ������Ʈ���� Ȱ��ȭ�մϴ�.
			_currentScene->ActiveGameObjects();

			_currentScene->DestroyComponents(deltaTime);

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

			_currentScene->DestroyComponents(deltaTime);

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
