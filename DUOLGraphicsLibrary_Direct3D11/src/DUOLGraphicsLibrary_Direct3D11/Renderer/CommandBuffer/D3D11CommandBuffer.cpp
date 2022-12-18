#include "D3D11CommandBuffer.h"
#include "DUOLGraphicsLibrary/Core/TypeCast.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Texture.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Buffer/D3D11Buffer.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Sampler.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/PipelineState//D3D11PipelineState.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/RenderTarget/D3D11RenderTarget.h"
#include "DUOLGraphicsLibrary/Renderer/RenderPass.h"

namespace DUOLGraphicsLibrary
{
	D3D11CommandBuffer::D3D11CommandBuffer(
		const UINT64& guid
		, const ComPtr<ID3D11Device>& device
		, const ComPtr<ID3D11DeviceContext>& context
		, const CommandBufferDesc& renderContextDesc) :
		CommandBuffer(guid)
		, _device(device)
		, _d3dContext(context)
	{
	}

	void D3D11CommandBuffer::SetBuffer(Buffer* buffer, unsigned slot, long bindFlags, long stageFlags)
	{
		auto castedBuffer = TYPE_CAST(D3D11Buffer*, buffer);

		if ((bindFlags & static_cast<long>(BindFlags::CONSTANTBUFFER)) != 0)
		{
			ID3D11Buffer* constantBuffer = castedBuffer->GetNativeBuffer();
			_stateManager.SetConstantBuffers(_d3dContext.Get(), slot, 1, &constantBuffer, stageFlags);
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
			_stateManager.SetShaderResources(_d3dContext.Get(), slot, 1, &shaderResourceView, stageFlags);
		}

		if ((bindFlags & static_cast<long>(BindFlags::UNRODERED)) != 0)
		{
			//todo: unorderedmap 
			//ID3D11UnorderedAccessView* uav[] = { textureD3D.GetUAV() };
			//UINT auvCounts[] = { 0 };
			//stateMngr_->SetUnorderedAccessViews(slot, 1, uav, auvCounts, stageFlags);
		}
	}

	void D3D11CommandBuffer::SetSampler(Sampler* sampler, unsigned slot, long stageFlags)
	{
		auto castedSampler = TYPE_CAST(D3D11Sampler*, sampler);

		ID3D11SamplerState* samplerState = castedSampler->GetNativeSampler();
		_stateManager.SetSamplers(_d3dContext.Get(), slot, 1, &samplerState, stageFlags);
	}

	void D3D11CommandBuffer::Begin()
	{
	}

	void D3D11CommandBuffer::Flush()
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);

		ID3D11ShaderResourceView* nullSRVViews[32] = { nullptr, };

		_d3dContext->VSSetShaderResources(0, _countof(nullSRVViews), nullSRVViews);
		_d3dContext->PSSetShaderResources(0, _countof(nullSRVViews), nullSRVViews);

		_d3dContext->VSSetShader(nullptr, nullptr, 0);
		_d3dContext->PSSetShader(nullptr, nullptr, 0);
		_d3dContext->GSSetShader(nullptr, nullptr, 0);
		_d3dContext->HSSetShader(nullptr, nullptr, 0);
		_d3dContext->DSSetShader(nullptr, nullptr, 0);

		_stateManager.Flush();
	}

	void D3D11CommandBuffer::End()
	{
	}

	void D3D11CommandBuffer::UpdateBuffer(Buffer* destBuffer, int destBufferOffset, const void* data, int dataSize)
	{
		auto castedBuffer = TYPE_CAST(D3D11Buffer*, destBuffer);
		castedBuffer->UpdateSubresource(_d3dContext.Get(), data, dataSize, destBufferOffset);
	}

	void D3D11CommandBuffer::CopyBuffer(Buffer* destBuffer, int destOffset, Buffer* srcBuffer, int srcOffset)
	{
		//todo
	}

	void D3D11CommandBuffer::SetViewport(const Viewport& viewport)
	{
		_stateManager.SetViewports(_d3dContext.Get(), 1, &viewport);
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

	void D3D11CommandBuffer::SetResources(const ResourceViewLayout& resourceViewLayout)
	{
		for (auto& resouceView : resourceViewLayout._resourceViews)
		{
			if (resouceView._resource != nullptr)
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
	}

	void D3D11CommandBuffer::SetPipelineState(PipelineState* pipelineState)
	{
		auto castedPipeline = TYPE_CAST(D3D11PipelineState*, pipelineState);

		castedPipeline->BindPipeline(&_stateManager, _d3dContext.Get());
	}

	void D3D11CommandBuffer::SetBlendFactor(const float* color)
	{
	}

	void D3D11CommandBuffer::SetRenderTarget(RenderTarget* renderTarget, unsigned slot)
	{
		//todo 필요한가?
	}

	void D3D11CommandBuffer::SetRenderPass(RenderPass* renderPass)
	{
		//max renderTargets Count is 8
		ID3D11RenderTargetView* colorRenderTargets[8] = { nullptr,};

		int renderTargetCount = renderPass->_renderTargetViewRefs.size();
		for(int renderTargetIndex = 0; renderTargetIndex < renderTargetCount; renderTargetIndex++)
		{
			const float color[4] = { 0.f, 0.f, 0.f, 0.f };

			colorRenderTargets[renderTargetIndex] = TYPE_CAST(D3D11RenderTarget*, renderPass->_renderTargetViewRefs[renderTargetIndex])->GetNativeRenderTarget()._renderTargetView.Get();
			//_d3dContext->ClearRenderTargetView(colorRenderTargets[renderTargetIndex], color);
		}

		auto depthStencilView = TYPE_CAST(D3D11RenderTarget*, renderPass->_depthStencilViewRef)->GetNativeRenderTarget()._depthStencilView.Get();
		_d3dContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		_d3dContext->OMSetRenderTargets(renderTargetCount, &colorRenderTargets[0], depthStencilView);
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

	void D3D11CommandBuffer::BeginSteamOutput(int numBuffers, Buffer* const* buffers)
	{
		constexpr int maxSOBufferSize = 4;

		ID3D11Buffer* soTargets[maxSOBufferSize];
		UINT offsets[maxSOBufferSize];

		numBuffers = ((numBuffers < maxSOBufferSize)? numBuffers : maxSOBufferSize);

		for (std::uint32_t i = 0; i < numBuffers; ++i)
		{
			auto bufferD3D = TYPE_CAST(D3D11Buffer*, buffers[i]);
			soTargets[i] = bufferD3D->GetNativeBuffer();
			offsets[i] = 0;
		}

		_d3dContext->SOSetTargets(numBuffers, soTargets, offsets);
	}

	void D3D11CommandBuffer::EndStreamOutput()
	{
		_d3dContext->SOSetTargets(0, nullptr, nullptr);
	}
}
