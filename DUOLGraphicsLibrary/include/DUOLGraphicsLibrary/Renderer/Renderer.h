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

#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

#include <map>
#include <memory>
#include <vector>


namespace DUOLGraphicsLibrary
{
	class IFontEngine;
	class Module;
	class Renderer;

	//todo : d3d ���ҽ� ��ü�� ������ �����Ҷ�, �޸� �����ϴ� ����� �ξ����. ������û�� ���ҽ��� ������������ ���߿� ���Ҵ��Ѵٴ���...
	//todo : + ��Ÿ �����Ҵ� ��ü�鵵

 /**

     @class   Renderer
     @brief   �׷��Ƚ� ������������ ������. �������� �ʿ��� ��ƼƼ�� ����, �����Ѵ�.
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT Renderer : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Renderer);
	public:
		Renderer(const RendererDesc& rendererDesc):
			EntityBase(0)
			,_rendererDesc(rendererDesc)
		{
			
		}
		virtual  ~Renderer() = default;

	protected:
		RendererDesc _rendererDesc;
		
	public:
		static Renderer* CreateRenderer(const RendererDesc& renderDesc);

		static void DeleteRenderer(Renderer* renderer);

		/*---- Device & Context ----*/
		virtual RenderContext* CreateRenderContext(const RenderContextDesc& renderContextDesc) abstract;

		virtual ModuleInfo GetModuleInfo() abstract;

		virtual IFontEngine* GetFontEngine() abstract;
		
		virtual bool Release(RenderContext* renderContext) abstract;
		/*---- CommandBuffer ----*/
  /**
      @brief  commandBuffer ���� todo: ���� commandQueue ����
      @param  commandBufferDesc - 
      @retval                   - 
  **/
		virtual CommandBuffer* CreateCommandBuffer(const UINT64& objectID, const CommandBufferDesc& commandBufferDesc) abstract;

		virtual bool Release(CommandBuffer* commandBuffer) abstract;

		/*---- Buffer ----*/
		virtual Buffer* CreateBuffer(const UINT64& objectID, const BufferDesc& desc, const void* initialData = nullptr) abstract;

		virtual bool Release(Buffer* buffer) abstract;

		virtual void WriteBuffer(Buffer* buffer, const void* data, int dataSize, int bufferStartOffset) abstract;

		virtual void* MapBuffer(Buffer* buffer, CPUAccessFlags accessFlag) abstract;

		virtual void UnmapBuffer(Buffer* buffer) abstract;

		/*---- BufferArray ----*/
		virtual BufferArray* CreateBufferArray(const UINT64& objectID, int bufferCount, Buffer* buffers) abstract;

		virtual bool Release(BufferArray* bufferArray) abstract;

		/*---- Texture ----*/
		virtual Texture* CreateTexture(const UINT64& objectID, const TextureDesc& textureDesc) abstract;

		virtual bool Release(Texture* texture) abstract;

		virtual bool WriteTexture(Texture* texture) abstract;

		virtual bool ReadTexture(Texture* texture, const TextureLocation& srcLocation, void* dataBufferPoint, UINT32 dataBufferSize) abstract;

		virtual bool GenerateMips(Texture* texture) abstract;

		virtual bool SetResolution(Texture* texture, const DUOLMath::Vector2& resolution) abstract;

		/*---- Sampler ----*/
		virtual Sampler* CreateSampler(const UINT64& objectID, const SamplerDesc& samplerDesc) abstract;

		virtual bool Release(Sampler* sampler) abstract;

		/*---- Shader ----*/
		virtual Shader* CreateShader(const UINT64& objectID, const ShaderDesc& shaderDesc) abstract;

		virtual bool Release(Shader* shader) abstract;

		/*---- RenderTarget ----*/
		virtual RenderTarget* CreateRenderTarget(const UINT64& objectID, const RenderTargetDesc& rendertargetDesc) abstract;

		virtual bool ClearRenderTarget(RenderTarget* renderTarget, const DUOLMath::Vector4& color = {0.f, 0.f, 0.f, 0.f}) abstract;

		virtual bool ClearUnorderedAccessView(Resource* resource, const unsigned* color = nullptr) abstract;

		//deprecated function. texture resize�� �̿��ϼ���!
		//virtual bool SetResolution(RenderTarget* renderTarget, const DUOLMath::Vector2& resolution) abstract;

		virtual bool Release(RenderTarget* renderTarget) abstract;

		///*---- RenderPass ----*/
		//virtual RenderPass* CreateRenderPass(const UINT64& objectID, const RenderPassDesc& renderPassDesc) abstract;

		//virtual bool Release(RenderPass* renderTarget) abstract;

		/*---- PipelineState  ----*/
		virtual PipelineState* CreatePipelineState(const UINT64& objectID, const PipelineStateDesc& pipelineDesc) abstract;

		virtual PipelineState* CreatePipelineState(const UINT64& objectID, const ComputePipelineStateDesc& pipelineDesc) abstract;

		virtual bool Release(PipelineStateDesc* pipelineState) abstract;

		/*---- Debugging ----*/
		virtual void BeginEvent(const wchar_t* message) abstract;

		virtual void EndEvent() abstract;
		
	};
}

