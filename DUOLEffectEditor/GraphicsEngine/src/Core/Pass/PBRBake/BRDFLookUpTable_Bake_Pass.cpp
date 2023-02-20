#include "Core/Pass/PBRBake/BRDFLookUpTable_Bake_Pass.h"
#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/SamplerState.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/Resource/VBIBMesh.h"
#include "Core/DirectX11/RasterizerState.h"


MuscleGrapics::BRDFLookUpTable_Bake_Pass::BRDFLookUpTable_Bake_Pass() : Pass_Texture(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

	PipeLineDesc pipeLineDesc;

	resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/DeferredRendering.hlsli"), "VS_MAIN");

	resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/Shader_BRDFLookUpTable.hlsl"), "PSMain");

	InsertShader(pipeLineDesc);
}

void MuscleGrapics::BRDFLookUpTable_Bake_Pass::SetConstants(
	std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData)
{
	RasterizerState::SetRasterizerState(0);

	_d3dImmediateContext->PSSetSamplers(1, 1, SamplerState::GetLinearWrapSamplerState());

	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetResource<VBIBMesh>("Texture");

	for (int i = 0; i < renderingData.size(); i++)
		_d3dImmediateContext->PSSetShaderResources(renderingData[i].second, 1, &renderingData[i].first);

	constexpr UINT stride = sizeof(Vertex::Texture);

	constexpr UINT offset = 0;

	Vertex::Basic* vertices;

	Vertex::Basic* verticesPtr;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	constexpr float left = -1.0f;
	constexpr float right = 1.0f;
	constexpr float top = 1.0f;
	constexpr float bottom = -1.0f;

	vertices = new Vertex::Basic[6];

	vertices[0].Pos = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[1].Pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[2].Pos = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[3].Pos = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[4].Pos = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[5].Pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.

	vertices[0].Texture = XMFLOAT2(0.0f, 0.0f);
	vertices[1].Texture = XMFLOAT2(1.0f, 1.0f);
	vertices[2].Texture = XMFLOAT2(0.0f, 1.0f);
	vertices[3].Texture = XMFLOAT2(0.0f, 0.0f);
	vertices[4].Texture = XMFLOAT2(1.0f, 0.0f);
	vertices[5].Texture = XMFLOAT2(1.0f, 1.0f);

	HR(_d3dImmediateContext->Map(*vbibMesh->GetVB(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	verticesPtr = (Vertex::Basic*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(Vertex::Basic) * 6));

	_d3dImmediateContext->Unmap(*vbibMesh->GetVB(), 0);

	delete[] vertices;

	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼

	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼

}

void MuscleGrapics::BRDFLookUpTable_Bake_Pass::Draw(
	std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData)
{
	SetShader();
	SetConstants(renderingData);
	DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();
	_d3dImmediateContext->DrawIndexed(6, 0, 0);
}
