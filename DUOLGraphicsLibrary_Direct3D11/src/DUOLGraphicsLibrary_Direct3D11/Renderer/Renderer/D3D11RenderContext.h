#pragma once
#include "DUOLGraphicsLibrary/Renderer/RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11//ComPtr.h"
#include <d3d11.h>
#include <dxgi.h>

namespace DUOLGraphicsLibrary
{
	class D3D11RenderContext : public RenderContext
	{
	private:
		DXGI_SAMPLE_DESC _multiSampleDesc;

		DXGI_FORMAT _colorFormat;

		DXGI_FORMAT _depthStencilFormat;

		ComPtr<ID3D11Device>        _device;

		ComPtr<ID3D11DeviceContext> _context;

		ComPtr<IDXGISwapChain>      _swapChain;

		ComPtr<ID3D11Texture2D>         _backbufferTexture;

		ComPtr<ID3D11RenderTargetView>  _backbufferRenderTargetView;

		ComPtr<ID3D11Texture2D>         _backbufferDepthStencilTexture;

		ComPtr<ID3D11DepthStencilView>  _backbufferDepthStencilView;

	private:
		void CreateSwapChain(const ComPtr<IDXGIFactory>& factory, const RendererDesc& rendererDesc);

		void CreateBackBuffer();

		void ResizeBackBuffer();

		void GetSampleDesc(const ComPtr<ID3D11Device>& device, const ScreenDesc& screenDesc);

	public:
		D3D11RenderContext(
			const ComPtr<IDXGIFactory>& factory
			, const ComPtr<ID3D11Device>& device
			, const ComPtr<ID3D11DeviceContext>& context
			, const RenderContextDesc& contextDesc
			, const RendererDesc& rendererDesc
		);
	};
}
