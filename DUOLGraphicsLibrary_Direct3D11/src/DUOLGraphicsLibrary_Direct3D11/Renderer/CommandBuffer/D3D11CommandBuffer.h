#pragma once
#include "DUOLGraphicsLibrary/Renderer/CommandBuffer.h"
#include "DUOLGraphicsLibrary/Renderer/Sampler.h"
#include "DUOLGraphicsLibrary/Renderer/Texture.h"
#include "DUOLGraphicsLibrary/Renderer/RenderTarget.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"
#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"

namespace DUOLGraphicsLibrary
{
	class D3D11RenderContext;

	class D3D11CommandBuffer : public CommandBuffer
	{

	public:
		D3D11CommandBuffer(
			const ComPtr<ID3D11Device>& device
			, D3D11RenderContext* context
			, const CommandBufferDesc& renderContextDesc
		);

	private:
		ComPtr<ID3D11Device> _device;
		ID3D11DeviceContext* _d3dContext;
		D3D11RenderContext* _context;

	private:
		void SetBuffer(Buffer* buffer, unsigned slot, long bindFlags, long stageFlags);

		void SetTexture(Texture* texture, unsigned slot, long bindFlags, long stageFlags);

		void SetSampler(Sampler* sampler, unsigned slot, long stageFlags);

	public:

		virtual void Begin() override;

		virtual void End() override;

		/*dest = destination*/
		virtual void UpdateBuffer(Buffer* destBuffer, int destBufferOffset, const void* data, int dataSize) override;

		/*dest = destination*/
		virtual void CopyBuffer(Buffer* destBuffer, int destOffset, Buffer* srcBuffer, int srcOffset) override;

		/*---- Viewport ----*/

		virtual void SetViewport(const Viewport& viewport) override;

		virtual void SetScissorRect() override;

		/*---- Input Assembler ----*/

		virtual void SetVertexBuffer(Buffer* buffer) override;

		virtual void SetVertexBufferArray(BufferArray* bufferArray) override;

		virtual void SetIndexBuffer(Buffer* buffer) override;

		/*---- Resource ----*/
		virtual void SetResource(Resource* resource, unsigned int slot, long bindFlags, long stageFlag) override;

		virtual void BindResources(const ResourceViewLayout& resourceViewLayout) override;

		/*---- Pipeline State  ----*/

		virtual void SetPipelineState(PipelineState* pipelineState) override;

		virtual void SetBlendFactor(const float* color) override;

		/*---- RenderTarget ----*/
		virtual void SetRenderTarget(RenderTarget* renderTarget, unsigned int slot) override;

		virtual void SetRenderPass(RenderPass* renderPass) override;

		/*---- Draw ----*/

		virtual void Draw(int numVertices, int startVertexLocation) override;

		virtual void DrawIndexed(int numIndices, int startIndexLoaction, int baseVertexLocation) override;

		virtual void DrawIndexedInstanced(int numIndices, int numInstance, int startIndexLocation, int startVertexLocation, int startInstanceLocation) override;

		virtual void DrawInstanced(int numVertices, int startVertexLocation) override;

		/*---- Compute ----*/

		//todo : 일단 나중에 하자.
		virtual void Dispatch() { /* dummy */ };
		
	};
}