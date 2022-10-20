#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/RenderTarget/D3D11RenderTarget.h"
#include "DUOLGraphicsLibrary/ShaderFlags.h"

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


		_backbufferRenderTargetView = std::make_unique<D3D11RenderTarget>(_device.Get(), _backbufferTexture.Get(), RenderTargetType::Color, _colorFormat);

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


		_backbufferDepthStencilView = std::make_unique<D3D11RenderTarget>(_device.Get(), _backbufferDepthStencilTexture.Get(), RenderTargetType::DepthStencil, _depthStencilFormat);
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

	void D3D11RenderContext::SetViewports(std::uint32_t numViewports, const Viewport* viewportArray)
	{
		_context->RSSetViewports(numViewports, reinterpret_cast<const D3D11_VIEWPORT*>(viewportArray));

	}

	void D3D11RenderContext::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology)
	{
		if (_inputAssemblyState._primitiveTopology != primitiveTopology)
		{
			_inputAssemblyState._primitiveTopology = primitiveTopology;
			_context->IASetPrimitiveTopology(primitiveTopology);
		}
	}

	void D3D11RenderContext::SetInputLayout(ID3D11InputLayout* inputLayout)
	{
		if (_inputAssemblyState._inputLayout != inputLayout)
		{
			_inputAssemblyState._inputLayout = inputLayout;
			_context->IASetInputLayout(inputLayout);
		}
	}

	void D3D11RenderContext::SetVertexShader(ID3D11VertexShader* shader)
	{
		if (_shaderState._vertexShader != shader)
		{
			_shaderState._vertexShader = shader;
			_context->VSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11RenderContext::SetHullShader(ID3D11HullShader* shader)
	{
		if (_shaderState._hullShader != shader)
		{
			_shaderState._hullShader = shader;
			_context->HSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11RenderContext::SetDomainShader(ID3D11DomainShader* shader)
	{
		if (_shaderState._domainShader != shader)
		{
			_shaderState._domainShader = shader;
			_context->DSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11RenderContext::SetGeometryShader(ID3D11GeometryShader* shader)
	{
		if (_shaderState._geometryShader != shader)
		{
			_shaderState._geometryShader = shader;
			_context->GSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11RenderContext::SetPixelShader(ID3D11PixelShader* shader)
	{
		if (_shaderState._pixelShader != shader)
		{
			_shaderState._pixelShader = shader;
			_context->PSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11RenderContext::SetComputeShader(ID3D11ComputeShader* shader)
	{
		if (_shaderState._computeShader != shader)
		{
			_shaderState._computeShader = shader;
			_context->CSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11RenderContext::SetRasterizerState(ID3D11RasterizerState* rasterizerState)
	{
		if (_renderState._rasterizerState != rasterizerState)
		{
			_renderState._rasterizerState = rasterizerState;
			_context->RSSetState(rasterizerState);
		}
	}

	void D3D11RenderContext::SetDepthStencilState(ID3D11DepthStencilState* depthStencilState)
	{
		if (_renderState._depthStencilState != depthStencilState)
		{
			_renderState._depthStencilState = depthStencilState;
			_context->OMSetDepthStencilState(depthStencilState, _renderState._stencilRef);
		}
	}

	void D3D11RenderContext::SetDepthStencilState(ID3D11DepthStencilState* depthStencilState, UINT stencilRef)
	{
		if (_renderState._depthStencilState != depthStencilState || _renderState._stencilRef != stencilRef)
		{
			_renderState._depthStencilState = depthStencilState;
			_renderState._stencilRef = stencilRef;
			_context->OMSetDepthStencilState(depthStencilState, _renderState._stencilRef);
		}
	}

	void D3D11RenderContext::SetStencilRef(UINT stencilRef)
	{
		if (_renderState._stencilRef != stencilRef)
		{
			_renderState._stencilRef = stencilRef;
			_context->OMSetDepthStencilState(_renderState._depthStencilState, _renderState._stencilRef);
		}
	}

	void D3D11RenderContext::SetBlendState(ID3D11BlendState* blendState, UINT sampleMask)
	{
		if (_renderState._blendState != blendState || _renderState._sampleMask != sampleMask)
		{
			_renderState._blendState = blendState;
			_renderState._sampleMask = sampleMask;
			_context->OMSetBlendState(_renderState._blendState, _renderState._blendFactor, _renderState._sampleMask);
		}
	}

	void D3D11RenderContext::SetBlendState(ID3D11BlendState* blendState, const FLOAT* blendFactor, UINT sampleMask)
	{
		if (_renderState._blendState != blendState)
		{
			_renderState._blendState = blendState;
			_renderState._blendFactor[0] = blendFactor[0];
			_renderState._blendFactor[1] = blendFactor[1];
			_renderState._blendFactor[2] = blendFactor[2];
			_renderState._blendFactor[3] = blendFactor[3];
			_renderState._sampleMask = sampleMask;
			_context->OMSetBlendState(_renderState._blendState, _renderState._blendFactor, _renderState._sampleMask);
		}
	}

	void D3D11RenderContext::SetBlendFactor(const FLOAT* blendFactor)
	{
	}

	void D3D11RenderContext::SetConstantBuffers(UINT startSlot, UINT bufferCount, ID3D11Buffer* const* buffers, long stageFlags)
	{
		if (static_cast<long>(ShaderType::VERTEX) & stageFlags)
		{
				_context->VSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::HULL) & stageFlags)
		{
			_context->HSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::DOMAINS) & stageFlags)
		{
			_context->DSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::GEOMETRY) & stageFlags)
		{
			_context->GSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::PIXEL) & stageFlags)
		{
			_context->PSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::COMPUTE) & stageFlags)
		{
			_context->CSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
	}

	void D3D11RenderContext::SetConstantBuffersArray(UINT startSlot, UINT bufferCount, ID3D11Buffer* const* buffers, const UINT* firstConstants, const UINT* numConstants, long stageFlags)
	{
		if (static_cast<long>(ShaderType::VERTEX) & stageFlags)
		{
			_context->VSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::HULL) & stageFlags)
		{
			_context->HSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::DOMAINS) & stageFlags)
		{
			_context->DSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::GEOMETRY) & stageFlags)
		{
			_context->GSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::PIXEL) & stageFlags)
		{
			_context->PSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::COMPUTE) & stageFlags)
		{
			_context->CSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
	}

	void D3D11RenderContext::SetShaderResources(UINT startSlot, UINT resourceCount, ID3D11ShaderResourceView* const* views, long stageFlags)
	{
		if (static_cast<long>(ShaderType::VERTEX) & stageFlags)
		{
			_context->VSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::HULL) & stageFlags)
		{
			_context->HSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::DOMAINS) & stageFlags)
		{
			_context->DSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::GEOMETRY) & stageFlags)
		{
			_context->GSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::PIXEL) & stageFlags)
		{
			_context->PSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::COMPUTE) & stageFlags)
		{
			_context->CSSetShaderResources(startSlot, resourceCount, views);
		}
	}

	void D3D11RenderContext::SetSamplers(UINT startSlot, UINT samplerCount, ID3D11SamplerState* const* samplers,
		long stageFlags)
	{
		if (static_cast<long>(ShaderType::VERTEX) & stageFlags)
		{
			_context->VSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::HULL) & stageFlags)
		{
			_context->HSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::DOMAINS) & stageFlags)
		{
			_context->DSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::GEOMETRY) & stageFlags)
		{
			_context->GSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::PIXEL) & stageFlags)
		{
			_context->PSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::COMPUTE) & stageFlags)
		{
			_context->CSSetSamplers(startSlot, samplerCount, samplers);
		}
	}

	D3D11RenderContext::D3D11RenderContext(
		const ComPtr<IDXGIFactory>& factory
		, const ComPtr<ID3D11Device>& device
		, const ComPtr<ID3D11DeviceContext>& context
		, const RenderContextDesc& contextDesc
		, const RendererDesc& rendererDesc) :
		RenderContext(contextDesc._screenDesc, contextDesc._frameRate)
		, _colorFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
		, _multiSampleDesc{ 1u, 0u }
		, _device(device)
		, _context(context)
	{

		CreateSwapChain(factory, rendererDesc);
		CreateBackBuffer();
	}
}
