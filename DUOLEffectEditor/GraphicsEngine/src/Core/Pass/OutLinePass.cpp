#include "Core/Pass/OutLinePass.h"

#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/SamplerState.h"
#include "Core/DirectX11/RasterizerState.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/Resource/VBIBMesh.h"
\
MuscleGrapics::OutLinePass::OutLinePass() :
	Pass_Texture(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

	PipeLineDesc pipeLineDesc;

	resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/DeferredRendering.hlsli"), "VS_MAIN");

	resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/PostProcessing.hlsl"), "PS_OutLine");

	InsertShader(pipeLineDesc);
}

void MuscleGrapics::OutLinePass::SetConstants(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData)
{
	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetResource<VBIBMesh>("Texture");

	for (auto& iter : renderingData)
	{
		_d3dImmediateContext->PSSetShaderResources(iter.second, 1, &iter.first);
	}

	_d3dImmediateContext->PSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

	constexpr UINT stride = sizeof(Vertex::Texture);

	constexpr UINT offset = 0;

	Vertex::Basic* vertices;

	Vertex::Basic* verticesPtr;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Create the vertex array.
	vertices = new Vertex::Basic[6];

	vertices[0].Pos = XMFLOAT3(_left, _top, 0.0f);  // Top left.
	vertices[1].Pos = XMFLOAT3(_right, _bottom, 0.0f);  // Bottom right.
	vertices[2].Pos = XMFLOAT3(_left, _bottom, 0.0f);  // Bottom left.
	vertices[3].Pos = XMFLOAT3(_left, _top, 0.0f);  // Top left.
	vertices[4].Pos = XMFLOAT3(_right, _top, 0.0f);  // Top right.
	vertices[5].Pos = XMFLOAT3(_right, _bottom, 0.0f);  // Bottom right.

	vertices[0].Texture = XMFLOAT2(0.0f, 0.0f);
	vertices[1].Texture = XMFLOAT2(1.0f, 1.0f);
	vertices[2].Texture = XMFLOAT2(0.0f, 1.0f);
	vertices[3].Texture = XMFLOAT2(0.0f, 0.0f);
	vertices[4].Texture = XMFLOAT2(1.0f, 0.0f);
	vertices[5].Texture = XMFLOAT2(1.0f, 1.0f);

	_d3dImmediateContext->Map(*vbibMesh->GetVB(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	verticesPtr = (Vertex::Basic*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(Vertex::Basic) * 6));

	_d3dImmediateContext->Unmap(*vbibMesh->GetVB(), 0);

	delete[] vertices;

	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼

	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼

	RasterizerState::SetRasterizerState(0);
}

void MuscleGrapics::OutLinePass::Draw(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData)
{
	SetShader();

	SetConstants(renderingData);

	DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();


	_d3dImmediateContext->DrawIndexed(6, 0, 0);
}
