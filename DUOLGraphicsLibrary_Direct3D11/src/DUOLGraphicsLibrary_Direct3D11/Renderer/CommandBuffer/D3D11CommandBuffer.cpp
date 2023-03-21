#include "D3D11CommandBuffer.h"
#include "DUOLGraphicsLibrary/Core/TypeCast.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Texture.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Buffer/D3D11Buffer.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Sampler.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/PipelineState//D3D11PipelineState.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/RenderTarget/D3D11RenderTarget.h"
#include "DUOLGraphicsLibrary/Renderer/RenderPass.h"
#include "DUOLGraphicsLibrary/Renderer/Shader.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Buffer/D3D11BufferWithRV.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Shader/D3D11Shader.h"

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
		, _query(nullptr)
		, _queryCheck(true)
	{
#ifdef _DEBUG
		//디버깅일때만 쿼리가 가능합니다.
		D3D11_QUERY_DESC queryDesc;
		ZeroMemory(&queryDesc, sizeof(D3D11_QUERY_DESC));

		queryDesc.Query = D3D11_QUERY_PIPELINE_STATISTICS;

		_device->CreateQuery(&queryDesc, _query.GetAddressOf());
#endif
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
			auto castedRVBuffer = TYPE_CAST(D3D11BufferWithRV*, buffer);

			_stateManager.SetShaderResources(_d3dContext.Get(), slot, 1, castedRVBuffer->GetSRV(), stageFlags);
		}

		//외부에서 한번에 바인딩해줍니다.
		if ((bindFlags & static_cast<long>(BindFlags::UNORDEREDACCESS)) != 0)
		{
			auto castedRVBuffer = TYPE_CAST(D3D11BufferWithRV*, buffer);
			auto uav = castedRVBuffer->GetUAV();

			_stateManager.SetUnorderedAccessView(_d3dContext.Get(), slot, 1, nullptr, uav, stageFlags);
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

		if ((bindFlags & static_cast<long>(BindFlags::UNORDEREDACCESS)) != 0)
		{
			//todo: unorderedmap 
			ID3D11UnorderedAccessView* uav = castedTexture->GetUnorderedAccessView();
			UINT auvCounts[] = { 0 };
			_stateManager.SetUnorderedAccessView(_d3dContext.Get(), slot, 1, auvCounts, &uav, stageFlags);
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
#ifdef _DEBUG
		if (_queryCheck)
		{
			_d3dContext->Begin(_query.Get());
		}
#endif
	}

	void D3D11CommandBuffer::Flush()
	{
		ID3D11RenderTargetView* nullViews[8] = { nullptr };
		ID3D11UnorderedAccessView* nullUAViews[8] = { nullptr };
		_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
		ID3D11ShaderResourceView* nullSRVViews[32] = { nullptr, };
		_d3dContext->CSSetUnorderedAccessViews(0, 8, nullUAViews, 0);

		ID3D11SamplerState* nullSampler[8] = { nullptr, };

		_d3dContext->VSSetShaderResources(0, _countof(nullSRVViews), nullSRVViews);
		_d3dContext->PSSetShaderResources(0, _countof(nullSRVViews), nullSRVViews);
		_d3dContext->GSSetShaderResources(0, _countof(nullSRVViews), nullSRVViews);
		_d3dContext->DSSetShaderResources(0, _countof(nullSRVViews), nullSRVViews);
		_d3dContext->HSSetShaderResources(0, _countof(nullSRVViews), nullSRVViews);
		_d3dContext->CSSetShaderResources(0, _countof(nullSRVViews), nullSRVViews);

		_d3dContext->VSSetShader(nullptr, nullptr, 0);
		_d3dContext->PSSetShader(nullptr, nullptr, 0);
		_d3dContext->GSSetShader(nullptr, nullptr, 0);
		_d3dContext->HSSetShader(nullptr, nullptr, 0);
		_d3dContext->DSSetShader(nullptr, nullptr, 0);
		_d3dContext->CSSetShader(nullptr, nullptr, 0);

		_d3dContext->VSSetSamplers(0, _countof(nullSampler), nullSampler);
		_d3dContext->PSSetSamplers(0, _countof(nullSampler), nullSampler);
		_d3dContext->GSSetSamplers(0, _countof(nullSampler), nullSampler);
		_d3dContext->HSSetSamplers(0, _countof(nullSampler), nullSampler);
		_d3dContext->DSSetSamplers(0, _countof(nullSampler), nullSampler);
		_d3dContext->CSSetSamplers(0, _countof(nullSampler), nullSampler);

		_stateManager.Flush();
	}

	void D3D11CommandBuffer::ClearState()
	{
		Flush();
		//_d3dContext->Flush();
		_d3dContext->ClearState();
	}

	void D3D11CommandBuffer::End()
	{
#ifdef _DEBUG
		if (_queryCheck)
		{
			_d3dContext->End(_query.Get());
			_queryCheck = false;
		}
#endif
	}

	void D3D11CommandBuffer::UpdateBuffer(Buffer* destBuffer, int destBufferOffset, const void* data, int dataSize)
	{
		auto castedBuffer = TYPE_CAST(D3D11Buffer*, destBuffer);
		castedBuffer->UpdateSubresource(_d3dContext.Get(), data, dataSize, destBufferOffset);
	}

	void D3D11CommandBuffer::CopyBuffer(Buffer* dstBuffer, int dstOffset, Buffer* srcBuffer, int srcOffset, int size)
	{
		auto dstCastedBuffer = TYPE_CAST(D3D11Buffer*, dstBuffer);
		auto srcCastedBuffer = TYPE_CAST(D3D11Buffer*, srcBuffer);

		auto box = CD3D11_BOX(                                    // pSrcBox
			static_cast<LONG>(srcOffset), 0, 0,
			static_cast<LONG>(srcOffset + size), 1, 1
		);

		_d3dContext->CopySubresourceRegion(
			dstCastedBuffer->GetNativeBuffer(),                       // pDstResource
			0,                                              // DstSubresource
			static_cast<UINT>(dstOffset),                   // DstX
			0,                                              // DstY
			0,                                              // DstZ
			srcCastedBuffer->GetNativeBuffer(),                       // pSrcResource
			0,                                              // SrcSubresource
			&box
		);
	}

	void D3D11CommandBuffer::CopyTexture(Texture* destTexture, const TextureLocation& destOption, Texture* srcTexture,
		const TextureLocation& srcOption)
	{
		auto dstCastedTexture = TYPE_CAST(D3D11Texture*, destTexture);
		auto srcCastedTexture = TYPE_CAST(D3D11Texture*, srcTexture);

		auto& size = srcTexture->GetTextureDesc()._textureExtent;
		D3D11_BOX srcBox = CD3D11_BOX(0, 0, 0, static_cast<LONG>(size.x), static_cast<LONG>(size.y), static_cast<LONG>(size.z));

		_d3dContext->CopySubresourceRegion(
			dstCastedTexture->GetNativeTexture()._resource.Get()
			, D3D11CalcSubresource(destOption._mipLevel, destOption._arrayLayer, dstCastedTexture->GetTextureDesc()._mipLevels)
			, 0
			, 0
			, 0
			, srcCastedTexture->GetNativeTexture()._resource.Get()
			, D3D11CalcSubresource(destOption._mipLevel, destOption._arrayLayer, dstCastedTexture->GetTextureDesc()._mipLevels)
			, &srcBox
		);
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
		if (buffer == nullptr)
		{
			_d3dContext->IASetVertexBuffers(0, 1, nullptr, 0, 0);
			return;
		}

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
		if (buffer == nullptr)
		{
			_d3dContext->IASetVertexBuffers(0, 1, nullptr, 0, 0);
			return;
		}

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
		ID3D11UnorderedAccessView* uav[8] = { nullptr };
		bool _hasUnorderedAccessView = false;
		int minSlotForUAV = 8;
		int maxSlotForUAV = 0;

		for (auto& resourceView : resourceViewLayout._resourceViews)
		{
			//OMSetRenderTargetsAndUnorderedAccessViews 로 바인딩 해줄 친구들은 한꺼번에 바인딩해준다
			//Note  RTVs, DSV, and UAVs cannot be set independently; they all need to be set at the same time.

			if ((resourceView._bindFlags == static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS)) & (resourceView._stageFlags == static_cast<long>(ShaderType::PIXEL)))
			{
				if (resourceView._resource != nullptr)
				{
					_hasUnorderedAccessView = true;

					switch (resourceView._resource->GetResourceType())
					{
					case ResourceType::BUFFER:
					{
						//외부에서 한번에 바인딩해줍니다.
						if ((resourceView._bindFlags & static_cast<long>(BindFlags::UNORDEREDACCESS)) != 0)
						{
							auto castedRVBuffer = TYPE_CAST(D3D11BufferWithRV*, resourceView._resource);

							uav[resourceView._slot] = *castedRVBuffer->GetUAV();
							minSlotForUAV = std::min<int>(minSlotForUAV, resourceView._slot);
							maxSlotForUAV = std::max<int>(maxSlotForUAV, resourceView._slot);
						}
					}
					break;
					case ResourceType::TEXTURE:
						if ((resourceView._bindFlags & static_cast<long>(BindFlags::UNORDEREDACCESS)) != 0)
						{
							auto castedTexture = TYPE_CAST(D3D11Texture*, resourceView._resource);

							uav[resourceView._slot] = castedTexture->GetUnorderedAccessView();
							minSlotForUAV = std::min<int>(minSlotForUAV, resourceView._slot);
							maxSlotForUAV = std::max<int>(maxSlotForUAV, resourceView._slot);
						}
						break;
					case ResourceType::SAMPLER:
					case ResourceType::UNDEFINED:
						break;
					}
				}
			}
			else
			{
				if (resourceView._resource != nullptr)
				{
					switch (resourceView._resource->GetResourceType())
					{
					case ResourceType::UNDEFINED:
						break;
					case ResourceType::BUFFER:
						SetBuffer(TYPE_CAST(Buffer*, resourceView._resource), resourceView._slot, resourceView._bindFlags, resourceView._stageFlags);
						break;
					case ResourceType::TEXTURE:
						SetTexture(TYPE_CAST(Texture*, resourceView._resource), resourceView._slot, resourceView._bindFlags, resourceView._stageFlags);
						break;
					case ResourceType::SAMPLER:
						SetSampler(TYPE_CAST(Sampler*, resourceView._resource), resourceView._slot, resourceView._stageFlags);
						break;
					}
				}
			}
		}

		if(_hasUnorderedAccessView)
		{
			_stateManager.SetUnorderedAccessView(_d3dContext.Get(), minSlotForUAV, maxSlotForUAV - minSlotForUAV + 1, nullptr, uav, static_cast<long>(ShaderType::PIXEL));
		}

	}

	void D3D11CommandBuffer::SetShader(Shader* shader)
	{

		switch (shader->GetShaderType())
		{
		case ShaderType::UNKNOWN: break;
		case ShaderType::VERTEX:
		{
			auto castedShader = TYPE_CAST(D3D11Shader*, shader);
			_stateManager.SetVertexShader(_d3dContext.Get(), castedShader->GetNativeShader()._vertexShader.Get());
			break;
		}
		case ShaderType::HULL: break;
		case ShaderType::DOMAINS: break;
		case ShaderType::GEOMETRY:
		{
			auto castedShader = TYPE_CAST(D3D11Shader*, shader);
			_stateManager.SetGeometryShader(_d3dContext.Get(), castedShader->GetNativeShader()._geometryShader.Get());
			break;
		}
		case ShaderType::PIXEL:
		{
			auto castedShader = TYPE_CAST(D3D11Shader*, shader);
			_stateManager.SetPixelShader(_d3dContext.Get(), castedShader->GetNativeShader()._pixelShader.Get());
			break;
		}
		case ShaderType::COMPUTE: break;
		default:;
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

	void D3D11CommandBuffer::SetRenderTarget(RenderTarget* renderTarget, RenderTarget* depth, unsigned slot)
	{
		ID3D11RenderTargetView** rs;
		ID3D11DepthStencilView* ds;

		if (depth == nullptr)
		{
			ds = nullptr;
		}
		else
		{
			auto nativeds = TYPE_CAST(D3D11RenderTarget*, depth);
			ds = nativeds->GetNativeRenderTarget()._depthStencilView.Get();
		}

		if (renderTarget == nullptr)
		{
			rs = nullptr;
			_d3dContext->OMSetRenderTargets(0, rs, ds);
		}
		else
		{
			auto rt = TYPE_CAST(D3D11RenderTarget*, renderTarget);
			rs = rt->GetNativeRenderTarget()._renderTargetView.GetAddressOf();
			_d3dContext->OMSetRenderTargets(1, rs, ds);
		}
	}

	void D3D11CommandBuffer::SetRenderPass(RenderPass* renderPass)
	{
		//max renderTargets Count is 8
		ID3D11RenderTargetView* colorRenderTargets[8] = { nullptr, };

		UINT renderTargetCount = static_cast<UINT>(renderPass->_renderTargetViewRefs.size());
		for (UINT renderTargetIndex = 0; renderTargetIndex < renderTargetCount; renderTargetIndex++)
		{
			const float color[4] = { 0.f, 0.f, 0.f, 0.f };

			colorRenderTargets[renderTargetIndex] = TYPE_CAST(D3D11RenderTarget*, renderPass->_renderTargetViewRefs[renderTargetIndex])->GetNativeRenderTarget()._renderTargetView.Get();
		}

		ID3D11DepthStencilView* d3dDepthStencilView = nullptr;

		if (renderPass->_depthStencilViewRef != nullptr)
		{
			auto depthStencilView = TYPE_CAST(D3D11RenderTarget*, renderPass->_depthStencilViewRef);
			d3dDepthStencilView = depthStencilView->GetNativeRenderTarget()._depthStencilView.Get();
		}

		_d3dContext->OMSetRenderTargets(renderTargetCount, &colorRenderTargets[0], d3dDepthStencilView);
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

	void D3D11CommandBuffer::DrawAuto()
	{
		_d3dContext->DrawAuto();
	}

	void D3D11CommandBuffer::BeginStreamOutput(int numBuffers, Buffer* const* buffers)
	{
		constexpr int maxSOBufferSize = 4;

		ID3D11Buffer* soTargets[maxSOBufferSize] = { nullptr, };
		UINT offsets[maxSOBufferSize] = { 0, };

		numBuffers = ((numBuffers < maxSOBufferSize) ? numBuffers : maxSOBufferSize);

		for (int i = 0; i < numBuffers; ++i)
		{
			auto bufferD3D = TYPE_CAST(D3D11Buffer*, buffers[i]);
			soTargets[i] = bufferD3D->GetNativeBuffer();
			offsets[i] = 0;
		}

		_d3dContext->SOSetTargets(1, soTargets, offsets);
	}

	void D3D11CommandBuffer::EndStreamOutput()
	{
		_d3dContext->SOSetTargets(0, nullptr, nullptr);
	}

	void D3D11CommandBuffer::Dispatch(int WorkGroupsX, int WorkGroupsY, int WorkGroupsZ)
	{
		_d3dContext->Dispatch(WorkGroupsX, WorkGroupsY, WorkGroupsZ);
	}

	bool D3D11CommandBuffer::GetData(QueryInfo& outData)
	{
#ifdef _DEBUG
		D3D11_QUERY_DATA_PIPELINE_STATISTICS queryData;

		if (S_OK != _d3dContext->GetData(_query.Get(), &queryData, sizeof(D3D11_QUERY_DATA_PIPELINE_STATISTICS), 0))
		{
			return false;
		}
		_queryCheck = true;

		outData._IAVertices = queryData.IAVertices;
		outData._IAPrimitives = queryData.IAPrimitives;

		outData._VSInvocations = queryData.VSInvocations;

		outData._PSInvocations = queryData.PSInvocations;

		outData._CInvocations = queryData.CInvocations;
		outData._CPrimitives = queryData.CPrimitives;

		outData._GSInvocations = queryData.GSInvocations;
		outData._GSPrimitives = queryData.GSPrimitives;

		outData._HSInvocations = queryData.HSInvocations;
		outData._DSInvocations = queryData.DSInvocations;
		outData._CSInvocations = queryData.CSInvocations;

		return true;

#endif
		return false;
	}
}
