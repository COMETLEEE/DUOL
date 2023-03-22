#include "DUOLGameEngine/Engine.h"

#include "DUOLGameEngine/Manager/BehaviorTreeFactory.h"
#include "DUOLGameEngine/Manager/EventManager.h"
#include "DUOLGameEngine/Manager/DebugManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/NavigationManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/SerializeManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"

namespace DUOLGameEngine
{
	Engine::Engine() :
		_engineSpec({})
	{

	}

	Engine::~Engine()
	{

	}

	void Engine::Initialize(const EngineSpecification& gameSpecification)
	{
		DUOLCommon::LogHelper::Initialize();

		_engineSpec = gameSpecification;

#pragma region EVENTMANAGER_INIT_AND_REGISTER
		EventManager::GetInstance()->Initialize();
#pragma endregion

		InputManager::GetInstance()->Initialize(_engineSpec.hWnd);

		TimeManager::GetInstance()->Initialize();

		GraphicsManager::GetInstance()->Initialize(_engineSpec);

		PhysicsManager::GetInstance()->Initialize();

		ResourceManager::GetInstance()->Initialize(_engineSpec
			, GraphicsManager::GetInstance()->_graphicsEngine
			, PhysicsManager::GetInstance()->_physicsSystem);

		SerializeManager::GetInstance()->Initialize(_engineSpec);

		SceneManager::GetInstance()->Initialize();

		NavigationManager::GetInstance()->Initialize();

		UIManager::GetInstance()->Initialize();

#if defined(_DEBUG) || defined(NDEBUG)
		DebugManager::GetInstance()->Initialize();
#endif
	}

	void Engine::UnInitialize()
	{
#if defined(_DEBUG) || defined(NDEBUG)
		DebugManager::GetInstance()->UnInitialize();
#endif
		UIManager::GetInstance()->UnInitialize();

		BehaviorTreeFactory::GetInstance()->UnInitialize();

		NavigationManager::GetInstance()->UnInitialize();

		SceneManager::GetInstance()->UnInitialize();

		SerializeManager::GetInstance()->UnInitialize();

		ResourceManager::GetInstance()->UnInitialize();

		PhysicsManager::GetInstance()->UnInitialize();

		GraphicsManager::GetInstance()->UnInitialize();

		TimeManager::GetInstance()->UnInitialize();

		InputManager::GetInstance()->UnInitialize();

		EventManager::GetInstance()->UnInitialize();
	}

	bool Engine::DUOLGameEngine_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_SIZE:
		{
			const uint32_t screenWidth = LOWORD(lParam);

			const uint32_t screenHeight = HIWORD(lParam);

			if (wParam == SIZE_MINIMIZED)
			{
				break;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{

			}
			else if (wParam == SIZE_RESTORED)
			{

			}

			DUOLMath::Vector2 screenSize = { static_cast<float>(screenWidth), static_cast<float>(screenHeight) };

			// 해당 이벤트는 즉시 호출합니다.
			EventManager::GetInstance()->InvokeEvent<std::any>(TEXT("Resize"), &screenSize);

			break;
		}

		case WM_QUIT:
		{

			break;
		}

		// 여러기지 있을 수 있따.

		default:
			break;
		}

		return false;
	}

	void Engine::Update()
	{
#pragma region TIME_AND_INPUT
		TimeManager::GetInstance()->Update();

		const float scaledDeltaTime = TimeManager::GetInstance()->GetDeltaTime();

		InputManager::GetInstance()->Update(scaledDeltaTime);
#pragma endregion

		//// 너무 큰 언스케일드 델타 타임의 경우에는 엔진의 업데이트를 진행하지 않습니다.
		if (TimeManager::GetInstance()->GetUnscaledDeltaTime() >= 2.f)
			return;

#pragma region PHYSICS
		PhysicsManager::GetInstance()->Update(scaledDeltaTime);
#pragma endregion

#pragma region NAVIGATION
		NavigationManager::GetInstance()->Update(scaledDeltaTime);
#pragma endregion

#pragma region GAME_LOGIC
		SceneManager::GetInstance()->Update(scaledDeltaTime);
#pragma endregion

#pragma region EVENT
		EventManager::GetInstance()->Update();
#pragma endregion

#pragma region RESOURCE
		ResourceManager::GetInstance()->Update(scaledDeltaTime);
#pragma endregion

#if defined(_DEBUG) || defined(NDEBUG)
		DebugManager::GetInstance()->Update(scaledDeltaTime);
#endif
	}

	void Engine::UpdateEditMode()
	{
#pragma region TIME_AND_INPUT
		TimeManager::GetInstance()->Update();

		const float unscaledDeltaTime = TimeManager::GetInstance()->GetDeltaTime();

		InputManager::GetInstance()->Update(unscaledDeltaTime);
#pragma endregion

#pragma region PHYSICS
		PhysicsManager::GetInstance()->UpdateEditAndPauseMode(unscaledDeltaTime);
#pragma endregion

#pragma region GAME_LOGIC
		SceneManager::GetInstance()->UpdateEditAndPauseMode(unscaledDeltaTime);
#pragma endregion

#pragma region EVENT
		// 씬 에디팅에 업데이트해야하는 녀석들을 모아 업데이트합니다.
		EventManager::GetInstance()->InvokeEvent(TEXT("SceneEditModeUpdating"));

		EventManager::GetInstance()->Update();
#pragma endregion

#pragma region RESOURCE
		ResourceManager::GetInstance()->Update(unscaledDeltaTime);
#pragma endregion

#pragma region UI
		UIManager::GetInstance()->Update(unscaledDeltaTime);
#pragma endregion

#if defined(_DEBUG) || defined(NDEBUG)
		DebugManager::GetInstance()->Update(unscaledDeltaTime);
#endif
	}

	void Engine::UpdatePauseMode()
	{
#pragma region TIME_AND_INPUT
		TimeManager::GetInstance()->Update();

		const float unscaledDeltaTime = TimeManager::GetInstance()->GetDeltaTime();

		InputManager::GetInstance()->Update(unscaledDeltaTime);
#pragma endregion

#pragma region PHYSICS
		PhysicsManager::GetInstance()->UpdateEditAndPauseMode(unscaledDeltaTime);
#pragma endregion

#pragma region GAME_LOGIC
		SceneManager::GetInstance()->UpdateEditAndPauseMode(unscaledDeltaTime);
#pragma endregion

#pragma region EVENT
		EventManager::GetInstance()->InvokeEvent(TEXT("SceneEditModeUpdating"));

		EventManager::GetInstance()->Update();
#pragma endregion

#pragma region RESOURCE
		ResourceManager::GetInstance()->Update(unscaledDeltaTime);
#pragma endregion
	}

	void Engine::UpdateFrameMode()
	{
#pragma region TIME_AND_INPUT
		TimeManager::GetInstance()->Update();

		const float unscaledDeltaTime = TimeManager::GetInstance()->GetDeltaTime();

		InputManager::GetInstance()->Update(unscaledDeltaTime);
#pragma endregion

#pragma region EVENT
		EventManager::GetInstance()->Update();
#pragma endregion

#pragma region RESOURCE
		ResourceManager::GetInstance()->Update(unscaledDeltaTime);
#pragma endregion
	}

	void Engine::StartRenderingForGame()
	{
		GraphicsManager::GetInstance()->StartRenderingForGame();
	}

	void Engine::EndRenderingForGame()
	{
		GraphicsManager::GetInstance()->EndRenderingForGame();
	}
}
