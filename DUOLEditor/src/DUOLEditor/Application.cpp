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

#pragma region LOAD_SCENE_HARD_CODING
		//// TODO - ProjectSettings => .inl ������ ���� �ʱ�ȭ �ʿ��� �ʱ� �� ���� ??
		//const std::shared_ptr<DUOLEditor::CometTestScene> cometTestScene =
		//	std::make_shared<DUOLEditor::CometTestScene>();

		//DUOLGameEngine::SceneManager::GetInstance()->AddGameScene(cometTestScene);

		//DUOLGameEngine::SceneManager::GetInstance()->LoadScene(TEXT("CometTestScene"));

		//// TODO - ���� �ϵ� �ڵ��̶� ������ ���� Load�ϱ� ���ؼ� Update�� �� �� �ǽ����ݴϴ�.
		//_gameEngine->Update();
#pragma endregion

#pragma region LOAD_UNITY_SCENE + SERIALIZE_OUR_FORMAT
		//std::shared_ptr<DUOLGameEngine::Scene> scene = DUOLGameEngine::UnityMigrator::GetInstance()->MigrateUnitySceneFile(TEXT("Asset/Scene_Unity/CometExperiment.txt"));

		//DUOLGameEngine::SceneManager::GetInstance()->AddGameScene(scene);

		//DUOLGameEngine::SceneManager::GetInstance()->LoadScene(TEXT("CometExperiment"));

		////// TODO - ���� �ϵ� �ڵ��̶� ������ ���� Load�ϱ� ���ؼ� Update�� �� �� �ǽ����ݴϴ�.
		//_gameEngine->Update();

		//// TODO : Unity���� ���� Scene�� �ø���������մϴ�. �׽�Ʈ �ڵ��Դϴ�.
		//DUOLGameEngine::SerializeManager::GetInstance()->SerializeScene(scene.get());
#pragma endregion

#pragma region LOAD_��ü����_SCENE_SERIALIZED + PROTOTYPING
		// �ʱ� �� ���
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

		// TODO - ���� �ϵ� �ڵ��̶� ������ ���� Load�ϱ� ���ؼ� Update�� �� �� �ǽ����ݴϴ�.
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
				// ������ �� �г� ������Ʈ ���� ������Ʈ�մϴ�.
				_editor->PostUpdate(DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime());

				// �����Ϳ� ���� ImGui Draw �����͸� �����ϰ� �׸� �� ����۸� �����մϴ�.
				// PrePresent, Present ... ���ԵǾ� �ֽ��ϴ�.
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