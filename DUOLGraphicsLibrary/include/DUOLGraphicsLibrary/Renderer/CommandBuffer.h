#pragma once
#include "ResourceViewLayout.h"
#include "DUOLGraphicsLibrary/EntityBase.h"
#include "DUOLGraphicsLibrary/CommandBufferFlags.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary/RenderPassFlags.h"

namespace DUOLGraphicsLibrary
{
	class Buffer;
	class BufferArray;
	class PipelineState;
	class Resource;
	class RenderTarget;
	class RenderPass;

 /**

     @class   CommandBuffer
     @brief   ������ ��� ���� ���� Ȥ�� ���� ������ ���� Ŭ����
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT CommandBuffer : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::CommandBuffer);

	public:
		virtual void Begin() abstract;

		virtual void End() abstract;

		/*dest = destination*/
		virtual void UpdateBuffer(Buffer* destBuffer, int destBufferOffset, const void* data, int dataSize) abstract;

		/*dest = destination*/
		virtual void CopyBuffer(Buffer* destBuffer, int destOffset, Buffer* srcBuffer, int srcOffset) abstract;

		/*---- Viewport ----*/
		virtual void SetViewport(const Viewport& viewport) abstract;

		virtual void SetScissorRect() abstract;

		/*---- Input Assembler ----*/
		virtual void SetVertexBuffer(Buffer* buffer) abstract;

		virtual void SetVertexBufferArray(BufferArray* bufferArray) abstract;

		virtual void SetIndexBuffer(Buffer* buffer) abstract;

		/*---- Resource ----*/
		virtual void SetResource(Resource* resource, unsigned int slot, long bindFlags, long stageFlag) abstract;

		virtual void BindResources(const ResourceViewLayout& resourceViewLayout) abstract;

		/*---- Pipeline State  ----*/
		virtual void SetPipelineState(PipelineState* pipelineState) abstract;

		virtual void SetBlendFactor(const float* color) abstract;

		/*---- RenderTarget ----*/
		virtual void SetRenderTarget(RenderTarget* renderTarget, unsigned int slot) abstract;

		virtual void SetRenderPass(RenderPass* renderPass) abstract;

		/*---- Draw ----*/
		virtual void Draw(int numVertices, int startVertexLocation) abstract;

		virtual void DrawIndexed(int numIndices, int startIndexLoaction ,int baseVertexLocation) abstract;

		virtual void DrawIndexedInstanced(int numIndices, int numInstance, int startIndexLocation, int startVertexLocation, int startInstanceLocation) abstract;

		virtual void DrawInstanced(int numVertices, int startVertexLocation) abstract;

		/*---- Compute ----*/
		//todo : �ϴ� ���߿� ����.
		virtual void Dispatch(){ /* dummy */ };

	};
}

