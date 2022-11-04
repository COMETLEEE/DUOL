#include "D3D11PipelineState.h"

#include "DUOLGraphicsLibrary/Core/TypeCast.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXFlagMap.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Shader/D3D11Shader.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/StateManager/D3D11StateManager.h"

DUOLGraphicsLibrary::D3D11PipelineState::D3D11PipelineState(const UINT64& guid, ID3D11Device* device, const PipelineStateDesc& pipelineStateDesc):
	PipelineState(guid)
{
	LoadShader(pipelineStateDesc);
	//CreateDepthStencilState(device, pipelineStateDesc);
	CreateRasterizerState(device, pipelineStateDesc);
	//CreateBlendState(device, pipelineStateDesc);

	_primitiveTopology = MapDXPrimitiveTopology(pipelineStateDesc._primitiveTopology);
}

void DUOLGraphicsLibrary::D3D11PipelineState::BindPipeline(D3D11StateManager* stateManager, ID3D11DeviceContext* context)
{

	//context->SetDepthStencilState(_depthStencilState.Get());
	//context->SetBlendState(_blendState.Get(), 0);
	stateManager->SetRasterizerState(context, _rasterizerState.Get());

	stateManager->SetPrimitiveTopology(context, _primitiveTopology);
	stateManager->SetInputLayout(context, _inputLayout.Get());

	stateManager->SetVertexShader(context, _vertexShader.Get());
	stateManager->SetPixelShader(context, _pixelShader.Get());
}

void DUOLGraphicsLibrary::D3D11PipelineState::CreateDepthStencilState(ID3D11Device* device,
	const PipelineStateDesc& pipelineStateDesc)
{
	D3D11_BLEND_DESC blendStateDesc;
	blendStateDesc.AlphaToCoverageEnable = false;
	blendStateDesc.IndependentBlendEnable = false;



	device->CreateBlendState(&blendStateDesc, _blendState.ReleaseAndGetAddressOf());
}

void DUOLGraphicsLibrary::D3D11PipelineState::CreateRasterizerState(ID3D11Device* device,
	const PipelineStateDesc& pipelineStateDesc)
{
	D3D11_RASTERIZER_DESC rasterizerDesc;

	rasterizerDesc.CullMode = MapDXCullMode(pipelineStateDesc._rasterizerStateDesc._cullMode);
	rasterizerDesc.FillMode = MapDXFillMode(pipelineStateDesc._rasterizerStateDesc._fillMode);
	rasterizerDesc.FrontCounterClockwise = pipelineStateDesc._rasterizerStateDesc._frontCounterClockWise;

	rasterizerDesc.DepthBiasClamp = pipelineStateDesc._rasterizerStateDesc._depthBiasClamp;
	rasterizerDesc.DepthBias = pipelineStateDesc._rasterizerStateDesc._depthBias;
	rasterizerDesc.SlopeScaledDepthBias = pipelineStateDesc._rasterizerStateDesc._slopeScaledDepthBias;

	rasterizerDesc.ScissorEnable = pipelineStateDesc._rasterizerStateDesc._scissorEnable;
	rasterizerDesc.MultisampleEnable = pipelineStateDesc._rasterizerStateDesc._multiSampleEnable;
	rasterizerDesc.AntialiasedLineEnable = pipelineStateDesc._rasterizerStateDesc._antialiasedLineEnable;
	rasterizerDesc.DepthClipEnable = pipelineStateDesc._rasterizerStateDesc._depthClipEnable;

	device->CreateRasterizerState(&rasterizerDesc, _rasterizerState.ReleaseAndGetAddressOf());
}

void DUOLGraphicsLibrary::D3D11PipelineState::CreateBlendState(ID3D11Device* device,
	const PipelineStateDesc& pipelineStateDesc)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

}

void DUOLGraphicsLibrary::D3D11PipelineState::LoadShader(const PipelineStateDesc& pipelineStateDesc)
{
	if (pipelineStateDesc._vertexShader != nullptr && pipelineStateDesc._vertexShader->GetShaderType() == ShaderType::VERTEX)
	{
		auto castedShader = TYPE_CAST(D3D11Shader*, pipelineStateDesc._vertexShader);
		_vertexShader = castedShader->GetNativeShader()._vertexShader;
		_inputLayout = castedShader->GetInputLayout();
	}
	if (pipelineStateDesc._pixelShader != nullptr && pipelineStateDesc._pixelShader->GetShaderType() == ShaderType::PIXEL)
	{
		_pixelShader = TYPE_CAST(D3D11Shader*, pipelineStateDesc._pixelShader)->GetNativeShader()._pixelShader;
	}
	if (pipelineStateDesc._hullShader != nullptr && pipelineStateDesc._hullShader->GetShaderType() == ShaderType::HULL)
	{
		_hullShader = TYPE_CAST(D3D11Shader*, pipelineStateDesc._hullShader)->GetNativeShader()._hullShader;
	}
	if (pipelineStateDesc._geometryShader != nullptr && pipelineStateDesc._geometryShader->GetShaderType() == ShaderType::GEOMETRY)
	{
		_geometryShader = TYPE_CAST(D3D11Shader*, pipelineStateDesc._geometryShader)->GetNativeShader()._geometryShader;
	}
	if (pipelineStateDesc._domainShader != nullptr && pipelineStateDesc._domainShader->GetShaderType() == ShaderType::DOMAINS)
	{
		_domainShader = TYPE_CAST(D3D11Shader*, pipelineStateDesc._domainShader)->GetNativeShader()._domainShader;
	}

}
