#include "DUOLEditor/Application.h"

#include <cassert>

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"


#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLCommon/ImGui/imgui_impl_win32.h"
#include "DUOLCommon/ImGui/imgui_impl_dx11.h"

#include "DUOLEditor/UI/GUIManager.h"

#include "DUOLEditor/TestScenes/CometTestScene.h"
#include "DUOLGameEngine/Manager/SerializeManager.h"
#include "DUOLGameEngine/Manager/UnityMigrator/UnityMigrator.h"


// Prototyping
#include "DUOLEditor/TestScripts/PlayerController.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"

// Forward declare message handler from imgui_impl_win32.cpp => <window.h> 의존성을 없애기 위해서 이렇게 사용합니다. 
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

		// 정해진 루트의 .inl과 같은 초기 설정 파일 파싱 후 창 크기, 모드, 이름 등을 설정한다.
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

		// 윈도우 스타일에 따른 정확한 클라이언트 영역을 구해서 rect를 변환합니다.
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

#pragma region LOAD_SCENE_HARD_CODING
		//// TODO - ProjectSettings => .inl 파일을 통한 초기화 필요한 초기 씬 정보 ??
		//const std::shared_ptr<DUOLEditor::CometTestScene> cometTestScene =
		//	std::make_shared<DUOLEditor::CometTestScene>();

		//DUOLGameEngine::SceneManager::GetInstance()->AddGameScene(cometTestScene);

		//DUOLGameEngine::SceneManager::GetInstance()->LoadScene(TEXT("CometTestScene"));

		//// TODO - 아직 하드 코딩이라 실제로 씬을 Load하기 위해서 Update를 한 번 실시해줍니다.
		//_gameEngine->Update();
#pragma endregion

#pragma region LOAD_UNITY_SCENE + SERIALIZE_OUR_FORMAT
		//std::shared_ptr<DUOLGameEngine::Scene> scene = DUOLGameEngine::UnityMigrator::GetInstance()->MigrateUnitySceneFile(TEXT("Asset/Scene_Unity/CometExperiment.txt"));

		//DUOLGameEngine::SceneManager::GetInstance()->AddGameScene(scene);

		//DUOLGameEngine::SceneManager::GetInstance()->LoadScene(TEXT("CometExperiment"));

		////// TODO - 아직 하드 코딩이라 실제로 씬을 Load하기 위해서 Update를 한 번 실시해줍니다.
		//_gameEngine->Update();

		//// TODO : Unity에서 받은 Scene을 시리얼라이즈합니다. 테스트 코드입니다.
		//DUOLGameEngine::SerializeManager::GetInstance()->SerializeScene(scene.get());
#pragma endregion

#pragma region LOAD_자체포맷_SCENE_SERIALIZED + PROTOTYPING
		// 초기 씬 고고
		auto scene = DUOLGameEngine::SerializeManager::GetInstance()->
			DeserializeScene(TEXT("Asset/Scene/CometExperiment.dscene"));

		DUOLGameEngine::SceneManager::GetInstance()->AddGameScene(scene);

		DUOLGameEngine::GameObject* player = scene->CreateFromFBXModel(TEXT("Standard Idle"));

		player->SetName(TEXT("PlayerCharacter"));

		player->GetTransform()->SetPosition(DUOLMath::Vector3(10.f, 10.f, 3.f), DUOLGameEngine::Space::World);

		player->GetComponent<DUOLGameEngine::Animator>()->
			SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(TEXT("ProtoAnimCon")));

		DUOLGameEngine::CapsuleCollider* capsule = player->AddComponent<DUOLGameEngine::CapsuleCollider>();

		capsule->SetCenter(DUOLMath::Vector3(0.f, 0.8f, 0.f));

		capsule->SetRadius(0.3f);

		capsule->SetHeight(1.15f);

		capsule->SetDirection(DUOLGameEngine::CapsuleDirection::Y);

		DUOLGameEngine::Rigidbody* rigid = player->AddComponent<DUOLGameEngine::Rigidbody>();

		player->AddComponent<DUOLEditor::PlayerController>();


		

































		DUOLGameEngine::SceneManager::GetInstance()->LoadScene(TEXT("CometExperiment"));

		// TODO - 아직 하드 코딩이라 실제로 씬을 Load하기 위해서 Update를 한 번 실시해줍니다.
		_gameEngine->Update();
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
				// 에디터 각 패널 스테이트 별로 업데이트합니다.
				_editor->PostUpdate(DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime());

				// 에디터에 사용될 ImGui Draw 데이터를 생성하고 그린 후 백버퍼를 스왑합니다.
				// PrePresent, Present ... 포함되어 있습니다.
				_editor->LateUpdate(DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime());
			}
		}
	}

	void Application::UnInitialize()
	{
		_gameEngine->UnInitialize();

		DUOLCommon::LogHelper::UnInitialize();

		_editor.reset();

		_gameEngine.reset();
	}
}