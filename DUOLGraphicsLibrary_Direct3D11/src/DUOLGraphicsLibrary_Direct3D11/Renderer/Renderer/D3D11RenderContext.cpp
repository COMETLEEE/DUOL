#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/RenderTarget/D3D11RenderTarget.h"
#include "DUOLGraphicsLibrary/ShaderFlags.h"
#include "DUOLGraphicsLibrary_Direct3D11/FontEngine/FontEngine.h"

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

		hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(_backbufferTexture.GetAddressOf()));
		DXThrowError(hr, "D3D11RenderContext : CreateBackBuffer Failed, GetBuffer");
		D3D11_TEXTURE2D_DESC desc;
		_backbufferTexture->GetDesc(&desc);
		_backbufferRenderTargetView->CreateRenderTargetViews(_device.Get(), _backbufferTexture.Get(), RenderTargetType::Color, _colorFormat, {static_cast<float>(desc.Width),static_cast<float>(desc.Height)});
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
		_fontEngine->UnloadBackbuffer();

		//resize �Լ�
		//bufferCount = 0 -> ��������, width, height = 0 -> ��ũ��������, format = unknown -> ��������

		_context->ClearState();
		auto hr = _swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		DXThrowError(hr, "D3D11RenderContext : ResizeBackBuffer Failed, ResizeBuffer");

		CreateBackBuffer();
		_fontEngine->CreateBackbuffer();
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

	IDXGISwapChain* D3D11RenderContext::GetSwapHChain()
	{
		return _swapChain.Get();
	}

	void D3D11RenderContext::Present()
	{
		auto hr= _swapChain->Present(_frameRateDesc._interval, 0);

		if (hr != S_OK)
		{
			HRESULT reason = _device->GetDeviceRemovedReason();
			char outString[100];
			size_t size = 100;
			sprintf_s(outString, size, "Device removed! DXGI_ERROR code: 0x%X\n", reason);

			DXThrowError(reason, outString);
		}
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

	Texture* D3D11RenderContext::GetBackBufferTexture()
	{
		//todo.. ��...
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
		, _colorFormat(DXGI_FORMAT_B8G8R8A8_UNORM)
		, _multiSampleDesc{ 1u, 0u }
		, _device(device)
		, _context(context)
	{
		CreateSwapChain(factory, rendererDesc, contextDesc);
		_backbufferRenderTargetView = std::move(std::make_unique<D3D11RenderTarget>(0));
		CreateBackBuffer();

		ComPtr<IDXGIDevice> dxgiDevice;
		// Obtain the underlying DXGI device of the Direct3D11 device.
		DXThrowError(
			device.As(&dxgiDevice), "fontEngine Create Error : dxgiDevice Get Failed"
		);

		_fontEngine = std::make_unique<FontEngine>(dxgiDevice.Get(), _swapChain.Get(), reinterpret_cast<HWND>(rendererDesc._handle));
	}

	D3D11RenderContext::~D3D11RenderContext()
	{
		_fontEngine.reset();
	}
}
