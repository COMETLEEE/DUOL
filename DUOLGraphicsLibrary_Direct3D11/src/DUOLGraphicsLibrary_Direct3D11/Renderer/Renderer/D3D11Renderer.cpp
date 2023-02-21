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
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Sampler.h"
#include "DUOLGraphicsLibrary_Direct3D11/FontEngine/FontEngine.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Buffer/D3D11BufferWithRV.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/PipelineState/D3D11ComputePipelineState.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/PipelineState/D3D11GraphicsPipelineState.h"

namespace DUOLGraphicsLibrary
{
	D3D11Renderer::D3D11Renderer(const RendererDesc& rendererDesc) :
		Renderer(rendererDesc)
	{
		CreateFactory();
		QueryAdapters();
		CreateDevice();

		HRESULT hr = _D3D11Context->QueryInterface(__uuidof(_debugEvent), &_debugEvent);
	}

	D3D11Renderer::~D3D11Renderer()
	{
		_D3D11Context->ClearState();

		_D3D11RenderContexts.reset();
		_D3D11CommandBuffers.clear();
		_D3D11Shaders.clear();
		_D3D11Buffers.clear();
		_D3D11Sampler.clear();
		_D3D11PipelineStates.clear();
		_D3D11RenderTargets.clear();


#if defined(DEBUG) || defined(_DEBUG)  
		ComPtr<ID3D11Debug> Debug;

		_D3D11Device->QueryInterface(Debug.GetAddressOf());

		OutputDebugStringA("-------누수 오브젝트 목록입니다--------\r\n");
		Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
		OutputDebugStringA("-------------------------------------\r\n");
#endif
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
		createDeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
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
		_D3D11RenderContexts = std::make_unique<D3D11RenderContext>(0, _D3D11Factory, _D3D11Device, _D3D11Context, renderContextDesc, _rendererDesc);

		return _D3D11RenderContexts.get();
	}

	ModuleInfo D3D11Renderer::GetModuleInfo()
	{
		ModuleInfo info;

		info._device = _D3D11Device.Get();
		info._deviceContext = _D3D11Context.Get();
		info._moduleType = ModuleType::DIRECTX11;

		return info;
	}

	bool D3D11Renderer::Release(RenderContext* renderContext)
	{
		return _D3D11RenderContexts.release();
	}

	IFontEngine* D3D11Renderer::GetFontEngine()
	{
		auto ret = _D3D11RenderContexts->GetFontEngine();
		if (ret == nullptr)
			return nullptr;

		return ret;
	}

	CommandBuffer* D3D11Renderer::CreateCommandBuffer(const UINT64& objectID, const CommandBufferDesc& commandBufferDesc)
	{
		if ((commandBufferDesc.flag & (static_cast<long>(CommandBufferFlags::Deferred) | static_cast<long>(CommandBufferFlags::Multi))) != 0)
		{
			ComPtr<ID3D11DeviceContext> deferredContext;
			auto hr = _D3D11Device->CreateDeferredContext(0, deferredContext.ReleaseAndGetAddressOf());
			DXThrowError(hr, "ID3D11DeviceContex Error : CreateCommandBuffer");

			return TakeOwnershipFromUniquePtr(
				objectID,
				_D3D11CommandBuffers,
				std::make_unique<D3D11CommandBuffer>(objectID, _D3D11Device.Get(), deferredContext, commandBufferDesc));
		}
		else
		{
			return TakeOwnershipFromUniquePtr(objectID, _D3D11CommandBuffers, std::make_unique<D3D11CommandBuffer>(objectID, _D3D11Device, _D3D11Context, commandBufferDesc));		//return nullptr;
		}
	}

	bool D3D11Renderer::Release(CommandBuffer* commandBuffer)
	{
		return RemoveFromUniqueMap(_D3D11CommandBuffers, commandBuffer->GetGUID());
	}

	Buffer* D3D11Renderer::CreateBuffer(const UINT64& objectID, const BufferDesc& desc, const void* initialData)
	{
		return TakeOwnershipFromUniquePtr(objectID, _D3D11Buffers, MakeD3D11Buffer(objectID, _D3D11Device.Get(), desc, initialData));
	}

