#include <cassert>

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

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

		_currentScene->Awake();

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
			_currentScene->FixedUpdate(deltaTime);

			_currentScene->Update(deltaTime);

			_currentScene->CoroutineUpdate(deltaTime);

			_currentScene->LateUpdate(deltaTime);

			_currentScene->InActiveGameObjects();

			_currentScene->ActiveGameObjects();

			_currentScene->DestroyGameObjects();
		}

		// �ڷ�ƾ�� ����ؼ� ������ �������� ..?
		if (_isReservedChangeScene)
		{
			// ���� ���� ������ �ʿ��ϴٸ� ..?
			// Ŭ���̾�Ʈ���� ���� �ڷᱸ���� �� ¥�� Json���� ���� Load
			ChangeScene();
		}
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