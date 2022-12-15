#include "DUOLEditor/UI/GUIManager.h"

#include <d3d11.h>

#include "DUOLCommon/ImGui/imgui_impl_dx11.h"
#include "DUOLCommon/ImGui/imgui_impl_win32.h"

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
	GUIManager::GUIManager()
	{
		
	}

	GUIManager::~GUIManager()
	{

	}

	void GUIManager::Initialize(HWND hWnd)
	{
		if (!CreateDeviceD3D(hWnd))
		{
			CleanupDeviceD3D();
			// ::UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
		}

		// D3D ����̽��� �������ϴ�.
		isDeviceOpen = true;
		 
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

		// �÷��� / ������ �鿣�带 �¾��մϴ�.
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(_device, _deviceContext);

		// ��Ʈ�� �ε��մϴ�.
		// ���� �ƹ��� ��Ʈ�� �ε�Ǿ� ���� �ʴٸ� �⺻ ��Ʈ�� ����մϴ�.
		io.Fonts->AddFontDefault();
	}

	void GUIManager::UnInitialize()
	{
		// Clean Up IMGUI
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		CleanupDeviceD3D();

		isDeviceOpen = false;
	}

	void GUIManager::OnResize(const uint32_t& screenWidth, const uint32_t& screenHeight)
	{
		CleanupRenderTarget();

		_swapChain->ResizeBuffers(0, screenWidth, screenHeight, DXGI_FORMAT_UNKNOWN, 0);

		CreateRenderTarget();
	}

	void GUIManager::Update(float deltaTime)
	{
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