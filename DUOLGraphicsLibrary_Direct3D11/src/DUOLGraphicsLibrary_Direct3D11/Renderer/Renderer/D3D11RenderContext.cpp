#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"

namespace DUOLGraphicsLibrary
{
	void D3D11RenderContext::CreateSwapChain(const ComPtr<IDXGIFactory>& factory, const RendererDesc& rendererDesc)
	{

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		{
			ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
			//�ĸ� ������ �Ӽ���
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //�� ������ �뵵�� �����ΰ�? (�ĸ� ���ۿ� �������� �ϴ� �뵵(����ü��))
			swapChainDesc.BufferCount = 1;  //����ü�ο� ���� ������ ���� �Ϲ������� �ĸ�����ϳ�(���߹��۸�), ���߹��۸��� ������ �� �� �ִ�.
			swapChainDesc.OutputWindow = reinterpret_cast<HWND>(rendererDesc._handle); // ������ ����� ǥ���� â�� �ڵ�
			swapChainDesc.Windowed = true; // â���ȿ��
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;

			//��������Ʈ�� �÷��׵��� �߰� ������ �� �ִ�. ���⿣ ���Ե��� ����.
			swapChainDesc.BufferDesc.Width = _screenDesc._screenSize.first; // ��ũ����ũ��
			swapChainDesc.BufferDesc.Height = _screenDesc._screenSize.second;
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

		hr = _device->CreateRenderTargetView(_backbufferTexture.Get(), nullptr, _backbufferRenderTargetView.ReleaseAndGetAddressOf());
		DXThrowError(hr, "D3D11RenderContext : CreateBackBuffer Failed, CreateRenderTargetView");

		D3D11_TEXTURE2D_DESC textureDesc;
		{
			textureDesc.Width = _screenDesc._screenSize.first;
			textureDesc.Height = _screenDesc._screenSize.second;
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

		hr = _device->CreateDepthStencilView(_backbufferDepthStencilTexture.Get(), nullptr, _backbufferDepthStencilView.ReleaseAndGetAddressOf());
		DXThrowError(hr, "D3D11RenderContext : CreateBackBuffer Failed, CreateDepthStencil");
	}

	void D3D11RenderContext::ResizeBackBuffer()
	{
		_context->OMSetRenderTargets(0, nullptr, nullptr);

		_backbufferTexture.Reset();
		_backbufferRenderTargetView.Reset();
		_backbufferDepthStencilTexture.Reset();
		_backbufferDepthStencilView.Reset();

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

	D3D11RenderContext::D3D11RenderContext(
		const ComPtr<IDXGIFactory>& factory
		, const ComPtr<ID3D11Device>& device
		, const ComPtr<ID3D11DeviceContext>& context
		, const RenderContextDesc& contextDesc
		, const RendererDesc& rendererDesc) :
		RenderContext(contextDesc._screenDesc, contextDesc._frameRate)
		, _colorFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
		, _multiSampleDesc{ 1u, 0u }\
	{

	}
}
