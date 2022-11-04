#pragma once

#include <d3d11.h>
#include "IGraphicsEngine.h"
/// <summary>
/// 처음부터 분리해서 만들껄..
/// 이제 리펙토링 시작합니다.
/// 2022.06.13 신성현
/// </summary>
class IMash;
class ResourceManager;
class RasterizerState;
/// COM(Componenet Object Model) 이란 인터페이스? 

__declspec(align(16)) class DXEngine : public IGraphicsEngine
{
public:
	DXEngine();
	virtual ~DXEngine();

private:
	static DXEngine* m_Instance;

	HWND m_MainWnd; //핸들
	float m_ClientWidth; // 화면 해상도
	float m_ClientHeight;

	Camera* m_Camera;
	Device* m_Device;
	RenderTarget* m_RenderTarget;
	DepthStencil* m_DepthStencil;
	ResourceManager* m_ResourceManager;
	RasterizerState* m_RasterizerState;
public:
	// 게임 프로세스에서 사용 함수들
	virtual void Initialize(HWND hWnd, int Width, int height) override;
	virtual void BeginRender() override;
	virtual void EndRender() override;
	virtual void OnResize() override;
	virtual IMesh* GetMeshResource(string _Name) override;
	virtual void CameraUpdate(const XMMATRIX&& _View, const XMMATRIX&& _Proj, const XMFLOAT3&& _Pos) override;
	virtual void ShadowUpdate(const XMMATRIX&& _View, const XMMATRIX&& _Proj) override; //test
	virtual float GetWidth() override { return m_ClientWidth; }
	virtual float GetHeight() override { return m_ClientHeight; }





	virtual void ExecuteRender() override;
	virtual void PostRenderingData_Particle(std::queue<std::shared_ptr<RenderingData_Particle>>&& renderQueueParticle) override;
	virtual void PostRenderingData_3D(std::queue<std::shared_ptr<RenderingData_3D>>&& renderQueueParticle) override;
	virtual void PostRenderingData_UI(std::queue<std::shared_ptr<RenderingData_UI>>&& renderQueueParticle) override;
	virtual void PostTextData(std::queue<std::shared_ptr<TextData>>&& renderQueueParticle) override;
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
	Camera* GetCamera();
	XMMATRIX GetCameraView();
	XMMATRIX GetCameraProj();
	bool GetEnable4xMsaa() { return m_Device->GetEnable4xMsaa(); }
	UINT Get4xMsaaQuality() { return m_Device->Get4xQuality(); }

public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, 16);
	}
	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};

