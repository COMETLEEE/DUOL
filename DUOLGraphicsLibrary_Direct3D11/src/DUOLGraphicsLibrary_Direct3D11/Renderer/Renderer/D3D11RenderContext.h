#pragma once
#include "DUOLGraphicsLibrary/Renderer/RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11//ComPtr.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"

#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include <dxgi.h>
#include <memory>

namespace DUOLGraphicsLibrary
{
	class D3D11RenderTarget;

 /**

     @class   D3D11RenderContext
     @brief   D3D11 Immediate Context와 SwapChain을 갖고 있다
     @details ~
     @author  KyungMin Oh

 **/
	class D3D11RenderContext : public RenderContext
	{
	public:
		D3D11RenderContext(
			const UINT64& guid
			, const ComPtr<IDXGIFactory>& factory
			, const ComPtr<ID3D11Device>& device
			, const ComPtr<ID3D11DeviceContext>& context
			, const RenderContextDesc& contextDesc
			, const RendererDesc& rendererDesc
		);

	private:
		DXGI_SAMPLE_DESC _multiSampleDesc;

		DXGI_FORMAT _colorFormat;

		DXGI_FORMAT _depthStencilFormat;

		ComPtr<ID3D11Device>        _device;

		ComPtr<ID3D11DeviceContext> _context;

		ComPtr<IDXGISwapChain>      _swapChain;

		ComPtr<ID3D11Texture2D>         _backbufferTexture;

		std::unique_ptr<D3D11RenderTarget>  _backbufferRenderTargetView;

		ComPtr<ID3D11Texture2D>         _backbufferDepthStencilTexture;

		std::unique_ptr<D3D11RenderTarget>  _backbufferDepthStencilView;

	private:
		void CreateSwapChain(const ComPtr<IDXGIFactory>& factory, const RendererDesc& rendererDesc, const RenderContextDesc& contextDesc);

		void CreateBackBuffer();

		void ResizeBackBuffer();

		void GetSampleDesc(const ComPtr<ID3D11Device>& device, const ScreenDesc& screenDesc);

	public:
		virtual void Present() override;

		virtual void SetScreenDesc(const ScreenDesc& screenDesc) override;

		virtual void SetFrameRateDesc(const FrameRateDesc& framerateDesc) override;

		virtual RenderTarget* GetBackBufferRenderTarget() override;

		virtual RenderTarget* GetBackBufferDepthStencil() override;

		ID3D11DeviceContext* GetNativeContext() { return _context.Get(); };

	public:
		//void SetViewports(std::uint32_t numViewports, const Viewport* viewportArray);
		////void SetScissors(std::uint32_t numScissors, const Scissor* scissorArray);

		//void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology);

		//void SetInputLayout(ID3D11InputLayout* inputLayout);

		//void SetVertexShader(ID3D11VertexShader* shader);

		//void SetHullShader(ID3D11HullShader* shader);

		//void SetDomainShader(ID3D11DomainShader* shader);

		//void SetGeometryShader(ID3D11GeometryShader* shader);

		//void SetPixelShader(ID3D11PixelShader* shader);

		//void SetComputeShader(ID3D11ComputeShader* shader);

		//void SetRasterizerState(ID3D11RasterizerState* rasterizerState);

		//void SetDepthStencilState(ID3D11DepthStencilState* depthStencilState);

		//void SetDepthStencilState(ID3D11DepthStencilState* depthStencilState, UINT stencilRef);

		//void SetStencilRef(UINT stencilRef);

		//void SetBlendState(ID3D11BlendState* blendState, UINT sampleMask);

		//void SetBlendState(ID3D11BlendState* blendState, const FLOAT* blendFactor, UINT sampleMask);

		//void SetBlendFactor(const FLOAT* blendFactor);

		//void SetConstantBuffers(
		//	UINT                    startSlot,
		//	UINT                    bufferCount,
		//	ID3D11Buffer* const* buffers,
		//	long                    stageFlags
		//);

		//void SetConstantBuffersArray(
		//	UINT                    startSlot,
		//	UINT                    bufferCount,
		//	ID3D11Buffer* const* buffers,
		//	const UINT* firstConstants,
		//	const UINT* numConstants,
		//	long                    stageFlags
		//);

		//void SetShaderResources(
		//	UINT                                startSlot,
		//	UINT                                resourceCount,
		//	ID3D11ShaderResourceView* const* views,
		//	long                                stageFlags
		//);

		//void SetSamplers(
		//	UINT                        startSlot,
		//	UINT                       samplerCount,
		//	ID3D11SamplerState* const* samplers,
		//	long                        stageFlags
		//);

	};
}
