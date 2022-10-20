#include "D3D11CommandBuffer.h"
#include "DUOLGraphicsLibrary/Core/TypeCast.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Texture.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Buffer/D3D11Buffer.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Sampler.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/PipelineState//D3D11PipelineState.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/RenderPass/D3D11RenderPass.h"

namespace DUOLGraphicsLibrary
{
	D3D11CommandBuffer::D3D11CommandBuffer(
		const ComPtr<ID3D11Device>& device
		, D3D11RenderContext* context
		, const CommandBufferDesc& renderContextDesc) :
		_device(device)
		, _context(context)
		, _d3dContext(context->GetNativeContext())
	{
	}

	void D3D11CommandBuffer::SetBuffer(Buffer* buffer, unsigned slot, long bindFlags, long stageFlags)
	{
		auto castedBuffer = TYPE_CAST(D3D11Buffer*, buffer);

		if ((bindFlags & static_cast<long>(BindFlags::CONSTANTBUFFER)) != 0)
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
			ID3D11ShaderResourceView* shaderResourceView = castedTexture->GetShaderResourceView();
			_context->SetShaderResources(slot, 1, &shaderResourceView, stageFlags);
		}

		if ((bindFlags & static_cast<long>(BindFlags::UNRODERED)) != 0)
		{
			//ID3D11UnorderedAccessView* uav[] = { textureD3D.GetUAV() };
			//UINT auvCounts[] = { 0 };
			//stateMngr_->SetUnorderedAccessViews(slot, 1, uav, auvCounts, stageFlags);
		}
	}

	void D3D11CommandBuffer::SetSampler(Sampler* sampler, unsigned slot, long stageFlags)
	{
		auto castedSampler = TYPE_CAST(D3D11Sampler*, sampler);

		ID3D11SamplerState* samplerState = castedSampler->GetNativeSampler();
		_context->SetSamplers(slot, 1, &samplerState, stageFlags);
	}

	void D3D11CommandBuffer::Begin()
	{
	}

	void D3D11CommandBuffer::End()
	{
	}

	void D3D11CommandBuffer::UpdateBuffer(Buffer* destBuffer, int destBufferOffset, const void* data, int dataSize)
	{
		auto castedBuffer = TYPE_CAST(D3D11Buffer*, destBuffer);
		castedBuffer->UpdateSubresource(_d3dContext, data, dataSize, destBufferOffset);
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
		auto* castedBuffer = TYPE_CAST(D3D11Buffer*, buffer);
		ID3D11Buffer* d3dbuffer = castedBuffer->GetNativeBuffer();
		UINT stride = castedBuffer->GetStride();
		UINT offest = 0;

		_d3dContext->IASetVertexBuffers(0, 1, &d3dbuffer, &stride, &offest);
	}

	void D3D11CommandBuffer::SetVertexBufferArray(BufferArray* bufferArray)
	{
		//todo
	}

	void D3D11CommandBuffer::SetIndexBuffer(Buffer* buffer)
	{
		auto* castedBuffer = TYPE_CAST(D3D11Buffer*, buffer);
		ID3D11Buffer* d3dbuffer = castedBuffer->GetNativeBuffer();;
		DXGI_FORMAT format = castedBuffer->GetFormat();

		_d3dContext->IASetIndexBuffer(d3dbuffer, format, 0);
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

	void D3D11CommandBuffer::BindResources(const ResourceViewLayout& resourceViewLayout)
	{
		ResourceViewDesc a(nullptr, 0,0,0);

		for (auto& resouceView : resourceViewLayout._resourceViews)
		{
			switch (resouceView._resource->GetResourceType())
			{
			case ResourceType::UNDEFINED:
				break;
			case ResourceType::BUFFER:
				SetBuffer(TYPE_CAST(Buffer*, resouceView._resource), resouceView._slot, resouceView._bindFlags, resouceView._stageFlags);
				break;
			case ResourceType::TEXTURE:
				SetTexture(TYPE_CAST(Texture*, resouceView._resource), resouceView._slot, resouceView._bindFlags, resouceView._stageFlags);
				break;
			case ResourceType::SAMPLER:
				SetSampler(TYPE_CAST(Sampler*, resouceView._resource), resouceView._slot, resouceView._stageFlags);
				break;
			}
		}
	}

	void D3D11CommandBuffer::SetPipelineState(PipelineState* pipelineState)
	{
		auto castedPipeline = TYPE_CAST(D3D11PipelineState*, pipelineState);

		castedPipeline->BindPipeline(_context);
	}

	void D3D11CommandBuffer::SetBlendFactor(const float* color)
	{
	}

	void D3D11CommandBuffer::SetRenderTarget(RenderTarget* renderTarget, unsigned slot)
	{
	}

	void D3D11CommandBuffer::SetRenderPass(RenderPass* renderPass)
	{
		auto castedRenderPass = TYPE_CAST(D3D11RenderPass*, renderPass);

		castedRenderPass->BindRenderPass(_d3dContext);
	}

	void D3D11CommandBuffer::Draw(int numVertices, int startVertexLocation)
	{
		_d3dContext->Draw(numVertices, startVertexLocation);
	}

	void D3D11CommandBuffer::DrawIndexed(int numIndices, int startIndexLoaction, int baseVertexLocation)
	{
		_d3dContext->DrawIndexed(numIndices, startIndexLoaction, baseVertexLocation);
	}

	void D3D11CommandBuffer::DrawIndexedInstanced(int numIndices, int numInstance, int startIndexLocation,
		int startVertexLocation, int startInstanceLocation)
	{
	}

	void D3D11CommandBuffer::DrawInstanced(int numVertices, int startVertexLocation)
	{
	}
}
