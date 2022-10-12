#pragma once

#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

#include <dxgi.h>


namespace DUOLGraphicsLibrary
{

	class D3D11Renderer : public Renderer
	{

	public:
		D3D11Renderer();

		~D3D11Renderer();

	private:
		std::vector<VideoAdapterDesc> _videoAdatperDescs;

		/*---- Common Objects ----*/
		ComPtr<IDXGIFactory> _D3D11Factory;

		ComPtr<ID3D11Device> _D3D11Device;

		ComPtr<ID3D11DeviceContext> _D3D11Context;


		/*----- Created Objects ----*/

	private:
		void CreateFactory();

		void QueryAdapters();

		void GetVideoAdapterDesc(IDXGIAdapter* adapter);

		void CreateDevice();

	public:

	public:

		/*---------- Inheritance ------------*/
		/*---- Device & Context ----*/
		virtual RenderContext* CreateRenderContext(const RenderContextDesc& renderContextDesc) override final;

		virtual bool Release(RenderContext* renderContext) override final;

		/*---- CommnadBuffer ----*/
		virtual CommandBuffer* CreateCommandBuffer() override final;

		virtual bool Release(CommandBuffer* commandBuffer) override final;

		/*---- Buffer ----*/
		virtual Buffer* CreateBuffer(const BufferDesc& desc, const void* initialData = nullptr) override final;

		virtual bool Release(BufferDesc* renderContext) override final;

		virtual void WriteBuffer(Buffer& buffer, const void* data, int dataSize, int bufferStartOffset) override final;

		virtual void MapBuffer(Buffer& buffer) override final;

		virtual void UnmapBuffer(Buffer& buffer) override final;

		/*---- BufferArray ----*/
		virtual BufferArray* CreateBuffer(int bufferCount, Buffer* buffers) override final;

		virtual bool Release(BufferArray* bufferArray) override final;

		/*---- Texture ----*/
		virtual Texture* CreateTexture(const TextureDesc& textureDesc) override final;

		virtual bool Release(Texture& texture) override final;

		virtual bool WriteTexture(Texture& texture) override final;

		virtual bool ReadTexture(Texture& texture) override final;

		/*---- Sampler ----*/
		virtual Sampler* CreateSampler(const SamplerDesc& samplerDesc) override final;

		virtual bool Release(Sampler& texture) override final;

		/*---- Shader ----*/
		virtual Shader* CreateShader(const ShaderDesc& shaderDesc) override final;

		virtual bool Release(Shader& renderTarget) override final;

		/*---- RenderTarget ----*/
		virtual RenderTarget* CreateRenderTarget(const RenderTargetDesc& rendertargetDesc) override final;

		virtual bool Release(RenderTarget& renderTarget) override final;

		/*--------------------------*/

	};

}