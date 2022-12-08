#include <cassert>

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include "DUOLGameEngine/Manager/PhysicsManager.h"

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
		PhysicsManager::GetInstance()->InitializeCurrentGameScene(_currentScene->_gameObjectsInScene);
#pragma endregion

		// ���� ��ŸƮ�Ѵ�.
		_currentScene->Start();
	}

	void SceneManager::Initialize()
	{
		_isReservedChangeScene = false;
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

			// �ı� ��û�� ������Ʈ���� �ı��մϴ�.
			_currentScene->DestroyGameObjects();
		}

		if (_isReservedChangeScene)
		{
			// ���� ���� ������ �ʿ��ϴٸ� ..?
			// Ŭ���̾�Ʈ���� ���� �ڷᱸ���� �� ¥�� Json���� ���� Load
			ChangeScene();
		}
	}

	const std::shared_ptr<DUOLGameEngine::Scene>& SceneManager::GetCurrentScene()
	{
		return _currentScene;
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

	void SceneManager::AddGameScene(const std::shared_ptr<DUOLGameEngine::Scene> scene)
	{
		if (scene != nullptr)
		{
			_scenesInGame.insert({ scene->GetName(), scene });
		}
	}
}