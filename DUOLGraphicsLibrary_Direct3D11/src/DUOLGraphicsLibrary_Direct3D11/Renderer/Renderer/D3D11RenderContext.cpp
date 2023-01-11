#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/RenderTarget/D3D11RenderTarget.h"
#include "DUOLGraphicsLibrary/ShaderFlags.h"

namespace DUOLGraphicsLibrary
{
	void D3D11RenderContext::CreateSwapChain(const ComPtr<IDXGIFactory>& factory, const RendererDesc& rendererDesc, const RenderContextDesc& contextDesc)
	{

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		{
			ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
			//후면 버퍼의 속성들
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //이 버퍼의 용도가 무엇인가? (후면 버퍼에 렌더링을 하는 용도(스왑체인))
			swapChainDesc.BufferCount = 1;  //스왑체인에 사용될 버퍼의 개수 일반적으로 후면버퍼하나(이중버퍼링), 삼중버퍼링을 지정할 수 도 있다.
			swapChainDesc.OutputWindow = reinterpret_cast<HWND>(rendererDesc._handle); // 렌더링 결과를 표시할 창의 핸들
			swapChainDesc.Windowed = !contextDesc._screenDesc._isFullscreen; // 창모드효과
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;

			//스왑이펙트와 플래그들을 추가 설정할 수 있다. 여기엔 포함되지 않음.
			swapChainDesc.BufferDesc.Width = static_cast<UINT>(_screenDesc._screenSize.x); // 스크린의크기
			swapChainDesc.BufferDesc.Height = static_cast<UINT>(_screenDesc._screenSize.y);
			swapChainDesc.BufferDesc.RefreshRate.Numerator = _frameRateDesc._refreshRate; //디스플레이 모드 갱신율
			swapChainDesc.BufferDesc.RefreshRate.Denominator = _frameRateDesc._interval;
			swapChainDesc.BufferDesc.Format = _colorFormat; //후면버퍼 픽셀 형식
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			//이외에도 스캔라인, 비례모드를 설정할 수 있음
			//다중표본화를 위한 녀석. 표본 개수와 품질

			GetSampleDesc(_device, _screenDesc);

			swapChainDesc.SampleDesc = _multiSampleDesc;
		}

		auto hr = factory->CreateSwapChain(_device.Get(), &swapChainDesc, _swapChain.ReleaseAndGetAddressOf());

		DXThrowError(hr, "D3D11RenderContext : CreateSwapchain Failed");
	}

	void D3D11RenderContext::CreateBackBuffer()
	{
		HRESULT hr;

		hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(_backbufferTexture.GetAddressOf()));
		DXThrowError(hr, "D3D11RenderContext : CreateBackBuffer Failed, GetBuffer");

		_backbufferRenderTargetView->CreateRenderTargetViews(_device.Get(), _backbufferTexture.Get(), RenderTargetType::Color, _colorFormat);
	}

	void D3D11RenderContext::ResizeBackBuffer()
	{
		ID3D11RenderTargetView* nullViews[8] = { nullptr, };
		_context->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);

		ID3D11ShaderResourceView* nullSRVViews[32] = { nullptr, };

		_context->VSSetShaderResources(0, _countof(nullSRVViews), nullSRVViews);
		_context->PSSetShaderResources(0, _countof(nullSRVViews), nullSRVViews);

		_backbufferTexture.Reset();
		_backbufferRenderTargetView->UnloadRenderTargetView();

		//resize 함수
		//bufferCount = 0 -> 현상유지, width, height = 0 -> 스크린사이즈, format = unknown -> 현상유지

		auto hr = _swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		DXThrowError(hr, "D3D11RenderContext : ResizeBackBuffer Failed, ResizeBuffer");

		CreateBackBuffer();
	}

	void D3D11RenderContext::GetSampleDesc(const ComPtr<ID3D11Device>& device, const ScreenDesc& screenDesc)
	{
		for (UINT maxSampleCount = screenDesc._sampleCount; maxSampleCount > 1; --maxSampleCount)
		{
			UINT numQualityLevels = 0;
			if (device->CheckMultisampleQualityLevels(_colorFormat, maxSampleCount, &numQualityLevels) == S_OK)
			{
				if (numQualityLevels > 0)
					_multiSampleDesc = { maxSampleCount, numQualityLevels - 1 };
			}
		}
		_multiSampleDesc = { 1u, 0u };
	}

	void D3D11RenderContext::Present()
	{
		_swapChain->Present(_frameRateDesc._interval, 0);
	}

	void D3D11RenderContext::SetScreenDesc(const ScreenDesc& screenDesc)
	{
		if (_screenDesc._screenSize != screenDesc._screenSize)
		{
			_screenDesc._screenSize = screenDesc._screenSize;

			ResizeBackBuffer();
		}

		if (_screenDesc._isFullscreen != screenDesc._isFullscreen)
		{
			//todo 
			//추가할 것.
		}

		_screenDesc = screenDesc;
	}

	void D3D11RenderContext::SetFrameRateDesc(const FrameRateDesc& framerateDesc)
	{
		//todo
		//추가할것
	}

	RenderTarget* D3D11RenderContext::GetBackBufferRenderTarget()
	{
		return _backbufferRenderTargetView.get();
	}

	Texture* D3D11RenderContext::GetBackBufferTexture()
	{
		//todo.. 하...
		return nullptr;
	}

	D3D11RenderContext::D3D11RenderContext(
		const UINT64& guid
		, const ComPtr<IDXGIFactory>& factory
		, const ComPtr<ID3D11Device>& device
		, const ComPtr<ID3D11DeviceContext>& context
		, const RenderContextDesc& contextDesc
		, const RendererDesc& rendererDesc) :
		RenderContext(guid, contextDesc._screenDesc, contextDesc._frameRate)
		, _colorFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
		, _multiSampleDesc{ 1u, 0u }
		, _device(device)
		, _context(context)
	{
		CreateSwapChain(factory, rendererDesc, contextDesc);
		_backbufferRenderTargetView = std::move(std::make_unique<D3D11RenderTarget>(0));
		CreateBackBuffer();
	}
}
