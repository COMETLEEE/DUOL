#include "DUOLEditor/Application.h"

#include <cassert>

namespace DUOLEditor
{
	constexpr uint32_t SCREEN_WIDTH = 1600;

	constexpr uint32_t SCREEN_HEIGHT = 1080;

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
#pragma endregion

#pragma region ENGINE_INITIALIZE
		_gameEngine = std::make_shared<DUOLGameEngine::Engine>(gameSpec);

		_editor = std::make_shared<DUOLEditor::Editor>(_gameEngine);

#pragma endregion
	}

	void Application::Run() const
	{

	}

	void Application::UnInitialize()
	{
	}
}
