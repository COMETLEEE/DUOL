#include "D3D11CommandBuffer.h"
#include "DUOLGraphicsLibrary/Core/TypeCast.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Texture.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Buffer/D3D11Buffer.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Sampler.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11RenderContext.h"

namespace DUOLGraphicsLibrary
{
	D3D11CommandBuffer::D3D11CommandBuffer(
		const ComPtr<ID3D11Device>& device
		, D3D11RenderContext* context
		, const CommandBufferDesc& renderContextDesc) :
		_device(device)
		, _context(context)
	{
	}

	void D3D11CommandBuffer::SetBuffer(Buffer* buffer, unsigned slot, long bindFlags, long stageFlags)
	{
		auto castedBuffer = TYPE_CAST(D3D11Buffer*, buffer);

		if((bindFlags & static_cast<long>(BindFlags::CONSTANTBUFFER)) != 0)
		{
			ID3D11Buffer* constantBuffer = castedBuffer->GetNativeBuffer();
			_context->SetConstantBuffers(slot, 1, &constantBuffer, stageFlags);
		}

		if ((bindFlags & static_cast<long>(BindFlags::SHADERRESOURCE)) != 0)
		{
			ID3D11Buffer* constantBuffer = castedBuffer->GetNativeBuffer();
		}

		if ((bindFlags & static_cast<long>(BindFlags::UNRODERED)) != 0)
		{
			ID3D11Buffer* constantBuffer = castedBuffer->GetNativeBuffer();
		}
	}

	void D3D11CommandBuffer::SetTexture(Texture* texture, unsigned slot, long bindFlags, long stageFlags)
	{
		auto castedTexture = TYPE_CAST(D3D11Texture*, texture);

		if ((bindFlags & static_cast<long>(BindFlags::SHADERRESOURCE)) != 0)
		{
			//ID3D11ShaderResourceView* srv = castedTexture.GetShaderResourceView();
			//_context->SetShaderResources(slot, 1, srv, stageFlags);
		}

		/* Set texture UAV to all shader stages */
		if ((bindFlags & static_cast<long>(BindFlags::UNRODERED)) != 0)
		{
			//ID3D11UnorderedAccessView* uav[] = { textureD3D.GetUAV() };
			//UINT auvCounts[] = { 0 };
			//stateMngr_->SetUnorderedAccessViews(slot, 1, uav, auvCounts, stageFlags);
		}
	}

	void D3D11CommandBuffer::SetSampler(Sampler* sampler, unsigned slot, long stageFlags)
	{
	}

	void D3D11CommandBuffer::Begin()
	{
	}

	void D3D11CommandBuffer::End()
	{
	}

	void D3D11CommandBuffer::UpdateBuffer(Buffer* destBuffer, int destBufferOffset, const void* data, int dataSize)
	{
		auto castedBuffer =  TYPE_CAST(D3D11Buffer*, destBuffer);
		castedBuffer->UpdateSubresource(_context->GetNativeContext(), data, dataSize, destBufferOffset);
	}

	void D3D11CommandBuffer::CopyBuffer(Buffer* destBuffer, int destOffset, Buffer* srcBuffer, int srcOffset)
	{
		//todo
	}

	void D3D11CommandBuffer::SetViewport(const Viewport& viewport)
	{
		_context->SetViewports(1, &viewport);
	}

	void D3D11CommandBuffer::SetScissorRect()
	{
	}

	void D3D11CommandBuffer::SetVertexBuffer(Buffer* buffer)
	{
		auto context = _context->GetNativeContext();

		auto* castedBuffer = TYPE_CAST(D3D11Buffer*, buffer);
		ID3D11Buffer* d3dbuffer = castedBuffer->GetNativeBuffer();
		UINT stride = castedBuffer->GetStride();
		UINT offest = 0;

		context->IASetVertexBuffers(0, 1, &d3dbuffer, &stride, &offest);
	}

	void D3D11CommandBuffer::SetVertexBufferArray(BufferArray* bufferArray)
	{
		//todo
	}

	void D3D11CommandBuffer::SetIndexBuffer(Buffer* buffer)
	{
		auto context = _context->GetNativeContext();

		auto* castedBuffer = TYPE_CAST(D3D11Buffer*, buffer);
		ID3D11Buffer* d3dbuffer = castedBuffer->GetNativeBuffer();;
		DXGI_FORMAT format = castedBuffer->GetFormat();

		context->IASetIndexBuffer(d3dbuffer, format, 0);
	}

	void D3D11CommandBuffer::SetResource(Resource* resource, unsigned slot, long bindFlags, long stageFlags)
	{
		switch (resource->GetResourceType())
		{
		case ResourceType::UNDEFINED:
			break;
		case ResourceType::BUFFER:
			SetBuffer(TYPE_CAST(Buffer*, resource), slot, bindFlags, stageFlags);
			break;
		case ResourceType::TEXTURE:
			SetTexture(TYPE_CAST(Texture*, resource), slot, bindFlags, stageFlags);
			break;
		case ResourceType::SAMPLER:
			SetSampler(TYPE_CAST(Sampler*, resource), slot, stageFlags);
			break;
		}
	}

	void D3D11CommandBuffer::SetPipelineState(PipelineState* pipelineState)
	{
	}

	void D3D11CommandBuffer::SetBlendFactor(const float* color)
	{
	}

	void D3D11CommandBuffer::Draw(int numVertices, int startVertexLocation)
	{
	}

	void D3D11CommandBuffer::DrawIndexed(int numIndices, int startIndexLoaction, int baseVertexLocation)
	{
	}

	void D3D11CommandBuffer::DrawIndexedInstanced(int numIndices, int numInstance, int startIndexLocation,
		int startVertexLocation, int startInstanceLocation)
	{
	}

	void D3D11CommandBuffer::DrawInstanced(int numVertices, int startVertexLocation)
	{
	}
}
