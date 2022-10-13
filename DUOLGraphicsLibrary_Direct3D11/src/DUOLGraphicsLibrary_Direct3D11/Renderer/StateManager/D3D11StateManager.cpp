#include "D3D11StateManager.h"
#include "DUOLGraphicsLibrary/ShaderFlags.h"

namespace DUOLGraphicsLibrary
{
	void D3D11StateManager::SetViewports(UINT numViewports, const Viewport* viewportArray)
	{
		_context->RSSetViewports(numViewports, reinterpret_cast<const D3D11_VIEWPORT*>(viewportArray));

	}

	void D3D11StateManager::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology)
	{
		if (_inputAssemblyState._primitiveTopology != primitiveTopology)
		{
			_inputAssemblyState._primitiveTopology = primitiveTopology;
			_context->IASetPrimitiveTopology(primitiveTopology);
		}
	}

	void D3D11StateManager::SetInputLayout(ID3D11InputLayout* inputLayout)
	{
		if (_inputAssemblyState._inputLayout != inputLayout)
		{
			_inputAssemblyState._inputLayout = inputLayout;
			_context->IASetInputLayout(inputLayout);
		}
	}

	void D3D11StateManager::SetVertexShader(ID3D11VertexShader* shader)
	{
		if (_shaderState._vertexShader != shader)
		{
			_shaderState._vertexShader = shader;
			_context->VSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11StateManager::SetHullShader(ID3D11HullShader* shader)
	{
		if (_shaderState._hullShader != shader)
		{
			_shaderState._hullShader = shader;
			_context->HSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11StateManager::SetDomainShader(ID3D11DomainShader* shader)
	{
		if (_shaderState._domainShader != shader)
		{
			_shaderState._domainShader = shader;
			_context->DSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11StateManager::SetGeometryShader(ID3D11GeometryShader* shader)
	{
		if (_shaderState._geometryShader != shader)
		{
			_shaderState._geometryShader = shader;
			_context->GSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11StateManager::SetPixelShader(ID3D11PixelShader* shader)
	{
		if (_shaderState._pixelShader != shader)
		{
			_shaderState._pixelShader = shader;
			_context->PSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11StateManager::SetComputeShader(ID3D11ComputeShader* shader)
	{
		if (_shaderState._computeShader != shader)
		{
			_shaderState._computeShader = shader;
			_context->CSSetShader(shader, nullptr, 0);
		}
	}

	void D3D11StateManager::SetRasterizerState(ID3D11RasterizerState* rasterizerState)
	{
		if (_renderState._rasterizerState != rasterizerState)
		{
			_renderState._rasterizerState = rasterizerState;
			_context->RSSetState(rasterizerState);
		}
	}

	void D3D11StateManager::SetDepthStencilState(ID3D11DepthStencilState* depthStencilState)
	{
		if (_renderState._depthStencilState != depthStencilState)
		{
			_renderState._depthStencilState = depthStencilState;
			_context->OMSetDepthStencilState(depthStencilState, _renderState._stencilRef);
		}
	}

	void D3D11StateManager::SetDepthStencilState(ID3D11DepthStencilState* depthStencilState, UINT stencilRef)
	{
		if (_renderState._depthStencilState != depthStencilState || _renderState._stencilRef != stencilRef)
		{
			_renderState._depthStencilState = depthStencilState;
			_renderState._stencilRef = stencilRef;
			_context->OMSetDepthStencilState(depthStencilState, _renderState._stencilRef);
		}
	}

	void D3D11StateManager::SetStencilRef(UINT stencilRef)
	{
		if (_renderState._stencilRef != stencilRef)
		{
			_renderState._stencilRef = stencilRef;
			_context->OMSetDepthStencilState(_renderState._depthStencilState, _renderState._stencilRef);
		}
	}

	void D3D11StateManager::SetBlendState(ID3D11BlendState* blendState, UINT sampleMask)
	{
		if (_renderState._blendState != blendState || _renderState._sampleMask != sampleMask)
		{
			_renderState._blendState = blendState;
			_renderState._sampleMask = sampleMask;
			_context->OMSetBlendState(_renderState._blendState, _renderState._blendFactor, _renderState._sampleMask);
		}
	}

	void D3D11StateManager::SetBlendState(ID3D11BlendState* blendState, const FLOAT* blendFactor, UINT sampleMask)
	{
		if (_renderState._blendState != blendState)
		{
			_renderState._blendState = blendState;
			_renderState._blendFactor[0] = blendFactor[0];
			_renderState._blendFactor[1] = blendFactor[1];
			_renderState._blendFactor[2] = blendFactor[2];
			_renderState._blendFactor[3] = blendFactor[3];
			_renderState._sampleMask = sampleMask;
			_context->OMSetBlendState(_renderState._blendState, _renderState._blendFactor, _renderState._sampleMask);
		}
	}

	void D3D11StateManager::SetBlendFactor(const FLOAT* blendFactor)
	{
	}

	void D3D11StateManager::SetConstantBuffers(UINT startSlot, UINT bufferCount, ID3D11Buffer* const* buffers, long stageFlags)
	{
		if (static_cast<long>(ShaderType::VERTEX) & stageFlags)
		{
			_context->VSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::HULL) & stageFlags)
		{
			_context->HSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::DOMAINS) & stageFlags)
		{
			_context->DSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::GEOMETRY) & stageFlags)
		{
			_context->GSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::PIXEL) & stageFlags)
		{
			_context->PSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::COMPUTE) & stageFlags)
		{
			_context->CSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
	}

	void D3D11StateManager::SetConstantBuffersArray(UINT startSlot, UINT bufferCount, ID3D11Buffer* const* buffers, const UINT* firstConstants, const UINT* numConstants, long stageFlags)
	{
		if (static_cast<long>(ShaderType::VERTEX) & stageFlags)
		{
			_context->VSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::HULL) & stageFlags)
		{
			_context->HSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::DOMAINS) & stageFlags)
		{
			_context->DSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::GEOMETRY) & stageFlags)
		{
			_context->GSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::PIXEL) & stageFlags)
		{
			_context->PSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
		if (static_cast<long>(ShaderType::COMPUTE) & stageFlags)
		{
			_context->CSSetConstantBuffers(startSlot, bufferCount, buffers);
		}
	}

	void D3D11StateManager::SetShaderResources(UINT startSlot, UINT resourceCount, ID3D11ShaderResourceView* const* views, long stageFlags)
	{
		if (static_cast<long>(ShaderType::VERTEX) & stageFlags)
		{
			_context->VSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::HULL) & stageFlags)
		{
			_context->HSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::DOMAINS) & stageFlags)
		{
			_context->DSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::GEOMETRY) & stageFlags)
		{
			_context->GSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::PIXEL) & stageFlags)
		{
			_context->PSSetShaderResources(startSlot, resourceCount, views);
		}
		if (static_cast<long>(ShaderType::COMPUTE) & stageFlags)
		{
			_context->CSSetShaderResources(startSlot, resourceCount, views);
		}
	}

	void D3D11StateManager::SetSamplers(UINT startSlot, UINT samplerCount, ID3D11SamplerState* const* samplers,
		long stageFlags)
	{
		if (static_cast<long>(ShaderType::VERTEX) & stageFlags)
		{
			_context->VSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::HULL) & stageFlags)
		{
			_context->HSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::DOMAINS) & stageFlags)
		{
			_context->DSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::GEOMETRY) & stageFlags)
		{
			_context->GSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::PIXEL) & stageFlags)
		{
			_context->PSSetSamplers(startSlot, samplerCount, samplers);
		}
		if (static_cast<long>(ShaderType::COMPUTE) & stageFlags)
		{
			_context->CSSetSamplers(startSlot, samplerCount, samplers);
		}
	}


}
