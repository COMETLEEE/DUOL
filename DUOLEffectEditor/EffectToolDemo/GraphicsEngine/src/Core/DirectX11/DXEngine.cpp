#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/Renderer.h"
#include "Core/DirectX11/Device.h"
#include "Core/DirectX11/RenderTarget.h"
#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/BlendState.h"
#include "Core/DirectX11/SamplerState.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/DirectX11/RasterizerState.h"


#include "../Common/Imgui/imgui.h"
#include "../Common/Imgui/imgui_impl_win32.h"
#include "../Common/Imgui/imgui_impl_dx11.h"
#include "../Common/Imgui/imgui_internal.h"

namespace MuscleGrapics
{
	DXEngine* DXEngine::_instance = nullptr;

	DXEngine::DXEngine() :
		IGraphicsEngine(),
		_mainWnd(nullptr),
		_rasterizerState(nullptr)
	{
		if (_instance)
			delete _instance;

		_instance = this;
	}

	DXEngine::~DXEngine()
	{
		delete _device;

		delete _renderTarget;

		delete _depthStencil;

		delete _resourceManager;

		delete _rasterizerState;

		delete _renderer;

		delete _samplerState;

		delete _blendState;

		ImGui_ImplDX11_Shutdown();

		ImGui_ImplWin32_Shutdown();

		ImGui::DestroyContext();
	}

	void DXEngine::Initialize(HWND hWnd, int Width, int height)
	{
		_mainWnd = hWnd;

		_device = new Device();

		_device->Initialize(hWnd, Width, height);

		_depthStencil = new DepthStencil();

		_renderTarget = new RenderTarget();

		_rasterizerState = new RasterizerState();

		_samplerState = new SamplerState();

		_blendState = new BlendState();

		_resourceManager = new ResourceManager();

		_resourceManager->init();

		_renderer = new Renderer();

		OnResize();

		/// IMGUI 
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;

			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}


		ImGui_ImplWin32_Init(hWnd);

		ImGui_ImplDX11_Init(_device->GetDevice(), _device->GetDeviceContext());
	}
	DXEngine* DXEngine::GetInstance()
	{
		return _instance;
	}
	ResourceManager* DXEngine::GetResourceManager()
	{
		return _resourceManager;
	}
	ID3D11Device* DXEngine::GetD3dDevice()
	{
		return _device->GetDevice();
	}
	ID3D11DeviceContext* DXEngine::Getd3dImmediateContext()
	{
		return _device->GetDeviceContext();
	}
	IDXGISwapChain* DXEngine::GetSwapChain()
	{
		return _device->GetSwapChain();
	}
	DepthStencil* DXEngine::GetDepthStencil()
	{
		return _depthStencil;
	}
	RenderTarget* DXEngine::GetRenderTarget()
	{
		return _renderTarget;
	}
	void DXEngine::ExecuteRender()
	{
		BeginRender(); // 렌더링 큐를 다 넘겨주고 마지막에 EndRender와 BeginRender 함수를 실행시킨다.

		_renderer->ExecuteRender(); // 렌더러가 패스를 돌며 렌더링을 실시한다.

		_renderTarget->ExecuteRender(); // 렌더 타겟에 디퍼드 조립을 한다. 

		_renderer->ExecuteForwardRender(); // Ui, Particle 등 반투명 오브젝트를 포워드 렌더링으로 실행한다.

		EndRender();
	}
	void DXEngine::PostRenderingData_Particle(std::queue<std::shared_ptr<RenderingData_Particle>>&& renderQueueParticle)
	{
		_renderer->MoveRenderingData_Particle(move(renderQueueParticle));
	}
	void DXEngine::PostRenderingData_3D(std::queue<std::shared_ptr<RenderingData_3D>>&& renderQueue3D)
	{
		_renderer->MoveRenderingData_3D(move(renderQueue3D));
	}
	void DXEngine::PostRenderingData_UI(std::queue<std::shared_ptr<RenderingData_UI>>&& renderQueueUI)
	{
		_renderer->MoveRenderingData_UI(move(renderQueueUI));
	}
	void DXEngine::PostRenderingData_ImGui(std::queue<std::function<void()>>&& renderQueueImGui)
	{
		_renderer->MoveRenderingData_ImGui(move(renderQueueImGui));
	}
	void DXEngine::PostTextData(std::queue<std::shared_ptr<TextData>>&& renderQueueText)
	{
		_renderer->MoveTextData(move(renderQueueText));
	}
	void DXEngine::PostPerFrameData(std::shared_ptr<PerFrameData>&& perframeData)
	{
		_renderer->MovePerFrameData(move(perframeData));
	}
	void DXEngine::ReleaseTexture()
	{
	}
	void* DXEngine::InsertTexture(tstring path)
	{
		return GetResourceManager()->InsertTexture(path);
	}
	void* DXEngine::GetTexture(tstring textureMap)
	{
		return GetResourceManager()->GetTexture(textureMap);
	}
	bool DXEngine::GetEnable4xMsaa()
	{
		return _device->GetEnable4xMsaa();
	}
	UINT DXEngine::Get4xMsaaQuality()
	{
		return _device->Get4xQuality();
	}
	void DXEngine::BeginRender()
	{
		_renderTarget->BeginRender();

		_depthStencil->Clear();
	}
	void DXEngine::EndRender()
	{
		_renderTarget->PopShaderResource();

		ImGuiIO& io = ImGui::GetIO();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();

			ImGui::RenderPlatformWindowsDefault();
		}

		GetSwapChain()->Present(1, 0);
	}

	void DXEngine::OnResize()
	{
		ATLTRACE("OnResize 진입.\n");

		RECT _rect;

		GetClientRect(_mainWnd, &_rect);

		_clientWidth = float(_rect.right - _rect.left);

		_clientHeight = float(_rect.bottom - _rect.top);

		_renderTarget->OnResize();

		_depthStencil->OnResize();

	}
}
