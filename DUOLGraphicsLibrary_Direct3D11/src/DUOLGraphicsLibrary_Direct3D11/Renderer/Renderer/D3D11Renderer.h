#pragma once

#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

#include <unordered_map>

#include <dxgi.h>


namespace DUOLGraphicsLibrary
{
	class D3D11RenderContext;
	class D3D11CommandBuffer;
	class D3D11Shader;
	class D3D11Buffer;
	class D3D11Texture;
	class D3D11Sampler;
	class D3D11PipelineState;
	class D3D11RenderTarget;

	class  D3D11Renderer : public Renderer
	{
		template <typename T>
		using Container = std::unordered_map<UINT64, std::unique_ptr<T>>;

	public:
		D3D11Renderer(const RendererDesc& rendererDesc);

		virtual ~D3D11Renderer();

	private:
		std::vector<VideoAdapterDesc> _videoAdatperDescs;

		/*---- Common Objects ----*/
		ComPtr<IDXGIFactory> _D3D11Factory;

		ComPtr<ID3D11Device> _D3D11Device;

		ComPtr<ID3D11DeviceContext> _D3D11Context;

		/*----- Created Objects ----*/
		Container<D3D11RenderContext> _D3D11RenderContexts;

		Container<D3D11CommandBuffer> _D3D11CommandBuffers;

		Container<D3D11Shader> _D3D11Shaders;

		Container<D3D11Texture> _D3D11Textures;

		Container<D3D11Sampler> _D3D11Sampler;

		Container<D3D11Buffer> _D3D11Buffers;

		Container<D3D11PipelineState> _D3D11PipelineStates;

		Container<D3D11RenderTarget> _D3D11RenderTargets;

	private:
		void CreateFactory();

		void QueryAdapters();

		void GetVideoAdapterDesc(IDXGIAdapter* adapter);

		void CreateDevice();

	public:

		/*---------- Inheritance ------------*/
		/*---- Device & Context ----*/
		virtual RenderContext* CreateRenderContext(const RenderContextDesc& renderContextDesc) override final;

		virtual bool Release(RenderContext* renderContext) override final;

		/*---- CommnadBuffer ----*/
		virtual CommandBuffer* CreateCommandBuffer(const UINT64& objectID, const CommandBufferDesc& commandBufferDesc) override final;

		virtual bool Release(CommandBuffer* commandBuffer) override final;

		/*---- Buffer ----*/
		virtual Buffer* CreateBuffer(const UINT64& objectID, const BufferDesc& desc, const void* initialData = nullptr) override final;

		virtual bool Release(Buffer* buffer) override final;

		virtual void WriteBuffer(Buffer& buffer, const void* data, int dataSize, int bufferStartOffset) override final;

		virtual void* MapBuffer(Buffer& buffer, CPUAccessFlags accessFlag) override final;

		virtual void UnmapBuffer(Buffer& buffer) override final;

		/*---- BufferArray ----*/
		virtual BufferArray* CreateBufferArray(const UINT64& objectID, int bufferCount, Buffer* buffers) override final;

		virtual bool Release(BufferArray* bufferArray) override final;

		/*---- Texture ----*/
		virtual Texture* CreateTexture(const UINT64& objectID, const TextureDesc& textureDesc) override final;

		virtual bool Release(Texture* texture) override final;

		virtual bool WriteTexture(Texture& texture) override final;

		virtual bool ReadTexture(Texture& texture) override final;

		/*---- Sampler ----*/
		virtual Sampler* CreateSampler(const UINT64& objectID, const SamplerDesc& samplerDesc) override final;

		virtual bool Release(Sampler* sampler) override final;

		/*---- Shader ----*/
		virtual Shader* CreateShader(const UINT64& objectID, const ShaderDesc& shaderDesc) override final;

		virtual bool Release(Shader* shader) override final;

		/*---- RenderTarget ----*/
		virtual RenderTarget* CreateRenderTarget(const UINT64& objectID, const RenderTargetDesc& rendertargetDesc) override final;

		virtual bool ClearRenderTarget(RenderTarget& renderTarget) override final;

		virtual bool SetResolution(RenderTarget& renderTarget, const DUOLMath::Vector2& resolution) override final;

		virtual bool Release(RenderTarget& renderTarget) override final;

		/*---- PipelineState  ----*/
		virtual PipelineState* CreatePipelineState(const UINT64& objectID, const PipelineStateDesc& pipelineDesc) override final;

		virtual bool Release(PipelineStateDesc* pipelineState) override final;

		/*--------------------------*/
	};

}