#include "DUOLGameEngine/Engine.h"

#include "DUOLGameEngine/Manager/EventManager.h"
#include "DUOLGameEngine/Manager/DebugManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

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

		SceneManager::GetInstance()->Initialize();

#if defined(_DEBUG)
		DebugManager::GetInstance()->Initialize();
#endif
	}

	void Engine::UnInitialize()
	{
#if defined(_DEBUG)
		DebugManager::GetInstance()->UnInitialize();
#endif

		SceneManager::GetInstance()->UnInitialize();

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
					
				}
				else if(wParam == SIZE_MAXIMIZED)
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

		const float unscaledDeltaTime = TimeManager::GetInstance()->GetDeltaTime();

		InputManager::GetInstance()->Update(unscaledDeltaTime);
#pragma endregion

#pragma region PHYSICS
		PhysicsManager::GetInstance()->Update(unscaledDeltaTime);
#pragma endregion

#pragma region GAME_LOGIC
		SceneManager::GetInstance()->Update(unscaledDeltaTime);
#pragma endregion

#pragma region EVENT
		EventManager::GetInstance()->Update();
#pragma endregion

#pragma region RESOURCE
		ResourceManager::GetInstance()->Update(unscaledDeltaTime);
#pragma endregion

#if defined(_DEBUG)
		DebugManager::GetInstance()->Update(unscaledDeltaTime);
#endif
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