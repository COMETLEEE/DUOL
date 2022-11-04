#include "DUOLGameEngine/Engine.h"

#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

namespace DUOLGameEngine
{
	Engine::Engine(const EngineSpecification& gameSpecification)
	{
		_engineSpec = gameSpecification;
	}

	Engine::~Engine()
	{

	}

	void Engine::Initialize()
	{
		InputManager::GetInstance()->Initialize(_engineSpec.hWnd);

		TimeManager::GetInstance()->Initialize();

		SceneManager::GetInstance()->Initialize();
	}

	void Engine::UnInitialize()
	{
		SceneManager::GetInstance()->UnInitialize();

		TimeManager::GetInstance()->UnInitialize();

		InputManager::GetInstance()->UnInitialize();
	}

	void Engine::Update()
	{
		TimeManager::GetInstance()->Update();

		const float unscaledDeltaTime = TimeManager::GetInstance()->GetDeltaTime();

		InputManager::GetInstance()->Update(unscaledDeltaTime);

		SceneManager::GetInstance()->Update(unscaledDeltaTime);
	}
}
