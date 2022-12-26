#include "DUOLEditor/Application.h"

#include <cassert>

#include "DUOLCommon/LogHelper.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLCommon/ImGui/imgui_impl_win32.h"
#include "DUOLCommon/ImGui/imgui_impl_dx11.h"

#include "DUOLEditor/UI/GUIManager.h"

// Forward declare message handler from imgui_impl_win32.cpp => <window.h> 의존성을 없애기 위해서 이렇게 사용합니다. 
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern void CleanupRenderTarget();

extern void CreateRenderTarget();

extern DUOLEditor::Application g_App;

namespace DUOLEditor
{
	constexpr uint32_t SCREEN_WIDTH = 1600;

	constexpr uint32_t SCREEN_HEIGHT = 1080;

	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (::ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
			return true;

		if (g_App._gameEngine != nullptr)
		{
			if (DUOLGameEngine::Engine::GetInstance()->DUOLGameEngine_WndProcHandler(hWnd, message, wParam, lParam))
				return true;
		}

		switch (message)
		{
			case WM_SIZE:
			{
				if (wParam != SIZE_MINIMIZED)
				{
					if (DUOLEditor::GUIManager::GetInstance()->isDeviceOpen == true)
					{
						DUOLEditor::GUIManager::GetInstance()->OnResize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
					}
				}

				break;
			}

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
		DUOLGameEngine::EngineSpecification engineSpec;

		engineSpec.screenWidth = SCREEN_WIDTH;

		engineSpec.screenHeight = SCREEN_HEIGHT;

		engineSpec.startSceneName = DUOLCommon::StringHelper::ToTString("Editor");

		engineSpec.editorModeOption = &_editorModeOption;

		const DUOLCommon::tstring gameTitle = DUOLCommon::StringHelper::ToTString("DUOL EDITOR");

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

		engineSpec.hWnd = CreateWindow(appName, appName, WS_OVERLAPPEDWINDOW,
			100, 100, engineSpec.screenWidth, engineSpec.screenHeight, NULL, NULL, hInstance, NULL);

		assert(engineSpec.hWnd != 0 && "Failed To Start Game");

		ShowWindow(engineSpec.hWnd, SW_SHOWNORMAL);

		UpdateWindow(engineSpec.hWnd);
#pragma endregion

#pragma region ENGINE_INITIALIZE
		_gameEngine = DUOLGameEngine::Engine::GetInstance();

		_gameEngine->Initialize(engineSpec);

		// Log system initialize.
		DUOLCommon::LogHelper::Initialize();
#pragma endregion

#pragma region EDITOR_UI_INITIALIZE
		_editor = std::make_shared<DUOLEditor::Editor>();

		_editor->Initialize(_gameEngine.get(), &_editorModeOption);

		GUIManager::GetInstance()->Initialize(engineSpec.hWnd);
#pragma endregion
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

				if (msg.message == WM_QUIT) break;
			}
			else
			{
				// Editor의 현재 모드를 참조해 업데이트합니다.
				_gameEngine->Update();

				// Editor and GUI Update.
				_editor->Update(DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime());
			}
		}
	}

	void Application::UnInitialize()
	{
		_editor.reset();

		_gameEngine.reset();
	}
}