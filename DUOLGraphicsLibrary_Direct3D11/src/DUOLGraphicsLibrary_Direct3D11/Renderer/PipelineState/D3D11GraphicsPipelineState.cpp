#include "D3D11GraphicsPipelineState.h"

#include "DUOLGraphicsLibrary/Core/TypeCast.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXFlagMap.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Shader/D3D11Shader.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/StateManager/D3D11StateManager.h"

DUOLGraphicsLibrary::D3D11GraphicsPipelineState::D3D11GraphicsPipelineState(const UINT64& guid, ID3D11Device* device, const PipelineStateDesc& pipelineStateDesc) :
	D3D11PipelineState(guid)
	, _inputLayout(nullptr)
	, _vertexShader(nullptr)
	, _hullShader(nullptr)
	, _domainShader(nullptr)
	, _geometryShader(nullptr)
	, _pixelShader(nullptr)
	, _depthStencilState(nullptr)
	, _rasterizerState(nullptr)
	, _blendState(nullptr)
	, _pipelineStateDesc(pipelineStateDesc)
{
	LoadShader(pipelineStateDesc);
	CreateDepthStencilState(device, pipelineStateDesc);
	CreateRasterizerState(device, pipelineStateDesc);
	CreateBlendState(device, pipelineStateDesc);

	_primitiveTopology = MapDXPrimitiveTopology(pipelineStateDesc._primitiveTopology);
}

bool DUOLGraphicsLibrary::D3D11GraphicsPipelineState::HasGeometryShader()
{
	return (_geometryShader == nullptr) ? false : true;
}

void DUOLGraphicsLibrary::D3D11GraphicsPipelineState::BindPipeline(D3D11StateManager* stateManager, ID3D11DeviceContext* context)
{
	stateManager->SetBlendState(context, _blendState.Get(), 0xffffffff);
	stateManager->SetDepthStencilState(context, _depthStencilState.Get(), 0);
	stateManager->SetRasterizerState(context, _rasterizerState.Get());

	stateManager->SetPrimitiveTopology(context, _primitiveTopology);
	stateManager->SetInputLayout(context, _inputLayout.Get());

	stateManager->SetVertexShader(context, _vertexShader.Get());
	stateManager->SetPixelShader(context, _pixelShader.Get());
	stateManager->SetGeometryShader(context, _geometryShader.Get());
}

void DUOLGraphicsLibrary::D3D11GraphicsPipelineState::CreateDepthStencilState(ID3D11Device* device,
	const PipelineStateDesc& pipelineStateDesc)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	depthStencilDesc.DepthEnable = pipelineStateDesc._depthStencilStateDesc._depthEnable;
	depthStencilDesc.DepthFunc = MapDXComparisonFunc(pipelineStateDesc._depthStencilStateDesc._depthCompareOp);
	depthStencilDesc.DepthWriteMask = (pipelineStateDesc._depthStencilStateDesc._writeEnable) ? D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;

	depthStencilDesc.StencilEnable = pipelineStateDesc._depthStencilStateDesc._stencilEnable;
	depthStencilDesc.StencilReadMask = pipelineStateDesc._depthStencilStateDesc._stencilReadMask;
	depthStencilDesc.StencilWriteMask = pipelineStateDesc._depthStencilStateDesc._stencilWriteMask;

	depthStencilDesc.FrontFace.StencilFailOp = MapDXStencil(pipelineStateDesc._depthStencilStateDesc._frontFace._stencilFailOp);
	depthStencilDesc.FrontFace.StencilDepthFailOp = MapDXStencil(pipelineStateDesc._depthStencilStateDesc._frontFace._stencilDepthFailOp);
	depthStencilDesc.FrontFace.StencilPassOp = MapDXStencil(pipelineStateDesc._depthStencilStateDesc._frontFace._stencilPassOp);
	depthStencilDesc.FrontFace.StencilFunc = MapDXComparisonFunc(pipelineStateDesc._depthStencilStateDesc._frontFace._comparisonOp);

	depthStencilDesc.BackFace.StencilFailOp = MapDXStencil(pipelineStateDesc._depthStencilStateDesc._backFace._stencilFailOp);
	depthStencilDesc.BackFace.StencilDepthFailOp = MapDXStencil(pipelineStateDesc._depthStencilStateDesc._backFace._stencilDepthFailOp);
	depthStencilDesc.BackFace.StencilPassOp = MapDXStencil(pipelineStateDesc._depthStencilStateDesc._backFace._stencilPassOp);
	depthStencilDesc.BackFace.StencilFunc = MapDXComparisonFunc(pipelineStateDesc._depthStencilStateDesc._backFace._comparisonOp);

	device->CreateDepthStencilState(&depthStencilDesc, _depthStencilState.GetAddressOf());
}

void DUOLGraphicsLibrary::D3D11GraphicsPipelineState::CreateRasterizerState(ID3D11Device* device,
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

void DUOLGraphicsLibrary::D3D11GraphicsPipelineState::CreateBlendState(ID3D11Device* device,
	const PipelineStateDesc& pipelineStateDesc)
{
	D3D11_BLEND_DESC blendStateDesc;
	blendStateDesc.AlphaToCoverageEnable = pipelineStateDesc._blendStateDesc._alphaToCoverageEnable;
	blendStateDesc.IndependentBlendEnable = pipelineStateDesc._blendStateDesc._independentBlendEnable;

	for (int renderTargetIdx = 0; renderTargetIdx < 8; renderTargetIdx++)
	{
		blendStateDesc.RenderTarget[renderTargetIdx].BlendEnable = pipelineStateDesc._blendStateDesc._renderTarget[renderTargetIdx]._blendEnable;
		blendStateDesc.RenderTarget[renderTargetIdx].SrcBlend = MapDXBlend(pipelineStateDesc._blendStateDesc._renderTarget[renderTargetIdx]._srcBlend);
		blendStateDesc.RenderTarget[renderTargetIdx].DestBlend = MapDXBlend(pipelineStateDesc._blendStateDesc._renderTarget[renderTargetIdx]._destBlend);
		blendStateDesc.RenderTarget[renderTargetIdx].BlendOp = MapDXBlendOp(pipelineStateDesc._blendStateDesc._renderTarget[renderTargetIdx]._blendOp);
		blendStateDesc.RenderTarget[renderTargetIdx].SrcBlendAlpha = MapDXBlend(pipelineStateDesc._blendStateDesc._renderTarget[renderTargetIdx]._srcBlendAlpha);
		blendStateDesc.RenderTarget[renderTargetIdx].DestBlendAlpha = MapDXBlend(pipelineStateDesc._blendStateDesc._renderTarget[renderTargetIdx]._destBlendAlpha);
		blendStateDesc.RenderTarget[renderTargetIdx].BlendOpAlpha = MapDXBlendOp(pipelineStateDesc._blendStateDesc._renderTarget[renderTargetIdx]._blendOpAlpha);
		blendStateDesc.RenderTarget[renderTargetIdx].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	device->CreateBlendState(&blendStateDesc, _blendState.ReleaseAndGetAddressOf());
}

void DUOLGraphicsLibrary::D3D11GraphicsPipelineState::LoadShader(const PipelineStateDesc& pipelineStateDesc)
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
