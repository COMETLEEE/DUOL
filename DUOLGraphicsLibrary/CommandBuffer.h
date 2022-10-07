#pragma once

#include "DUOLGraphicsLibrary/PipelineStateFlags.h"

namespace DUOLGraphicsLibrary
{
	class Buffer;
	class BufferArray;
	class PipelineState;

	class CommandBuffer
	{

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

		virtual void SetPipelineState(PipelineState* pipelineState);

		/*----  ----*/
		/*----  ----*/
		/*----  ----*/
		/*----  ----*/



	};
}

