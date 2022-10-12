#include "D3D11Renderer.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"

namespace DUOLGraphicsLibrary
{
	D3D11Renderer::D3D11Renderer()
	{
		CreateFactory();
		QueryAdapters();
		CreateDevice();
	}

	D3D11Renderer::~D3D11Renderer()
	{
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
		return nullptr;
	}

	bool D3D11Renderer::Release(RenderContext* renderContext)
	{
		return false;
	}

	CommandBuffer* D3D11Renderer::CreateCommandBuffer()
	{
		return nullptr;
	}

	bool D3D11Renderer::Release(CommandBuffer* commandBuffer)
	{
		return false;
	}

	Buffer* D3D11Renderer::CreateBuffer(const BufferDesc& desc, const void* initialData)
	{
		return nullptr;
	}

	bool D3D11Renderer::Release(BufferDesc* renderContext)
	{
		return false;
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

	BufferArray* D3D11Renderer::CreateBuffer(int bufferCount, Buffer* buffers)
	{
		return nullptr;
	}

	bool D3D11Renderer::Release(BufferArray* bufferArray)
	{
		return false;
	}

	Texture* D3D11Renderer::CreateTexture(const TextureDesc& textureDesc)
	{
		return nullptr;
	}

	bool D3D11Renderer::Release(Texture& texture)
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
		return nullptr;
	}

	bool D3D11Renderer::Release(Sampler& texture)
	{
		return false;
	}

	Shader* D3D11Renderer::CreateShader(const ShaderDesc& shaderDesc)
	{
		return nullptr;
	}

	bool D3D11Renderer::Release(Shader& renderTarget)
	{
		return false;
	}

	RenderTarget* D3D11Renderer::CreateRenderTarget(const RenderTargetDesc& rendertargetDesc)
	{
		return nullptr;
	}

	bool D3D11Renderer::Release(RenderTarget& renderTarget)
	{
		return false;
	}
}
