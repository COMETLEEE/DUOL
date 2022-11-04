#include "D3D11StateManager.h"
#include "DUOLGraphicsLibrary/ShaderFlags.h"

namespace DUOLGraphicsLibrary
{
	void D3D11StateManager::SetViewports(ID3D11DeviceContext* context, UINT numViewports, const Viewport* viewportArray)
	{
		context->RSSetViewports(numViewports, reinterpret_cast<const D3D11_VIEWPORT*>(viewportArray));

	}

	void D3D11StateManager::SetPrimitiveTopology(ID3D11DeviceContext* context, D3D11_PRIMITIVE_TOPOLOGY primitiveTopology)
	{
		if (_inputAssemblyState._primitiveTopology != primitiveTopology)
		{
			_inputAssemblyState._primitiveTopology = primitiveTopology;
			context->IASetPrimitiveTopology(primitiveTopology);
		}
	}

	void D3D11StateManager::SetInputLayout(ID3D11DeviceContext* context, ID3D11InputLayout* inputLayout)
	{
		if (_inputAssemblyState._inputLayout != inputLayout)
		{
			_inputAssemblyState._inputLayout = inputLayout;
			context->IASetInputLayout(inputLayout);
		}
	}

	void D3D11StateManager::SetVertexShader(ID3D11DeviceContext* context, ID3D11VertexShader* shader)
	{
		if (_shaderState._vertexShader != shader)
		{
			_shaderState._vertexShader = shader;
			context->VSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11StateManager::SetHullShader(ID3D11DeviceContext* context, ID3D11HullShader* shader)
	{
		if (_shaderState._hullShader != shader)
		{
			_shaderState._hullShader = shader;
			context->HSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11StateManager::SetDomainShader(ID3D11DeviceContext* context, ID3D11DomainShader* shader)
	{
		if (_shaderState._domainShader != shader)
		{
			_shaderState._domainShader = shader;
			context->DSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11StateManager::SetGeometryShader(ID3D11DeviceContext* context, ID3D11GeometryShader* shader)
	{
		if (_shaderState._geometryShader != shader)
		{
			_shaderState._geometryShader = shader;
			context->GSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11StateManager::SetPixelShader(ID3D11DeviceContext* context, ID3D11PixelShader* shader)
	{
		if (_shaderState._pixelShader != shader)
		{
			_shaderState._pixelShader = shader;
			context->PSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11StateManager::SetComputeShader(ID3D11DeviceContext* context, ID3D11ComputeShader* shader)
	{
		if (_shaderState._computeShader != shader)
		{
			_shaderState._computeShader = shader;
			context->CSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11StateManager::SetRasterizerState(ID3D11DeviceContext* context, ID3D11RasterizerState* rasterizerState)
	{
		if (_renderState._rasterizerState != rasterizerState)
		{
			_renderState._rasterizerState = rasterizerState;
			context->RSSetState(rasterizerState);
		}
	}

	void D3D11StateManager::SetDepthStencilState(ID3D11DeviceContext* context, ID3D11DepthStencilState* depthStencilState)
	{
		if (_renderState._depthStencilState != depthStencilState)
		{
			_renderState._depthStencilState = depthStencilState;
			context->OMSetDepthStencilState(depthStencilState, _renderState._stencilRef);
		}
	}

	void D3D11StateManager::SetDepthStencilState(ID3D11DeviceContext* context, ID3D11DepthStencilState* depthStencilState, UINT stencilRef)
	{
		if (_renderState._depthStencilState != depthStencilState || _renderState._stencilRef != stencilRef)
		{
			_renderState._depthStencilState = depthStencilState;
			_renderState._stencilRef = stencilRef;
			context->OMSetDepthStencilState(depthStencilState, _renderState._stencilRef);
		}
	}

	void D3D11StateManager::SetStencilRef(ID3D11DeviceContext* context, UINT stencilRef)
	{
		if (_renderState._stencilRef != stencilRef)
		{
			_renderState._stencilRef = stencilRef;
			context->OMSetDepthStencilState(_renderState._depthStencilState, _renderState._stencilRef);
		}
	}

	void D3D11StateManager::SetBlendState(ID3D11DeviceContext* context, ID3D11BlendState* blendState, UINT sampleMask)
	{
		if (_renderState._blendState != blendState || _renderState._sampleMask != sampleMask)
		{
			_renderState._blendState = blendState;
			_renderState._sampleMask = sampleMask;
			context->OMSetBlendState(_renderState._blendState, _renderState._blendFactor, _renderState._sampleMask);
		}
	}

	void D3D11StateManager::SetBlendState(ID3D11DeviceContext* context, ID3D11BlendState* blendState, const FLOAT* blendFactor, UINT sampleMask)
	{
		if (_renderState._blendState != blendState)
		{
			_renderState._blendState = blendState;
			_renderState._blendFactor[0] = blendFactor[0];
			_renderState._blendFactor[1] = blendFactor[1];
			_renderState._blendFactor[2] = blendFactor[2];
			_renderState._blendFactor[3] = blendFactor[3];
			_renderState._sampleMask = sampleMask;
			context->OMSetBlendState(_renderState._blendState, _renderState._blendFactor, _renderState._sampleMask);
		}
	}

	void D3D11StateManager::SetBlendFactor(ID3D11DeviceContext* context, const FLOAT* blendFactor)
	{
	}

	void D3D11StateManager::SetConstantBuffers(ID3D11DeviceContext* context, UINT startSlot, UINT bufferCount, ID3D11Buffer* const* buffers, long stageFlags)
	{
		if (static_cast<long>(ShaderType::VERTEX) & stageFlags)
		{
			context->VSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::HULL) & stageFlags)
		{
			context->HSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::DOMAINS) & stageFlags)
		{
			context->DSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::GEOMETRY) & stageFlags)
		{
			context->GSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::PIXEL) & stageFlags)
		{
			context->PSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::COMPUTE) & stageFlags)
		{
			context->CSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
	}

	void D3D11StateManager::SetConstantBuffersArray(ID3D11DeviceContext* context, UINT startSlot, UINT bufferCount, ID3D11Buffer* const* buffers, const UINT* firstConstants, const UINT* numConstants, long stageFlags)
	{
		if (static_cast<long>(ShaderType::VERTEX) & stageFlags)
		{
			context->VSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::HULL) & stageFlags)
		{
			context->HSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::DOMAINS) & stageFlags)
		{
			context->DSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::GEOMETRY) & stageFlags)
		{
			context->GSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::PIXEL) & stageFlags)
		{
			context->PSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::COMPUTE) & stageFlags)
		{
			context->CSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
	}

	void D3D11StateManager::SetShaderResources(ID3D11DeviceContext* context, UINT startSlot, UINT resourceCount, ID3D11ShaderResourceView* const* views, long stageFlags)
	{
		if (static_cast<long>(ShaderType::VERTEX) & stageFlags)
		{
			context->VSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::HULL) & stageFlags)
		{
			context->HSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::DOMAINS) & stageFlags)
		{
			context->DSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::GEOMETRY) & stageFlags)
		{
			context->GSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::PIXEL) & stageFlags)
		{
			context->PSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::COMPUTE) & stageFlags)
		{
			context->CSSetShaderResources(startSlot, resourceCount, views);
		}
	}

	void D3D11StateManager::SetSamplers(ID3D11DeviceContext* context, UINT startSlot, UINT samplerCount, ID3D11SamplerState* const* samplers,
		long stageFlags)
	{
		if (static_cast<long>(ShaderType::VERTEX) & stageFlags)
		{
			context->VSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::HULL) & stageFlags)
		{
			context->HSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::DOMAINS) & stageFlags)
		{
			context->DSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::GEOMETRY) & stageFlags)
		{
			context->GSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::PIXEL) & stageFlags)
		{
			context->PSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::COMPUTE) & stageFlags)
		{
			context->CSSetSamplers(startSlot, samplerCount, samplers);
		}
	}


}
