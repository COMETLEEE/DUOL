#include "DUOLEditor/Application.h"

#include <cassert>

#include "DUOLCommon/LogHelper.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLCommon/ImGui/imgui_impl_win32.h"
#include "DUOLCommon/ImGui/imgui_impl_dx11.h"

// Forward declare message handler from imgui_impl_win32.cpp => <window.h> �������� ���ֱ� ���ؼ� �̷��� ����մϴ�. 
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

		// ������ ��Ʈ�� .inl�� ���� �ʱ� ���� ���� �Ľ� �� â ũ��, ���, �̸� ���� �����Ѵ�.
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

		// ����, ���߿� Window�� ���� ���� ���� �������� .. �׳� Window Ŭ���� �ϳ� �߻�ȭ�ϴ°� ���� �� ���� ..
#pragma endregion

#pragma region IMGUI_TEST
		if (!CreateDeviceD3D(engineSpec.hWnd))
		{
			CleanupDeviceD3D();
			::UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
		}

		// ImGui Context�� �¾��մϴ�.
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			// Enable Multi-Viewport / Platform Windows

		// ImGui Style�� �¾��մϴ�.
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// �÷��� / ������ �鿣�带 �¾��մϴ�
		ImGui_ImplWin32_Init(engineSpec.hWnd);
		ImGui_ImplDX11_Init(_device, _deviceContext);

		// ��Ʈ�� �ε��մϴ�.
		// ���� �ƹ��� ��Ʈ�� �ε�Ǿ� ���� �ʴٸ� �⺻ ��Ʈ�� ����մϴ�.
		// 
		io.Fonts->AddFontDefault();
#pragma endregion
	}

	void Application::Run() const
	{
		// ���� ImGui Program�� State
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
				// ImGui�� ���ο� �������� �����մϴ�.
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				// ���� �����츦 ���ϴ�. (Sample Code)
				if (showDemoWindow)
					ImGui::ShowDemoWindow(&showDemoWindow);

				// ���� ���� ������ �����츦 ���ϴ�. �̸��� �ִ� �����츦 �ٷ� �� Begin �� End�� ����մϴ�. 
				{
					static float f = 0.f;
					static int counter = 0;

					ImGui::Begin("Hello, World");							// Hello, World��� �̸��� �����츦 ���� ���⿡ ���̱� �����մϴ�.

					ImGui::Text("This is some useful text");					// �ؽ�Ʈ�� ���ϴ�.
					ImGui::Checkbox("Demo Window", &showDemoWindow);		// �����츦 ���� / Ŭ���� ������ ���� bool value�� �����մϴ�.
					ImGui::Checkbox("Another Window", &showAnotherWindow);	// �ٸ� �����츦 ���� / Ŭ���� ������ ���� bool value�� �����մϴ�.

					ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
					ImGui::ColorEdit3("Clear Color", (float*)&clearColor);

					if (ImGui::Button("Button"))
						counter++;

					ImGui::SameLine();											// ���� �ٿ� ���ڴٴ� ��
					ImGui::Text("Counter = %d", counter);

					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
					ImGui::End();
				}

				// Another Window�� ���ϴ�.
				if (showAnotherWindow)
				{
					ImGui::Begin("Another Window", &showAnotherWindow);
					ImGui::Text("Hello From Another Window !");

					if (ImGui::Button("Close Me"))
						showAnotherWindow = false;
					ImGui::End();
				}

				// Rendering �մϴ�.
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