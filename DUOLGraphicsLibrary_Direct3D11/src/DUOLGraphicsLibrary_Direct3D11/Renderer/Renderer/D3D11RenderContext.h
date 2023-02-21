#pragma once
#include "DUOLGraphicsLibrary/Renderer/RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11//ComPtr.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"

#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include <dxgi.h>
#include <memory>

namespace DUOLGraphicsLibrary
{
	class FontEngine;
	class D3D11Texture;
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

		~D3D11RenderContext();

	private:
		DXGI_SAMPLE_DESC _multiSampleDesc;

		DXGI_FORMAT _colorFormat;

		ComPtr<ID3D11Device>        _device;

		ComPtr<ID3D11DeviceContext> _context;

		ComPtr<IDXGISwapChain>      _swapChain;

		ComPtr<ID3D11Texture2D>         _backbufferTexture;

		std::unique_ptr<FontEngine> _fontEngine;

	private:
		std::unique_ptr<D3D11Texture>  _backbufferShaderResourceView;

		std::unique_ptr<D3D11RenderTarget>  _backbufferRenderTargetView;

	private:
		void CreateSwapChain(const ComPtr<IDXGIFactory>& factory, const RendererDesc& rendererDesc, const RenderContextDesc& contextDesc);

		void CreateBackBuffer();

		void ResizeBackBuffer();

		void GetSampleDesc(const ComPtr<ID3D11Device>& device, const ScreenDesc& screenDesc);

	public:
		IDXGISwapChain* GetSwapHChain();

		virtual void Present() override;

		virtual void SetScreenDesc(const ScreenDesc& screenDesc) override;

		virtual void SetFrameRateDesc(const FrameRateDesc& framerateDesc) override;

		virtual RenderTarget* GetBackBufferRenderTarget() override;

		virtual Texture* GetBackBufferTexture() override;

		ID3D11DeviceContext* GetNativeContext() { return _context.Get(); };

		FontEngine* GetFontEngine() const
		{
			return _fontEngine.get();
		}

	};
}
