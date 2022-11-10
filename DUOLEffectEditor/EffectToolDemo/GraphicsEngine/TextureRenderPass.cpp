#include "pch.h"
#include "TextureRenderPass.h"

#include "VertexDesc.h"

TextureRenderPass::TextureRenderPass() : PassBase<pair<ID3D11ShaderResourceView*, int>>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{

	CompileVertexShader(TEXT("Shader/DeferredRendering.hlsli"), "VS_MAIN", VertexDesc::DeferredVertexDesc, VertexDesc::DeferredVertexSize);
	CompilePixelShader(TEXT("Shader/DeferredRendering.hlsli"), "PS_TextureRender");



	/// <summary>
/// testcode
	D3D11_SAMPLER_DESC samDesc;
	auto device = DXEngine::GetInstance()->GetD3dDevice();
	samDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samDesc.MipLODBias = 0.f;
	samDesc.MaxAnisotropy = 2;
	samDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samDesc.BorderColor[0] = 0.f;
	samDesc.BorderColor[1] = 0.f;
	samDesc.BorderColor[2] = 0.f;
	samDesc.BorderColor[3] = 0.f;

	samDesc.MinLOD = -FLT_MAX;
	samDesc.MaxLOD = FLT_MAX;

	device->CreateSamplerState(&samDesc, &_wrapSamplerState);

	/// testcode

}

TextureRenderPass::~TextureRenderPass()
{
}

void TextureRenderPass::SetConstants(pair<ID3D11ShaderResourceView*, int>& renderingData)
{
	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(3);


	_d3dImmediateContext->PSSetShaderResources(renderingData.second, 1, &renderingData.first);

	_d3dImmediateContext->PSSetSamplers(0, 1, &_wrapSamplerState);


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

	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼
	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼

	_d3dImmediateContext->RSSetState(RasterizerState::m_SolidRS);
}

void TextureRenderPass::Draw(pair<ID3D11ShaderResourceView*, int>& renderingData)
{
	SetShaer();
	SetConstants(renderingData);
	DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();
	_d3dImmediateContext->DrawIndexed(6, 0, 0);
}
