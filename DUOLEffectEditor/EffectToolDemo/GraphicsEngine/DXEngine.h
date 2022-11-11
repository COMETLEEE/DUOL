#pragma once

#include <d3d11.h>
#include "IGraphicsEngine.h"
/// <summary>
/// 처음부터 분리해서 만들껄..
/// 이제 리펙토링 시작합니다.
/// 2022.06.13 신성현
/// </summary>
class ResourceManager;
class RasterizerState;
class Renderer;
class Device;
class RenderTarget;
class DepthStencil;
class SamplerState;

/// COM(Componenet Object Model) 이란 인터페이스? 

class DXEngine : public IGraphicsEngine
{
public:
	DXEngine();
	virtual ~DXEngine();

private:
	static DXEngine* m_Instance;

	HWND m_MainWnd; //핸들
	float m_ClientWidth; // 화면 해상도
	float m_ClientHeight;

	Device* m_Device;

	RenderTarget* m_RenderTarget;

	DepthStencil* m_DepthStencil;

	ResourceManager* m_ResourceManager;

	RasterizerState* m_RasterizerState;

	SamplerState* _samplerState;

	Renderer* m_Renderer;

public:
	// 게임 프로세스에서 사용 함수들
	virtual void Initialize(HWND hWnd, int Width, int height) override;

	void BeginRender();

	void EndRender();

	virtual void OnResize() override;

	float GetWidth() { return m_ClientWidth; }

	float GetHeight() { return m_ClientHeight; }





	virtual void ExecuteRender() override;
	virtual void PostRenderingData_Particle(std::queue<std::shared_ptr<RenderingData_Particle>>&& renderQueueParticle) override;
	virtual void PostRenderingData_3D(std::queue<std::shared_ptr<RenderingData_3D>>&& renderQueue3D) override;
	virtual void PostRenderingData_UI(std::queue<std::shared_ptr<RenderingData_UI>>&& renderQueueUI) override;
	virtual void PostRenderingData_ImGui(std::queue<std::function<void()>>&& renderQueueImGui) override;

	virtual void PostTextData(std::queue<std::shared_ptr<TextData>>&& renderQueueText) override;
	virtual void PostPerFrameData(std::shared_ptr<PerFrameData>&& perframeData) override;
	virtual void ReleaseTexture() override;


public: //Get Set

	static DXEngine* GetInstance();
	ResourceManager* GetResourceManager();
	HWND GetHWND() { return m_MainWnd; }

	ID3D11Device* GetD3dDevice(); // 디바이스
	ID3D11DeviceContext* Getd3dImmediateContext(); // 디바이스 컨텍스트
	IDXGISwapChain* GetSwapChain();
	DepthStencil* GetDepthStencil();
	RenderTarget* GetRenderTarget();

	bool GetEnable4xMsaa() ;
	UINT Get4xMsaaQuality();


};

