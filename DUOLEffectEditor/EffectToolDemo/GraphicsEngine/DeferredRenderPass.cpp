#include "pch.h"
#include "DeferredRenderPass.h"

#include "VertexDesc.h"
DeferredRenderPass::DeferredRenderPass() : PassBase<std::vector<pair<ID3D11ShaderResourceView*, int>>>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	CompileVertexShader(TEXT("Shader/DeferredRendering.hlsli"), "VS_MAIN", VertexDesc::DeferredVertexDesc, VertexDesc::DeferredVertexSize);
	CompilePixelShader(TEXT("Shader/DeferredRendering.hlsli"), "PS_DeferredRender");

	CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerFream));
}

DeferredRenderPass::~DeferredRenderPass()
{
}

void DeferredRenderPass::SetConstants(std::vector<pair<ID3D11ShaderResourceView*, int>>& renderingData)
{
	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(3);


	for (int i = 0; i < renderingData.size(); i++)
	{
		_d3dImmediateContext->PSSetShaderResources(renderingData[i].second, 1, &renderingData[i].first);
	}



	constexpr UINT stride = sizeof(Vertex::Texture);
	constexpr UINT offset = 0;

	Vertex::Basic* vertices;
	Vertex::Basic* verticesPtr;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	//float left, right, top, bottom;
	constexpr float left = -1.0f;
	constexpr float right = 1.0f;
	constexpr float top = 1.0f;
	constexpr float bottom = -1.0f;


	// Create the vertex array.
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
	// Lock the vertex buffer so it can be written to.
	// todo
	_d3dImmediateContext->Map(*vbibMesh->GetVB(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (Vertex::Basic*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(Vertex::Basic) * 6));

	// Unlock the vertex buffer.
	_d3dImmediateContext->Unmap(*vbibMesh->GetVB(), 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	ConstantBuffDesc::CB_PerFream dataPtr;
	memset(&dataPtr, 0, sizeof(ConstantBuffDesc::CB_PerFream));
	UpdateConstantBuffer(1, dataPtr);


	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼
	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼
	_d3dImmediateContext->PSSetSamplers(0, 1, &SamplerState::_wrapSamplerState);
	_d3dImmediateContext->RSSetState(RasterizerState::m_SolidRS);

}

void DeferredRenderPass::Draw(std::vector<pair<ID3D11ShaderResourceView*, int>>& renderingData)
{
	SetShader();
	SetConstants(renderingData);
	DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();
	_d3dImmediateContext->DrawIndexed(6, 0, 0);
}
