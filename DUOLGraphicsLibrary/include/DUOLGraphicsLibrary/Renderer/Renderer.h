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
#include "DUOLGraphicsLibrary/CommandBufferFlags.h"

#include "DUOLGraphicsLibrary/Renderer/PipelineState.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"

#include "DUOLGraphicsLibrary/Renderer/RenderPass.h"
#include "DUOLGraphicsLibrary/RenderPassFlags.h"

#include <map>
#include <memory>
#include <vector>

namespace DUOLGraphicsLibrary
{
	class Module;
	class Renderer;

 /**

     @class   Renderer
     @brief   그래픽스 파이프라인의 렌더러. 렌더링에 필요한 엔티티를 삭제, 생성한다.
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT Renderer : public EntityBase
	{

		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Renderer);
	public:
		Renderer() = default;

		virtual  ~Renderer() = default;

	private:

	private:

	protected:
		RendererDesc _rendererDesc;

	public:
		static Renderer* CreateRenderer(const RendererDesc& renderDesc);

		static void DeleteRenderer(Renderer* renderer);

		/*---- Device & Context ----*/
		virtual RenderContext* CreateRenderContext(const RenderContextDesc& renderContextDesc) abstract;

		virtual bool Release(RenderContext* renderContext) abstract;
		/*---- CommandBuffer ----*/
  /**
      @brief  commandBuffer 생성 todo: 추후 commandQueue 생성
      @param  commandBufferDesc - 
      @retval                   - 
  **/
		virtual CommandBuffer* CreateCommandBuffer(const CommandBufferDesc& commandBufferDesc) abstract;

		virtual bool Release(CommandBuffer* commandBuffer) abstract;

		/*---- Buffer ----*/
		virtual Buffer* CreateBuffer(const BufferDesc& desc, const void* initialData = nullptr) abstract;

		virtual bool Release(Buffer* buffer) abstract;

		virtual void WriteBuffer(Buffer& buffer, const void* data, int dataSize, int bufferStartOffset) abstract;

		virtual void MapBuffer(Buffer& buffer) abstract;

		virtual void UnmapBuffer(Buffer& buffer) abstract;

		/*---- BufferArray ----*/
		virtual BufferArray* CreateBufferArray(int bufferCount, Buffer* buffers) abstract;

		virtual bool Release(BufferArray* bufferArray) abstract;

		/*---- Texture ----*/
		virtual Texture* CreateTexture(const TextureDesc& textureDesc) abstract;

		virtual bool Release(Texture* texture) abstract;

		virtual bool WriteTexture(Texture& texture) abstract;

		virtual bool ReadTexture(Texture& texture) abstract;

		/*---- Sampler ----*/
		virtual Sampler* CreateSampler(const SamplerDesc& samplerDesc) abstract;

		virtual bool Release(Sampler* sampler) abstract;

		/*---- Shader ----*/
		virtual Shader* CreateShader(const ShaderDesc& shaderDesc) abstract;

		virtual bool Release(Shader* shader) abstract;

		/*---- RenderTarget ----*/
		virtual RenderTarget* CreateRenderTarget(const RenderTargetDesc& rendertargetDesc) abstract;

		virtual bool Release(RenderTarget& renderTarget) abstract;

		/*---- RenderPass ----*/
		virtual RenderPass* CreateRenderPass(const RenderPassDesc& renderPassDesc) abstract;

		virtual bool Release(RenderPass* renderTarget) abstract;

		/*---- PipelineState  ----*/
		virtual PipelineState* CreatePipelineState(const PipelineStateDesc& pipelineDesc) abstract;

		virtual bool Release(PipelineStateDesc* pipelineState) abstract;

	};
}