	std::unique_ptr<D3D11Buffer> D3D11Renderer::MakeD3D11Buffer(const UINT64& guid, ID3D11Device* device,
		const BufferDesc& bufferDesc, const void* initialData)
	{
		if((bufferDesc._bindFlags & (static_cast<long>(BindFlags::UNORDEREDACCESS) | static_cast<long>(BindFlags::SHADERRESOURCE))) != 0)
		{
			return 	std::make_unique<D3D11BufferWithRV>(guid, device, bufferDesc, initialData);
		}
		else
		{
			return 	std::make_unique<D3D11Buffer>(guid, device, bufferDesc, initialData);
		}
	}

	bool D3D11Renderer::Release(Buffer* buffer)
	{
		return RemoveFromUniqueMap(_D3D11Buffers, buffer->GetGUID());
	}

	void D3D11Renderer::WriteBuffer(Buffer* buffer, const void* data, int dataSize, int bufferStartOffset)
	{
		auto castedBuffer = TYPE_CAST(D3D11Buffer*, buffer);

		castedBuffer->UpdateSubresource(_D3D11Context.Get(), data, dataSize, bufferStartOffset);
	}

	void* D3D11Renderer::MapBuffer(Buffer* buffer, CPUAccessFlags accessFlag)
	{
		auto castedBuffer = TYPE_CAST(D3D11Buffer*, buffer);
		return castedBuffer->Map(_D3D11Context.Get(), accessFlag);
	}

	void D3D11Renderer::UnmapBuffer(Buffer* buffer)
	{
		auto castedBuffer = TYPE_CAST(D3D11Buffer*, buffer);
		castedBuffer->Unmap(_D3D11Context.Get());
	}

	BufferArray* D3D11Renderer::CreateBufferArray(const UINT64& objectID, int bufferCount, Buffer* buffers)
	{
		return nullptr;
	}

	bool D3D11Renderer::Release(BufferArray* bufferArray)
	{
		return false;
	}

	Texture* D3D11Renderer::CreateTexture(const UINT64& objectID, const TextureDesc& textureDesc)
	{
		return TakeOwnershipFromUniquePtr(objectID, _D3D11Textures, std::make_unique<D3D11Texture>(objectID, _D3D11Device.Get(), textureDesc));
	}

	bool D3D11Renderer::Release(Texture* texture)
	{
		return false;
	}

	bool D3D11Renderer::WriteTexture(Texture* texture)
	{
		return false;
	}

	bool D3D11Renderer::ReadTexture(Texture* texture, const TextureLocation& srcLocation, void* dataBufferPoint, UINT32 dataBufferSize)
	{
		D3D11Texture* castedTexture = TYPE_CAST(D3D11Texture*, texture);

		D3D11NativeTexture copy;
		castedTexture->CreateSubresourceCopyForCPUAccess(_D3D11Device.Get(), _D3D11Context.Get(), copy, D3D11_CPU_ACCESS_READ, srcLocation);

		/* Map subresource for reading */
		const UINT subresource = 0;

		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		auto hr = _D3D11Context->Map(copy._resource.Get(), subresource, D3D11_MAP_READ, 0, &mappedSubresource);
		DXThrowError(hr, "failed to map D3D11 texture copy resource");

		/* Copy host visible resource to CPU accessible resource */
		// Pixel 하나만 가져온다. 타입에 따라 카피할 영역이 달라짐.
		auto& format = texture->GetTextureDesc()._format;
		UINT32 copydataSize = 0;
		switch (format)
		{
		case ResourceFormat::FORMAT_R32G32B32A32_FLOAT:
		{
			copydataSize = 16;
			break;
		}
		case ResourceFormat::FORMAT_R8G8B8A8_UNORM:
		{
			copydataSize = 4;
			break;
		}
		default:
			break;
		}

		if (dataBufferSize < copydataSize)
		{
			copydataSize = dataBufferSize;
		}

		memcpy(dataBufferPoint, mappedSubresource.pData, copydataSize);
		/* Unmap resource */
		_D3D11Context->Unmap(copy._resource.Get(), subresource);

		if (copydataSize == 0)
			return false;
		else
			return true;
	}

	bool D3D11Renderer::GenerateMips(Texture* texture)
	{
		D3D11Texture* castedTexture = TYPE_CAST(D3D11Texture*, texture);

		_D3D11Context->GenerateMips(castedTexture->GetShaderResourceView());

		return true;
	}

