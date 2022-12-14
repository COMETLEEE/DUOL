#include "DUOLEditor/Application.h"

#include <cassert>

#include "DUOLCommon/LogHelper.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLCommon/ImGui/imgui_impl_win32.h"
#include "DUOLCommon/ImGui/imgui_impl_dx11.h"

// Forward declare message handler from imgui_impl_win32.cpp => <window.h> 의존성을 없애기 위해서 이렇게 사용합니다. 
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma region IMGUI_TEST
bool CreateDeviceD3D(HWND hWnd);

void CleanupDeviceD3D();

void CreateRenderTarget();

void CleanupRenderTarget();

static ID3D11Device* _device = nullptr;

static ID3D11DeviceContext* _deviceContext = nullptr;

static IDXGISwapChain* _swapChain = nullptr;

static ID3D11RenderTargetView* _mainRenderTargetView = nullptr;

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;

	_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	_device->CreateRenderTargetView(pBackBuffer, nullptr, &_mainRenderTargetView);

	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (_mainRenderTargetView) { _mainRenderTargetView->Release(); _mainRenderTargetView = nullptr; }
}

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;

	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

	D3D_FEATURE_LEVEL featureLevel;

	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION,
		&sd, &_swapChain, &_device, &featureLevel, &_deviceContext) != S_OK)
		return false;

	CreateRenderTarget();

	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();

	if (_swapChain) { _swapChain->Release(); _swapChain = nullptr; }

	if (_deviceContext) { _deviceContext->Release(); _deviceContext = nullptr; }

	if (_device) { _device->Release(); _device = nullptr; }
}

#pragma endregion

namespace DUOLEditor
{
	constexpr uint32_t SCREEN_WIDTH = 1600;

	constexpr uint32_t SCREEN_HEIGHT = 1080;

	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (::ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
			return true;

		switch (message)
		{
			case WM_SIZE:
			{
				if (_device != nullptr && wParam != SIZE_MINIMIZED)
				{
					CleanupRenderTarget();

					_swapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);

					CreateRenderTarget();
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

		_editor = std::make_shared<DUOLEditor::Editor>(_gameEngine);

		// 만약, 나중에 Window를 여기 저기 띄우고 나누려면 .. 그냥 Window 클래스 하나 추상화하는게 좋을 것 같음 ..
#pragma endregion

#pragma region IMGUI_TEST
		if (!CreateDeviceD3D(engineSpec.hWnd))
		{
			CleanupDeviceD3D();
			::UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
		}

		// ImGui Context를 셋업합니다.
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			// Enable Multi-Viewport / Platform Windows

		// ImGui Style을 셋업합니다.
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// 플랫폼 / 렌더러 백엔드를 셋업합니다
		ImGui_ImplWin32_Init(engineSpec.hWnd);
		ImGui_ImplDX11_Init(_device, _deviceContext);

		// 폰트를 로드합니다.
		// 만약 아무런 폰트가 로드되어 있지 않다면 기본 폰트를 사용합니다.
		// 
		io.Fonts->AddFontDefault();
#pragma endregion
	}

	void Application::Run() const
	{
		// 현재 ImGui Program의 State
		bool showDemoWindow = true;
		bool showAnotherWindow = false;
		ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.6f, 1.f);

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
				// _gameEngine->Update();

				_editor->Update(DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime());

#pragma region IMGUI_TEST
				// ImGui의 새로운 프레임을 시작합니다.
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				// 데모 윈도우를 띄웁니다. (Sample Code)
				if (showDemoWindow)
					ImGui::ShowDemoWindow(&showDemoWindow);

				// 제가 만든 심플한 윈도우를 띄웁니다. 이름이 있는 윈도우를 다룰 때 Begin 과 End를 사용합니다. 
				{
					static float f = 0.f;
					static int counter = 0;

					ImGui::Begin("Hello, World");							// Hello, World라는 이름의 윈도우를 띄우고 여기에 붙이기 시작합니다.

					ImGui::Text("This is some useful text");					// 텍스트를 띄웁니다.
					ImGui::Checkbox("Demo Window", &showDemoWindow);		// 윈도우를 오픈 / 클로즈 할지에 대한 bool value를 수정합니다.
					ImGui::Checkbox("Another Window", &showAnotherWindow);	// 다른 윈도우를 오픈 / 클로즈 할지에 대한 bool value를 수정합니다.

					ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
					ImGui::ColorEdit3("Clear Color", (float*)&clearColor);

					if (ImGui::Button("Button"))
						counter++;

					ImGui::SameLine();											// 같은 줄에 쓰겠다는 뜻
					ImGui::Text("Counter = %d", counter);

					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
					ImGui::End();
				}

				// Another Window를 띄웁니다.
				if (showAnotherWindow)
				{
					ImGui::Begin("Another Window", &showAnotherWindow);
					ImGui::Text("Hello From Another Window !");

					if (ImGui::Button("Close Me"))
						showAnotherWindow = false;
					ImGui::End();
				}

				// Rendering 합니다.
				ImGui::Render();

				const float clearColorWithAlpha[4] = { clearColor.x * clearColor.x, clearColor.y * clearColor.y,
							clearColor.z * clearColor.z, clearColor.w * clearColor.w };

				_deviceContext->ClearRenderTargetView(_mainRenderTargetView, clearColorWithAlpha);
				_deviceContext->OMSetRenderTargets(1, &_mainRenderTargetView, nullptr);
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				ImGuiIO& io = ImGui::GetIO(); (void)io;

				// Update and Render additional Platform Windows
				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
				}

				_swapChain->Present(1, 0);
#pragma endregion
			}
		}

		// Clean Up IMGUI
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		CleanupDeviceD3D();
	}

	void Application::UnInitialize()
	{
		_editor.reset();

		_gameEngine.reset();
	}
}