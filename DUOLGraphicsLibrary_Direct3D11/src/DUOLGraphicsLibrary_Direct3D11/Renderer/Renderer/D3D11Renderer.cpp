#include "D3D11Renderer.h"

#include "D3D11RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"
#include "DUOLGraphicsLibrary/Core/Helper.h"
#include "DUOLGraphicsLibrary/Core/TypeCast.h"

#include "DUOLGraphicsLibrary_Direct3D11/Renderer/CommandBuffer/D3D11CommandBuffer.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Shader/D3D11Shader.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Buffer/D3D11Buffer.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/PipelineState//D3D11PipelineState.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/RenderTarget/D3D11RenderTarget.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Texture.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/RenderPass/D3D11RenderPass.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Sampler.h"


namespace DUOLGraphicsLibrary
{
	D3D11Renderer::D3D11Renderer(const RendererDesc& rendererDesc)
	{
		_rendererDesc = rendererDesc;

		CreateFactory();
		QueryAdapters();
		CreateDevice();
	}

	D3D11Renderer::~D3D11Renderer()
	{
		_D3D11RenderContexts.clear();
		_D3D11CommandBuffers.clear();
		_D3D11Shaders.clear();
		_D3D11Buffers.clear();
		_D3D11PipelineStates.clear();
		_D3D11RenderTargets.clear();
	}

	void D3D11Renderer::CreateFactory()
	{
		DXThrowError(CreateDXGIFactory(IID_PPV_ARGS(&_D3D11Factory)), "D3D11Renderer Error : Create Factory");
	}

	void D3D11Renderer::QueryAdapters()
	{
		ComPtr<IDXGIAdapter> adapter;

		for (int adapterIndex = 0; _D3D11Factory->EnumAdapters(adapterIndex, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; adapterIndex++)
		{
			GetVideoAdapterDesc(adapter.Get());
			adapter.Reset();
		}
	}

	void D3D11Renderer::GetVideoAdapterDesc(IDXGIAdapter* adapter)
	{
		DXGI_ADAPTER_DESC adapterDesc;
		adapter->GetDesc(&adapterDesc);

		VideoAdapterDesc videoAdapterDesc;

		wcsncpy_s(videoAdapterDesc._hardwareName, adapterDesc.Description, 128);
		videoAdapterDesc._videoMemorySize = adapterDesc.DedicatedVideoMemory;

		_videoAdatperDescs.push_back(videoAdapterDesc);
	}

	void D3D11Renderer::CreateDevice()
	{

		auto featureLevel =
#if D3D11_ENABLE_FEATURELEVEL >= 1
			D3D_FEATURE_LEVEL_11_1
#else
			D3D_FEATURE_LEVEL_11_0
#endif
			;

		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL FeatureLevelOut;

		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			createDeviceFlags,
			0, 0,
			D3D11_SDK_VERSION,
			_D3D11Device.ReleaseAndGetAddressOf(),
			&FeatureLevelOut,
			_D3D11Context.ReleaseAndGetAddressOf()
		);

		DXThrowError(hr, "D3D11Renderer Error : Create Device");
	}

	RenderContext* D3D11Renderer::CreateRenderContext(const RenderContextDesc& renderContextDesc)
	{
		//return TakeOwnershipFromUniquePtr(_D3D11RenderContexts, std::make_unique<D3D11RenderContext>(_D3D11Factory, _D3D11Device, _D3D11Context, renderContextDesc, _rendererDesc));
		_D3D11ImmediateContext = std::make_unique<D3D11RenderContext>(_D3D11Factory, _D3D11Device, _D3D11Context, renderContextDesc, _rendererDesc);
		return _D3D11ImmediateContext.get();
	}

	bool D3D11Renderer::Release(RenderContext* renderContext)
	{
		return RemoveFromUniqueSet(_D3D11RenderContexts, renderContext);
	}

	CommandBuffer* D3D11Renderer::CreateCommandBuffer(const CommandBufferDesc& commandBufferDesc)
	{
		return TakeOwnershipFromUniquePtr(_D3D11CommandBuffers, std::make_unique<D3D11CommandBuffer>(_D3D11Device, _D3D11ImmediateContext.get(), commandBufferDesc));		//return nullptr;
	}

