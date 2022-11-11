#include "DUOLGameEngine/Engine.h"

#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

namespace DUOLGameEngine
{
	Engine::Engine()
	{
		
	}

	Engine::~Engine()
	{

	}

	void Engine::Initialize(const EngineSpecification& gameSpecification)
	{
		_engineSpec = gameSpecification;

		InputManager::GetInstance()->Initialize(_engineSpec.hWnd);

		TimeManager::GetInstance()->Initialize();

		GraphicsManager::GetInstance()->Initialize(_engineSpec);

		ResourceManager::GetInstance()->Initialize(_engineSpec, 
			GraphicsManager::GetInstance()->_graphicsEngine);

		SceneManager::GetInstance()->Initialize();
	}

	void Engine::UnInitialize()
	{
		SceneManager::GetInstance()->UnInitialize();

		ResourceManager::GetInstance()->UnInitialize();

		GraphicsManager::GetInstance()->UnInitialize();

		TimeManager::GetInstance()->UnInitialize();

		InputManager::GetInstance()->UnInitialize();
	}

	void Engine::OnResize(const uint32_t& screenWidth, const uint32_t& screenHeight)
	{
		// 0. Engine Spec 수정
		_engineSpec.screenWidth = screenWidth;
		_engineSpec.screenHeight = screenHeight;

		// 1. GraphicsManager의 OnResize 등 등록되어 있는 Event Handlers invoke !
		_onResizeEvent.Invoke(screenWidth, screenHeight);
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

				OnResize(screenWidth, screenHeight);

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
		TimeManager::GetInstance()->Update();

		const float unscaledDeltaTime = TimeManager::GetInstance()->GetDeltaTime();

		InputManager::GetInstance()->Update(unscaledDeltaTime);

		SceneManager::GetInstance()->Update(unscaledDeltaTime);

		ResourceManager::GetInstance()->Update(unscaledDeltaTime);

		GraphicsManager::GetInstance()->Update(unscaledDeltaTime);
	}
}