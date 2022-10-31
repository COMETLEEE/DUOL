#include "DUOLGame/Application.h"

#include "DUOLCommon/LogHelper.h"

namespace DUOLGame
{
	constexpr uint32_t SCREEN_WIDTH = 1280;

	constexpr uint32_t SCREEN_HEIGHT = 720;

	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
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
		wndClass.lpfnWndProc = MainWndProc;
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

#pragma region INITIALIZE_ENGINE_AND_MODULES
		_gameEngine = std::make_unique<DUOLGameEngine::Engine>(gameSpec);

		_gameEngine->Initialize();

		DUOLCommon::LogHelper::Initialize();
#pragma endregion
	}

	void Application::UnInitialize()
	{
#pragma region UNINITIALIZE_ENGINE_AND_MODULES
		_gameEngine->UnInitialize();

		_gameEngine.reset();

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