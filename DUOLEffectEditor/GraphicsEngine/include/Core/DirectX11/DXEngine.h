#pragma once
#include "Export/IGraphicsEngine.h"
#include "util/TypeDefine.h"

/// <summary>
/// 처음부터 분리해서 만들껄..
/// 이제 리펙토링 시작합니다.
/// 2022.06.13 신성현
/// </summary>
namespace MuscleGrapics
{
	class ResourceManager;
	class RasterizerState;
	class Renderer;
	class Device;
	class RenderTarget;
	class DepthStencil;
	class SamplerState;
	class BlendState;

	/// COM(Componenet Object Model) 이란 인터페이스? 

	class DXEngine : public IGraphicsEngine
	{
	public:
		DXEngine();

		virtual ~DXEngine() override;

	private:
		static DXEngine* _instance;

		HWND _mainWnd; //핸들

		float _clientWidth; // 화면 해상도

		float _clientHeight;

		Device* _device;

		RenderTarget* _renderTarget;

		DepthStencil* _depthStencil;

		ResourceManager* _resourceManager;

		RasterizerState* _rasterizerState;

		SamplerState* _samplerState;

		Renderer* _renderer;

		BlendState* _blendState;

	public:
		// 게임 프로세스에서 사용 함수들
		virtual void Initialize(HWND hWnd, int Width, int height) override;

		void BeginRender();

		void EndRender();

		virtual void OnResize() override;

		float GetWidth() { return _clientWidth; }

		float GetHeight() { return _clientHeight; }

		virtual void ExecuteRender() override;

		virtual void PostRenderingData_Particle(std::queue<std::shared_ptr<RenderingData_Particle>>&& renderQueueParticle) override;

		virtual void PostRenderingData_3D(std::queue<std::shared_ptr<RenderingData_3D>>&& renderQueue3D) override;

		virtual void PostRenderingData_UI(std::queue<std::shared_ptr<RenderingData_UI>>&& renderQueueUI) override;

		virtual void PostRenderingData_ImGui(std::queue<std::function<void()>>&& renderQueueImGui) override;

		virtual void PostTextData(std::queue<std::shared_ptr<TextData>>&& renderQueueText) override;

		virtual void PostPerFrameData(std::shared_ptr<PerFrameData>&& perframeData) override;

		virtual void ReleaseTexture() override;

		virtual void* InsertTexture(tstring path) override;

		virtual void* GetTexture(tstring textureMap) override;

		virtual void* GetNoiseMap(std::tuple<float, int, float> key) override;

		virtual unsigned int PickObjectID(int x, int y) override;

	public: //Get Set
		static DXEngine* GetInstance();

		ResourceManager* GetResourceManager();

		HWND GetHWND() { return _mainWnd; }

		ID3D11Device* GetD3dDevice(); // 디바이스

		ID3D11DeviceContext* Getd3dImmediateContext(); // 디바이스 컨텍스트

		IDXGISwapChain* GetSwapChain();

		DepthStencil* GetDepthStencil();

		RenderTarget* GetRenderTarget();

		bool GetEnable4xMsaa();

		UINT Get4xMsaaQuality();

	};
}
