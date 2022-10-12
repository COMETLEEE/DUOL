#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"

#include "DUOLGraphicsLibrary/RendererFlags.h"
#include "DUOLGraphicsLibrary/Renderer/RenderContext.h"

#include "DUOLGraphicsLibrary/Renderer/Buffer.h"
#include "DUOLGraphicsLibrary/BufferFlags.h"

#include "DUOLGraphicsLibrary/Renderer/BufferArray.h"

#include "DUOLGraphicsLibrary/Renderer/Texture.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"

#include "DUOLGraphicsLibrary/Renderer/Sampler.h"
#include "DUOLGraphicsLibrary/SamplerFlags.h"

#include "DUOLGraphicsLibrary/Renderer/Shader.h"
#include "DUOLGraphicsLibrary/ShaderFlags.h"

#include "DUOLGraphicsLibrary/Renderer/RenderTarget.h"
#include "DUOLGraphicsLibrary/RenderTargetFlags.h"

#include "DUOLGraphicsLibrary/Renderer/CommandBuffer.h"

#include <memory>
#include <vector>
//#include <string>

namespace DUOLGraphicsLibrary
{

	class DUOLGRAPHICSLIBRARY_EXPORT Renderer : public EntityBase
	{

		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Renderer);

	private:

	private:

	protected:
	public:

	public:
		static std::unique_ptr<Renderer> CreateRenderer(const RendererDesc& renderDesc);

		static void DeleteRenderer(std::unique_ptr<Renderer>&& renderer);

		/*---- Device & Context ----*/
		virtual RenderContext* CreateRenderContext(const RenderContextDesc& renderContextDesc) abstract;

		virtual bool Release(RenderContext* renderContext) abstract;
		/*---- CommnadBuffer ----*/
		virtual CommandBuffer* CreateCommandBuffer() abstract;

		virtual bool Release(CommandBuffer* commandBuffer) abstract; 

		/*---- Buffer ----*/
		virtual Buffer* CreateBuffer(const BufferDesc& desc, const void* initialData = nullptr) abstract;

		virtual bool Release(BufferDesc* renderContext) abstract;

		virtual void WriteBuffer(Buffer& buffer, const void* data, int dataSize, int bufferStartOffset) abstract;

		virtual void MapBuffer(Buffer& buffer) abstract;

		virtual void UnmapBuffer(Buffer& buffer) abstract;

		/*---- BufferArray ----*/
		virtual BufferArray* CreateBuffer(int bufferCount, Buffer* buffers) abstract;

		virtual bool Release(BufferArray* bufferArray) abstract;

		/*---- Texture ----*/
		virtual Texture* CreateTexture(const TextureDesc& textureDesc) abstract;

		virtual bool Release(Texture& texture) abstract;

		virtual bool WriteTexture(Texture& texture) abstract;

		virtual bool ReadTexture(Texture& texture) abstract;

		/*---- Sampler ----*/
		virtual Sampler* CreateSampler(const SamplerDesc& samplerDesc) abstract;

		virtual bool Release(Sampler& texture) abstract;

		/*---- Shader ----*/
		virtual Shader* CreateShader(const ShaderDesc& shaderDesc) abstract;

		virtual bool Release(Shader& renderTarget) abstract;

		/*---- RenderTarget ----*/
		virtual RenderTarget* CreateRenderTarget(const RenderTargetDesc& rendertargetDesc) abstract;

		virtual bool Release(RenderTarget& renderTarget) abstract;

		/*----   ----*/


	};
}

