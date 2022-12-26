#include "DUOLGame/Application.h"

#include "DUOLCommon/LogHelper.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

// TEST SCENES
#include "DUOLGame/TestScenes/CometTestScene.h"
#include "DUOLGame/TestScenes/YDTestScene.h"

extern DUOLGame::Application g_App;

namespace DUOLGame
{
	constexpr uint32_t SCREEN_WIDTH = 1280;

	constexpr uint32_t SCREEN_HEIGHT = 720;

	LRESULT CALLBACK ApplicationWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (g_App._gameEngine != nullptr)
		{
			if (DUOLGameEngine::Engine::GetInstance()->DUOLGameEngine_WndProcHandler(hWnd, message, wParam, lParam))
				return true;
		}

		switch (message)
		{
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				break;
			}

			case WM_QUIT:
			{

				break;
			}

			default:
				return DefWindowProc(hWnd,
					message, wParam, lParam);
		}

		return 0;
	}

	void Application::Initialize()
	{
#pragma region WINDOW_INITIALIZE
		const HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandle(NULL));

		// 정해진 루트의 .inl과 같은 초기 설정 파일 파싱 후 창 크기, 모드, 이름 등을 설정한다.
		DUOLGameEngine::EngineSpecification gameSpec;

		gameSpec.screenWidth = SCREEN_WIDTH;

		gameSpec.screenHeight = SCREEN_HEIGHT;

		gameSpec.startSceneName = DUOLCommon::StringHelper::ToTString("Load");

		const DUOLCommon::tstring gameTitle = DUOLCommon::StringHelper::ToTString("DUOL GAME");

		const TCHAR* appName = gameTitle.c_str();

		WNDCLASS wndClass;

		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = ApplicationWndProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = hInstance;
		wndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = appName;

		RegisterClass(&wndClass);

		gameSpec.hWnd = CreateWindow(appName, appName, WS_OVERLAPPEDWINDOW,
			100, 100, gameSpec.screenWidth, gameSpec.screenHeight, NULL, NULL, hInstance, NULL);

		assert(gameSpec.hWnd != 0 && "Failed To Start Game");

		ShowWindow(gameSpec.hWnd, SW_SHOWNORMAL);

		UpdateWindow(gameSpec.hWnd);
#pragma endregion

#pragma region INITIALIZE_ENGINE_AND_MODULES
		_gameEngine = DUOLGameEngine::Engine::GetInstance();

		_gameEngine->Initialize(gameSpec);

		// Log system initialize.
		DUOLCommon::LogHelper::Initialize();

		// TODO : .inl 파일 등 설정 파일을 파싱하여 시작 정보를 얻자.
		const std::shared_ptr<CometTestScene> cometTestScene =
			std::make_shared<CometTestScene>();

		const std::shared_ptr<YDTestScene> ydTestScene =
			std::make_shared<YDTestScene>();

		DUOLGameEngine::SceneManager::GetInstance()->AddGameScene(cometTestScene);
		DUOLGameEngine::SceneManager::GetInstance()->AddGameScene(ydTestScene);

		//DUOLGameEngine::SceneManager::GetInstance()->LoadScene(TEXT("CometTestScene"));
		 DUOLGameEngine::SceneManager::GetInstance()->LoadScene(TEXT("YDTestScene"));
#pragma endregion
	}

	void Application::UnInitialize()
	{
#pragma region UNINITIALIZE_ENGINE_AND_MODULES
		_gameEngine->UnInitialize();

		DUOLCommon::LogHelper::UnInitialize();
#pragma endregion
	}

	void Application::Run() const
	{
		MSG msg;

		while (TRUE)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;
				DispatchMessage(&msg);
			}
			else
				_gameEngine->Update();
		}
	}
}
