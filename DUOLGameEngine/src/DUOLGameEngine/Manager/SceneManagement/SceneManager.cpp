#include <cassert>

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLGameEngine
{
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
		if (_currentScene == nullptr)
			return;

		_currentScene->FixedUpdate(deltaTime);

		_currentScene->Update(deltaTime);

		_currentScene->LateUpdate(deltaTime);

		_currentScene->InActiveGameObjects();

		_currentScene->ActiveGameObjects();

		_currentScene->DestroyGameObjects();

		// 코루틴을 사용해서 문맥을 나눠보기 ..?
		if (_isReservedChangeScene)
		{
			// 이전 씬의 정보가 필요하다면 ..?
			// 클라이언트에서 내부 자료구조를 잘 짜서 Json으로 상태 Load
			ChangeScene();
		}
	}

	void SceneManager::LoadScene(const DUOLCommon::tstring& sceneName, LoadSceneMode mode)
	{
		auto&& iter = _scenesInGame.find(sceneName);

		// 등록한 씬 리스트에 해당하는 이름의 씬이 없습니다 ..!
		if (iter == _scenesInGame.end())
		{
			assert(false);
		}

		_reservedScene = (*iter).second;
	}
}
