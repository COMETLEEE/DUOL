#include "pch.h"
#include "DXEngine.h"
#include "Renderer.h"

#include "../Common/Imgui/imgui.h"
#include "../Common/Imgui/imgui_impl_win32.h"
#include "../Common/Imgui/imgui_impl_dx11.h"
#include "../Common/Imgui/imgui_internal.h"

DXEngine* DXEngine::m_Instance = nullptr;

DXEngine::DXEngine() : IGraphicsEngine(),
m_MainWnd(nullptr),
m_RasterizerState(nullptr)
{
	if (m_Instance)
		delete m_Instance;

	m_Instance = this;
}

DXEngine::~DXEngine()
{
	delete m_Camera;
	delete m_Device;
	delete m_RenderTarget;
	delete m_DepthStencil;
	delete m_ResourceManager;
	delete m_RasterizerState;
	delete m_Renderer;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DXEngine::Initialize(HWND hWnd, int Width, int height)
{
	m_MainWnd = hWnd;
	m_Device = new Device();
	m_Device->Initialize(hWnd, Width, height);

	m_Camera = new Camera();

	m_DepthStencil = new DepthStencil();
	m_RenderTarget = new RenderTarget();

	m_RasterizerState = new RasterizerState();
	m_ResourceManager = new ResourceManager();
	m_ResourceManager->init();

	m_Renderer = new Renderer();

	// ���� �Ʒ��δ� OnReSzie �Լ��� �ٽ� �����.
	// ���� ��� ���� ����
	// �� ���ۿ� ���� ���� ��� �� ����.

	OnResize();



	/// IMGUI 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(m_Device->GetDevice(), m_Device->GetDeviceContext());
	ImGui::StyleColorsDark();

}


DXEngine* DXEngine::GetInstance()
{
	return m_Instance;
}

ResourceManager* DXEngine::GetResourceManager()
{
	return m_ResourceManager;
}

ID3D11Device* DXEngine::GetD3dDevice()
{
	return m_Device->GetDevice();
}

ID3D11DeviceContext* DXEngine::Getd3dImmediateContext()
{
	return m_Device->GetDeviceContext();
}

IDXGISwapChain* DXEngine::GetSwapChain()
{
	return m_Device->GetSwapChain();
}

DepthStencil* DXEngine::GetDepthStencil()
{
	return m_DepthStencil;
}

RenderTarget* DXEngine::GetRenderTarget()
{
	return m_RenderTarget;
}

Camera* DXEngine::GetCamera()
{
	return m_Camera;
}


void DXEngine::ExecuteRender()
{
	BeginRender();
	// ������ ť�� �� �Ѱ��ְ� �������� EndRender�� BeginRender �Լ��� �����Ų��.
	m_Renderer->ExecuteRender();
	EndRender();
	
}

void DXEngine::PostRenderingData_Particle(std::queue<std::shared_ptr<RenderingData_Particle>>&& renderQueueParticle)
{
	m_Renderer->MoveRenderingData_Particle(move(renderQueueParticle));
}

void DXEngine::PostRenderingData_3D(std::queue<std::shared_ptr<RenderingData_3D>>&& renderQueue3D)
{
	m_Renderer->MoveRenderingData_3D(move(renderQueue3D));
}

void DXEngine::PostRenderingData_UI(std::queue<std::shared_ptr<RenderingData_UI>>&& renderQueueUI)
{
	m_Renderer->MoveRenderingData_UI(move(renderQueueUI));
}

void DXEngine::PostTextData(std::queue<std::shared_ptr<TextData>>&& renderQueueText)
{
	m_Renderer->MoveTextData(move(renderQueueText));
}

void DXEngine::PostPerFrameData(std::shared_ptr<PerFrameData>&& perframeData)
{
	m_Renderer->MovePerFrameData(move(perframeData));
}

void DXEngine::ReleaseTexture()
{
}

DirectX::XMMATRIX DXEngine::GetCameraView()
{
	return m_Camera->GetCameraView();
}

DirectX::XMMATRIX DXEngine::GetCameraProj()
{
	return m_Camera->GetCameraProj();
}

void DXEngine::BeginRender()
{
	m_RenderTarget->BeginRender();
	m_DepthStencil->Clear();
	Effects::TextureRenderFX->SetCurrentViewProj(GetCamera()->GetCurrentViewProj());
	Effects::TextureRenderFX->SetPrevViewProj(GetCamera()->GetPrevViewProj());
}

void DXEngine::EndRender()
{
	m_RenderTarget->EndRender();
	//// ��ü ���� �ؽ��Ŀ� �׸���!!
	//RenderToTexture();
	//// ����� â�� �׸���.
	//RenderDebugWindow();
	// presenting (����) - �ĸ� ���۸� ���� ���ۿ� ��ȯ�ؼ� ȭ�鿡 ǥ�õǰ� �ϴ� ��.
	// ����� �ĸ��� �����͸� ��ü�ϴ� ��.




	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Test");
	ImGui::End();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::EndFrame();


	GetSwapChain()->Present(1, 0);
}

void DXEngine::OnResize()
{

	ATLTRACE("OnResize ����.\n");
	RECT _rect;
	GetClientRect(m_MainWnd, &_rect);
	// cache properties
	m_ClientWidth = float(_rect.right - _rect.left);
	m_ClientHeight = float(_rect.bottom - _rect.top);
	m_RenderTarget->OnResize();
	m_DepthStencil->OnResize();

}