	bool D3D11Renderer::SetResolution(Texture* texture, const DUOLMath::Vector2& resolution)
	{
		D3D11Texture* castedTexture = TYPE_CAST(D3D11Texture*, texture);

		castedTexture->SetResolution(_D3D11Device.Get(), resolution);

		return false;
	}

	Sampler* D3D11Renderer::CreateSampler(const UINT64& objectID, const SamplerDesc& samplerDesc)
	{
		return TakeOwnershipFromUniquePtr(objectID, _D3D11Sampler, std::make_unique<D3D11Sampler>(objectID, _D3D11Device.Get(), samplerDesc));
	}

	bool D3D11Renderer::Release(Sampler* sampler)
	{
		return RemoveFromUniqueMap(_D3D11Sampler, sampler->GetGUID());;
	}

	Shader* D3D11Renderer::CreateShader(const UINT64& objectID, const ShaderDesc& shaderDesc)
	{
		return TakeOwnershipFromUniquePtr(objectID, _D3D11Shaders, std::make_unique<D3D11Shader>(objectID, _D3D11Device.Get(), shaderDesc));
	}

	bool D3D11Renderer::Release(Shader* shader)
	{
		return RemoveFromUniqueMap(_D3D11Shaders, shader->GetGUID());;
	}

	RenderTarget* D3D11Renderer::CreateRenderTarget(const UINT64& objectID, const RenderTargetDesc& rendertargetDesc)
	{
		D3D11Texture* castedTexture = TYPE_CAST(D3D11Texture*, rendertargetDesc._texture);
		long textureBindFlags = castedTexture->GetTextureDesc()._bindFlags;

		if ((textureBindFlags & static_cast<long>(BindFlags::RENDERTARGET)) || (textureBindFlags & static_cast<long>(BindFlags::DEPTHSTENCIL)))
		{
			return TakeOwnershipFromUniquePtr(objectID, _D3D11RenderTargets, std::make_unique<D3D11RenderTarget>(objectID, _D3D11Device.Get(), rendertargetDesc));
		}
		else
		{
			return nullptr;
		}
	}

	bool D3D11Renderer::ClearRenderTarget(RenderTarget* renderTarget)
	{
		D3D11RenderTarget* castedRenderTargt = TYPE_CAST(D3D11RenderTarget*, renderTarget);

		castedRenderTargt->ClearRenderTarget(_D3D11Context.Get(), { 0.f,0.f,0.f,0.f });

		return false;
	}

	//bool D3D11Renderer::SetResolution(RenderTarget* renderTarget, const DUOLMath::Vector2& resolution)
	//{
	//	D3D11RenderTarget* castedRenderTargt = TYPE_CAST(D3D11RenderTarget*, renderTarget);

	//	castedRenderTargt->SetResolution(_D3D11Device.Get(), resolution);

	//	return false;
	//}

	bool D3D11Renderer::Release(RenderTarget* renderTarget)
	{
		return RemoveFromUniqueMap(_D3D11RenderTargets, renderTarget->GetGUID());;
	}


	PipelineState* D3D11Renderer::CreatePipelineState(const UINT64& objectID, const PipelineStateDesc& pipelineDesc)
	{
		return TakeOwnershipFromUniquePtr(objectID, _D3D11PipelineStates, std::make_unique<D3D11GraphicsPipelineState>(objectID, _D3D11Device.Get(), pipelineDesc));;
	}

	PipelineState* D3D11Renderer::CreatePipelineState(const UINT64& objectID,
		const ComputePipelineStateDesc& pipelineDesc)
	{
		return TakeOwnershipFromUniquePtr(objectID, _D3D11PipelineStates, std::make_unique<D3D11ComputePipelineState>(objectID, _D3D11Device.Get(), pipelineDesc));;
	}

	bool D3D11Renderer::Release(PipelineStateDesc* pipelineState)
	{
		return false;
	}

	void D3D11Renderer::BeginEvent(const wchar_t* message)
	{
		_debugEvent->BeginEvent(message);
	}

	void D3D11Renderer::EndEvent()
	{
		_debugEvent->EndEvent();
	}
}
