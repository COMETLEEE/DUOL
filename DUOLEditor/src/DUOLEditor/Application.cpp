#include "DUOLEditor/Application.h"

#include <cassert>

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLEditor/TestScenes/CometTestScene.h"
#include "DUOLEditor/UI/GUIManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

// Forward declare message handler from imgui_impl_win32.cpp => <window.h> �������� ���ֱ� ���ؼ� �̷��� ����մϴ�. 
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern DUOLEditor::Application g_App;

namespace DUOLEditor
{
	constexpr uint32_t SCREEN_START_LEFT = 100;

	constexpr uint32_t SCREEN_START_TOP = 100;

	constexpr uint32_t SCREEN_WIDTH = 1600;

	constexpr uint32_t SCREEN_HEIGHT = 1080;

	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (::ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
			return true;

		if (g_App._gameEngine != nullptr)
		{
			if (g_App.ProcWrapper(hWnd, message, wParam, lParam))
				return true;
		}

		switch (message)
		{
		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED)
			{

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

		// ������ ��Ʈ�� .inl�� ���� �ʱ� ���� ���� �Ľ� �� â ũ��, ���, �̸� ���� �����Ѵ�.
		DUOLGameEngine::EngineSpecification engineSpec;

		engineSpec.screenWidth = SCREEN_WIDTH;

		// engineSpec.screenWidth = GetSystemMetrics(SM_CXSCREEN);

		engineSpec.screenHeight = SCREEN_HEIGHT;

		// engineSpec.screenHeight = GetSystemMetrics(SM_CYSCREEN);

		engineSpec.startSceneName = DUOLCommon::StringHelper::ToTString("Editor");

		// engineSpec.projectPath = TEXT("");

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

		//RECT rect{ 100, 100,
		//	100 + engineSpec.screenWidth, 100 + engineSpec.screenHeight };

		RECT rect{ 100, 100,
			100 + engineSpec.screenWidth, 100 + engineSpec.screenHeight };

		// ������ ��Ÿ�Ͽ� ���� ��Ȯ�� Ŭ���̾�Ʈ ������ ���ؼ� rect�� ��ȯ�մϴ�.
		::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		engineSpec.hWnd = CreateWindow(appName, appName, WS_OVERLAPPEDWINDOW,
			rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

		assert(engineSpec.hWnd != nullptr && "Failed To Start Game");

		ShowWindow(engineSpec.hWnd, SW_NORMAL);

		UpdateWindow(engineSpec.hWnd);
#pragma endregion

#pragma region ENGINE_INITIALIZE
		_gameEngine = std::make_shared<DUOLGameEngine::Engine>();

		_gameEngine->Initialize(engineSpec);

		// Log system initialize.
		DUOLCommon::LogHelper::Initialize();
#pragma endregion

#pragma region CLIENT_LOAD_LIBRARY
		// ��������� DLL �ε带 ȣ�����ش� ..!
		_duolClient = LoadLibrary(TEXT("DUOLClient.dll"));
#pragma endregion

#pragma region LOAD_��ü����_SCENE_SERIALIZED + PROTOTYPING
		// TODO : Start scene load. (batch file)

		auto scene = DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(TEXT("Asset/Scene/BattleTest.dscene"));
		// auto scene = DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(TEXT("Asset/Scene/StartScene.dscene"));
#pragma endregion

#pragma region EDITOR_UI_INITIALIZE
		_editor = std::make_shared<DUOLEditor::Editor>();

		GUIManager::GetInstance()->Initialize(engineSpec.hWnd);

		_editor->Initialize(_gameEngine.get());
#pragma endregion
	}

	bool Application::ProcWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return _gameEngine->DUOLGameEngine_WndProcHandler(hWnd, message, wParam, lParam);
	}

	void Application::Run()
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
				// ������ �� �г� ������Ʈ ���� ������Ʈ�մϴ�.
				_editor->PostUpdate(DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime());

				// �����Ϳ� ���� ImGui Draw �����͸� �����ϰ� �׸� �� ����۸� �����մϴ�.
				// PrePresent, Present ... ���ԵǾ� �ֽ��ϴ�.
				_editor->LateUpdate(DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime());

				// ����Ǿ����ϴ�.
				if (!_editor->_isRunning)
					break;
			}
		}
	}

	void Application::UnInitialize()
	{
		_gameEngine->UnInitialize();

		DUOLCommon::LogHelper::UnInitialize();

		_editor.reset();

		_gameEngine.reset();

		FreeLibrary(_duolClient);

		_duolClient = nullptr;
	}
}