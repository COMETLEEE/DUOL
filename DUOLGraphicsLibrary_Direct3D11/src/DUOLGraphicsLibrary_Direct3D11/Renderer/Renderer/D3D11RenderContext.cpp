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
			//�ĸ� ������ �Ӽ���
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //�� ������ �뵵�� �����ΰ�? (�ĸ� ���ۿ� �������� �ϴ� �뵵(����ü��))
			swapChainDesc.BufferCount = 1;  //����ü�ο� ���� ������ ���� �Ϲ������� �ĸ�����ϳ�(���߹��۸�), ���߹��۸��� ������ �� �� �ִ�.
			swapChainDesc.OutputWindow = reinterpret_cast<HWND>(rendererDesc._handle); // ������ ����� ǥ���� â�� �ڵ�
			swapChainDesc.Windowed = !contextDesc._screenDesc._isFullscreen; // â���ȿ��
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;

			//��������Ʈ�� �÷��׵��� �߰� ������ �� �ִ�. ���⿣ ���Ե��� ����.
			swapChainDesc.BufferDesc.Width = static_cast<UINT>(_screenDesc._screenSize.x); // ��ũ����ũ��
			swapChainDesc.BufferDesc.Height = static_cast<UINT>(_screenDesc._screenSize.y);
			swapChainDesc.BufferDesc.RefreshRate.Numerator = _frameRateDesc._refreshRate; //���÷��� ��� ������
			swapChainDesc.BufferDesc.RefreshRate.Denominator = _frameRateDesc._interval;
			swapChainDesc.BufferDesc.Format = _colorFormat; //�ĸ���� �ȼ� ����
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			//�̿ܿ��� ��ĵ����, ��ʸ�带 ������ �� ����
			//����ǥ��ȭ�� ���� �༮. ǥ�� ������ ǰ��

			GetSampleDesc(_device, _screenDesc);

			swapChainDesc.SampleDesc = _multiSampleDesc;
		}

		auto hr = factory->CreateSwapChain(_device.Get(), &swapChainDesc, _swapChain.ReleaseAndGetAddressOf());

		DXThrowError(hr, "D3D11RenderContext : CreateSwapchain Failed");
	}

	void D3D11RenderContext::CreateBackBuffer()
	{
		HRESULT hr;

		_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(_backbufferTexture.GetAddressOf()));
		DXThrowError(hr, "D3D11RenderContext : CreateBackBuffer Failed, GetBuffer");

		RenderTargetDesc backbufferRenderTargetDesc;

		_backbufferRenderTargetView = std::make_unique<D3D11RenderTarget>(0, _device.Get(), _backbufferTexture.Get(), RenderTargetType::Color, _colorFormat);

		D3D11_TEXTURE2D_DESC textureDesc;
		{
			textureDesc.Width = static_cast<UINT>(_screenDesc._screenSize.x);
			textureDesc.Height = static_cast<UINT>(_screenDesc._screenSize.y);
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = _depthStencilFormat;
			textureDesc.SampleDesc = _multiSampleDesc;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;
		}
		hr = _device->CreateTexture2D(&textureDesc, nullptr, _backbufferDepthStencilTexture.ReleaseAndGetAddressOf());
		DXThrowError(hr, "D3D11RenderContext : CreateBackBuffer Failed, CreateTexture2D");


		_backbufferDepthStencilView = std::make_unique<D3D11RenderTarget>(0, _device.Get(), _backbufferDepthStencilTexture.Get(), RenderTargetType::DepthStencil, _depthStencilFormat);
	}

	void D3D11RenderContext::ResizeBackBuffer()
	{
		_context->OMSetRenderTargets(0, nullptr, nullptr);

		_backbufferTexture.Reset();
		_backbufferRenderTargetView.release();
		_backbufferDepthStencilTexture.Reset();
		_backbufferDepthStencilView.release();

		//resize �Լ�
		//bufferCount = 0 -> ��������, width, height = 0 -> ��ũ��������, format = unknown -> ��������

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
		_swapChain->Present(0, 0);
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
			//�߰��� ��.
		}

		_screenDesc = screenDesc;
	}

	void D3D11RenderContext::SetFrameRateDesc(const FrameRateDesc& framerateDesc)
	{
		//todo
		//�߰��Ұ�
	}

	RenderTarget* D3D11RenderContext::GetBackBufferRenderTarget()
	{
		return _backbufferRenderTargetView.get();
	}

	RenderTarget* D3D11RenderContext::GetBackBufferDepthStencil()
	{
		return _backbufferDepthStencilView.get();
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
		CreateBackBuffer();
	}
}
