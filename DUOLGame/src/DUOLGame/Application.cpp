#include "DUOLGame/Application.h"

#include "DUOLCommon//Log/LogHelper.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

extern DUOLGame::Application g_App;

namespace DUOLGame
{
	constexpr uint32_t SCREEN_START_LEFT = 100;

	constexpr uint32_t SCREEN_START_TOP = 100;

	constexpr uint32_t SCREEN_WIDTH = 1600;

	constexpr uint32_t SCREEN_HEIGHT = 1080;

	LRESULT CALLBACK ApplicationWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (g_App._gameEngine != nullptr)
		{
			if (g_App.ProcWrapper(hWnd, message, wParam, lParam))
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

		DUOLGameEngine::EngineSpecification gameSpec;

		gameSpec._screenWidth = SCREEN_WIDTH;

		gameSpec._screenHeight = SCREEN_HEIGHT;

		gameSpec._startSceneName = DUOLCommon::StringHelper::ToTString("Load");

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

		RECT rect{ SCREEN_START_LEFT, SCREEN_START_TOP,
		SCREEN_START_LEFT + gameSpec._screenWidth, SCREEN_START_TOP + gameSpec._screenHeight };

		// 윈도우 스타일에 따른 정확한 클라이언트 영역을 구해서 rect를 변환합니다.
		::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		gameSpec.hWnd = CreateWindow(appName, appName, WS_OVERLAPPEDWINDOW,
			SCREEN_START_LEFT, SCREEN_START_TOP, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

		assert(gameSpec.hWnd != 0 && "Failed To Start Game");

		ShowWindow(gameSpec.hWnd, SW_SHOWNORMAL);

		UpdateWindow(gameSpec.hWnd);
#pragma endregion

		_gameEngine = std::make_shared<DUOLGameEngine::Engine>();

		_gameEngine->Initialize(gameSpec);

		_duolClient = LoadLibrary(TEXT("DUOLClient.dll"));

		// Log system initialize.
		DUOLCommon::LogHelper::Initialize();

		// TODO : Start scene load.
		auto scene = DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(TEXT("Asset/Scene/Main.dscene"));

		DUOL_INFO(DUOL_FILE, "Application Initialize Success");
	}

	void Application::UnInitialize()
	{
#pragma region UNINITIALIZE_ENGINE_AND_MODULES
		_gameEngine->UnInitialize();

		FreeLibrary(_duolClient);

		DUOLCommon::LogHelper::UnInitialize();
#pragma endregion
	}

	bool Application::ProcWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return _gameEngine->DUOLGameEngine_WndProcHandler(hWnd, message, wParam, lParam);
	}

	void Application::Run() const
	{
		MSG msg;

		while (TRUE)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_QUIT)
				{
					break;
				}
			}
			else
			{
				_gameEngine->UpdateGame();
			}
		}
	}
}