	bool D3D11Renderer::Release(CommandBuffer* commandBuffer)
	{
		return RemoveFromUniqueSet(_D3D11CommandBuffers, commandBuffer);
	}

	Buffer* D3D11Renderer::CreateBuffer(const BufferDesc& desc, const void* initialData)
	{
		return TakeOwnershipFromUniquePtr(_D3D11Buffers, std::make_unique<D3D11Buffer>(_D3D11Device.Get(), desc, initialData));
	}

	bool D3D11Renderer::Release(Buffer* buffer)
	{
		return RemoveFromUniqueSet(_D3D11Buffers, buffer);
	}

	void D3D11Renderer::WriteBuffer(Buffer& buffer, const void* data, int dataSize, int bufferStartOffset)
	{
		//Renderer::WriteBuffer(buffer, data, dataSize, bufferStartOffset);
	}

	void D3D11Renderer::MapBuffer(Buffer& buffer)
	{
		//Renderer::MapBuffer(buffer);
	}

	void D3D11Renderer::UnmapBuffer(Buffer& buffer)
	{
		//Renderer::UnmapBuffer(buffer);
	}

	BufferArray* D3D11Renderer::CreateBufferArray(int bufferCount, Buffer* buffers)
	{
		return nullptr;
	}

	bool D3D11Renderer::Release(BufferArray* bufferArray)
	{
		return false;
	}

	Texture* D3D11Renderer::CreateTexture(const TextureDesc& textureDesc)
	{
		return TakeOwnershipFromUniquePtr(_D3D11Textures, std::make_unique<D3D11Texture>(_D3D11Device.Get(), textureDesc));
	}

	bool D3D11Renderer::Release(Texture* texture)
	{
		return false;
	}

	bool D3D11Renderer::WriteTexture(Texture& texture)
	{
		return false;
	}

	bool D3D11Renderer::ReadTexture(Texture& texture)
	{
		return false;
	}

	Sampler* D3D11Renderer::CreateSampler(const SamplerDesc& samplerDesc)
	{
		return TakeOwnershipFromUniquePtr(_D3D11Sampler, std::make_unique<D3D11Sampler>(_D3D11Device.Get(), samplerDesc));
	}

	bool D3D11Renderer::Release(Sampler* sampler)
	{
		return RemoveFromUniqueSet(_D3D11Sampler, sampler);;
	}

	Shader* D3D11Renderer::CreateShader(const ShaderDesc& shaderDesc)
	{
		return TakeOwnershipFromUniquePtr(_D3D11Shaders, std::make_unique<D3D11Shader>(_D3D11Device.Get(), shaderDesc));
	}

	bool D3D11Renderer::Release(Shader* shader)
	{
		return RemoveFromUniqueSet(_D3D11Shaders, shader);;
	}

	RenderTarget* D3D11Renderer::CreateRenderTarget(const RenderTargetDesc& rendertargetDesc)
	{
		D3D11Texture* castedTexture = TYPE_CAST(D3D11Texture*, rendertargetDesc._texture);
		long textureBindFlags = castedTexture->GetBindFlags();

		if ((textureBindFlags & static_cast<long>(BindFlags::DEPTHSTENCIL)) || (textureBindFlags & static_cast<long>(BindFlags::DEPTHSTENCIL)))
		{
			return TakeOwnershipFromUniquePtr(_D3D11RenderTargets, std::make_unique<D3D11RenderTarget>(_D3D11Device.Get(), rendertargetDesc));
		}
		else
		{
			return nullptr;
		}
	}

	bool D3D11Renderer::Release(RenderTarget& renderTarget)
	{
		return false;
	}

	RenderPass* D3D11Renderer::CreateRenderPass(const RenderPassDesc& renderPassDesc)
	{
		return TakeOwnershipFromUniquePtr(_D3D11RenderPasses, std::make_unique<D3D11RenderPass>(renderPassDesc));
	}

	bool D3D11Renderer::Release(RenderPass* renderTarget)
	{
		return false;
	}

	PipelineState* D3D11Renderer::CreatePipelineState(const PipelineStateDesc& pipelineDesc)
	{
		return TakeOwnershipFromUniquePtr(_D3D11PipelineStates, std::make_unique<D3D11PipelineState>(_D3D11Device.Get(), pipelineDesc));;
	}

	bool D3D11Renderer::Release(PipelineStateDesc* pipelineState)
	{
		return false;
	}
}
