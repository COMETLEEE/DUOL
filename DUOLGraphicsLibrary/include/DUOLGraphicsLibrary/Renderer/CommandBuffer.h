#pragma once
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"

#include "DUOLGraphicsLibrary/EntityBase.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary/CommandBufferFlags.h"
#include "DUOLGraphicsLibrary/RenderPassFlags.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"
#include "DUOLGraphicsLibrary/RendererFlags.h"


namespace DUOLGraphicsLibrary
{
	class Buffer;
	class BufferArray;
	class PipelineState;
	class Resource;
	class RenderTarget;
	struct RenderPass;

	/**

		@class   CommandBuffer
		@brief   ������ ��� ���� ���� Ȥ�� ���� ������ ���� Ŭ����
		@details ~
		@author  KyungMin Oh

	**/
	class DUOLGRAPHICSLIBRARY_EXPORT CommandBuffer : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::CommandBuffer);

	protected:
		CommandBuffer(const UINT64& guid) :
			EntityBase(guid)
		{

		}

	public:
		virtual void Begin() abstract;

		virtual void Flush() abstract;

		virtual void ClearState() abstract;

		virtual void End() abstract;

		/*dest = destination*/
		virtual void UpdateBuffer(Buffer* destBuffer, int destBufferOffset, const void* data, int dataSize) abstract;

		/*dest = destination*/
		virtual void CopyBuffer(Buffer* destBuffer, int destOffset, Buffer* srcBuffer, int srcOffset, int size) abstract;

		virtual void CopyTexture(Texture* destTexture, const TextureLocation& destOption, Texture* srcTexture, const TextureLocation& srcOption) abstract;

		/*---- Viewport ----*/
		virtual void SetViewport(const Viewport& viewport) abstract;

		virtual void SetScissorRect() abstract;

		/*---- Input Assembler ----*/
		virtual void SetVertexBuffer(Buffer* buffer) abstract;

		virtual void SetVertexBufferArray(BufferArray* bufferArray) abstract;

		virtual void SetIndexBuffer(Buffer* buffer) abstract;

		/*---- Resource ----*/
		virtual void SetResource(Resource* resource, unsigned int slot, long bindFlags, long stageFlag, unsigned initCount = -1) abstract;

		virtual void SetResources(const ResourceViewLayout& resourceViewLayout) abstract;

		/*---- Shader ----*/
		virtual void SetShader(Shader* shader) abstract;

		/*---- Pipeline State  ----*/
		virtual void SetPipelineState(PipelineState* pipelineState) abstract;

		virtual void SetBlendFactor(const float* color) abstract;

		/*---- RenderTarget ----*/
		virtual void SetRenderTarget(RenderTarget* renderTarget, RenderTarget* depth, unsigned int slot) abstract;

		virtual void SetRenderPass(RenderPass* renderPass) abstract;

		/*---- Draw ----*/
		virtual void Draw(int numVertices, int startVertexLocation) abstract;

		virtual void DrawIndexed(int numIndices, int startIndexLoaction, int baseVertexLocation) abstract;

		virtual void DrawIndexedInstanced(int numIndices, int numInstance, int startIndexLocation, int startVertexLocation, int startInstanceLocation) abstract;

		virtual void DrawInstanced(int numVertices, int startVertexLocation) abstract;

		virtual void DrawAuto() abstract;

		/*---- Geometry ----*/
		virtual void BeginStreamOutput(int numBuffers, Buffer* const* buffers) abstract;

		virtual void EndStreamOutput() abstract;

		/*---- Compute ----*/
		//todo : �ϴ� ���߿� ����.
		virtual void Dispatch(int WorkGroupsX, int WorkGroupsY, int WorkGroupsZ) abstract;

		/*---- Debug ----*/
		/**
		@brief Begin�� End ������ �����͸� ����մϴ�. 
		@param  outData - output Data �Դϴ�
		@retval ���������� ���� true�� false�� ��ȯ�մϴ�.
		**/
		virtual bool GetData(QueryInfo& outData) abstract;

	};
}

