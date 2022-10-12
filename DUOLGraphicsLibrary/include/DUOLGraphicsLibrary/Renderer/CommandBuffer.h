#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"

namespace DUOLGraphicsLibrary
{
	class Buffer;
	class BufferArray;
	class PipelineState;

	class CommandBuffer : public EntityBase
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

		/*---- Pipeline State  ----*/

		virtual void SetPipelineState(PipelineState* pipelineState) abstract;

		virtual void SetBlendFactor(const float* color) abstract;

		/*---- Draw ----*/

		virtual void Draw(int numVertices, int startVertexLocation) abstract;

		virtual void DrawIndexed(int numIndices, int startIndexLoaction ,int baseVertexLocation) abstract;

		virtual void DrawIndexedInstanced(int numIndices, int numInstance, int startIndexLocation, int startVertexLocation, int startInstanceLocation) abstract;

		virtual void DrawInstanced(int numVertices, int startVertexLocation);

		/*---- Compute ----*/

		//todo : 일단 나중에 하자.
		virtual void Dispatch(){ /* dummy */ };

	};
}

