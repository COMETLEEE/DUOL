#include "DUOLGame/Application.h"

#include "DUOLCommon//Log/LogHelper.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

// TEST SCENES
#include "DUOLGame/TestScenes/CometTestScene.h"
#include "DUOLGame/TestScenes/SHTestScene.h"
#include "DUOLGame/TestScenes/TestScene.h"
#include "DUOLGame/TestScenes/YDTestScene.h"
#include "DUOLGameEngine/Manager/UnityMigrator/UnityMigrator.h"

#include "DUOLGameEngine/ECS/GameObject.h"

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
		// Log system initialize.
		DUOLCommon::LogHelper::Initialize();

#pragma region WINDOW_INITIALIZE
		const HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandle(NULL));

		// ?????? ????? .inl?? ???? ??? ???? ???? ??? ?? ? ???, ???, ??? ???? ???????.
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

		RECT rect{ SCREEN_START_LEFT, SCREEN_START_TOP,
		SCREEN_START_LEFT + gameSpec.screenWidth, SCREEN_START_TOP + gameSpec.screenHeight };

		// 윈도우 스타일에 따른 정확한 클라이언트 영역을 구해서 rect를 변환합니다.
		::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		gameSpec.hWnd = CreateWindow(appName, appName, WS_OVERLAPPEDWINDOW,
			SCREEN_START_LEFT, SCREEN_START_TOP, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

		::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		assert(gameSpec.hWnd != 0 && "Failed To Start Game");

		ShowWindow(gameSpec.hWnd, SW_SHOWNORMAL);
		UpdateWindow(gameSpec.hWnd);
#pragma endregion

#pragma region INITIALIZE_ENGINE_AND_MODULES
		_gameEngine = std::make_shared<DUOLGameEngine::Engine>();

		_gameEngine->Initialize(gameSpec);

		// TODO : 씬 하드 코딩 .. => 목표는 .inl 파일을 이용해서 세이브 & 로드 ..!
		//const std::shared_ptr<CometTestScene> cometTestScene =
		//	std::make_shared<CometTestScene>();

		//const std::shared_ptr<YDTestScene> ydTestScene =
		//	std::make_shared<YDTestScene>();

		//const std::shared_ptr<SHTestScene> shTestScene =
		//	std::make_shared<SHTestScene>();

		const std::shared_ptr<SHTestScene> testScene =
			std::make_shared<SHTestScene>();

		//DUOLGameEngine::SceneManager::GetInstance()->AddGameScene(cometTestScene);
		//DUOLGameEngine::SceneManager::GetInstance()->AddGameScene(ydTestScene);
		//DUOLGameEngine::SceneManager::GetInstance()->AddGameScene(shTestScene);
		DUOLGameEngine::SceneManager::GetInstance()->AddGameScene(testScene);

		// 여기서 씬 변경해주세요. 나중에 로그로 확인하기 쉽습니다. 
		std::wstring nowscene = TEXT("SHTestScene");

		// DUOLGameEngine::SceneManager::GetInstance()->LoadScene(TEXT("SHTestScene"));
		DUOLGameEngine::SceneManager::GetInstance()->LoadScene(nowscene);
		//DUOLGameEngine::SceneManager::GetInstance()->LoadScene(TEXT("TestScene"));

#pragma region UNITY_SCENE
		//std::shared_ptr<DUOLGameEngine::Scene> scene = DUOLGameEngine::UnityMigrator::GetInstance()->MigrateUnitySceneFile(TEXT("Asset/Scene_Unity/CometExperiment.txt"));

		//DUOLGameEngine::SceneManager::GetInstance()->AddGameScene(scene);

		//DUOLGameEngine::SceneManager::GetInstance()->LoadScene(TEXT("CometExperiment"));
#pragma endregion

#pragma endregion

		DUOL_INFO(DUOL_FILE,"This Scene is {}", DUOLCommon::StringHelper::WStringToString(nowscene));
		DUOL_INFO(DUOL_FILE,"Application Initialize Success");
	}

	void Application::UnInitialize()
	{
#pragma region UNINITIALIZE_ENGINE_AND_MODULES
		_gameEngine->UnInitialize();

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
				if (msg.message == WM_QUIT) break;
				DispatchMessage(&msg);
			}
			else
			{
				_gameEngine->Update();

				_gameEngine->StartRenderingForGame();

				_gameEngine->EndRenderingForGame();
			}
		}
	}
}