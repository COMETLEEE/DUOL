#include "pch.h"
#include "TextureRenderPass.h"

#include "VertexDesc.h"

TextureRenderPass::TextureRenderPass() : PassBase<pair<ID3D11ShaderResourceView*, int>>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{

	CompileVertexShader(TEXT("Shader/DeferredRendering.hlsli"), "VS_MAIN", VertexDesc::DeferredVertexDesc, VertexDesc::DeferredVertexSize);
	CompilePixelShader(TEXT("Shader/DeferredRendering.hlsli"), "PS_TextureRender");



}

TextureRenderPass::~TextureRenderPass()
{
}

void TextureRenderPass::SetConstants(pair<ID3D11ShaderResourceView*, int>& renderingData)
{
	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(3);


	_d3dImmediateContext->PSSetShaderResources(renderingData.second, 1, &renderingData.first);

	_d3dImmediateContext->PSSetSamplers(0, 1, &SamplerState::_wrapSamplerState);


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
	SetShader();
	SetConstants(renderingData);
	DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();
	_d3dImmediateContext->DrawIndexed(6, 0, 0);
}

void TextureRenderPass::SetDrawRectangle(float left, float right, float top, float bottom)
{
	_left = left / DXEngine::GetInstance()->GetWidth() * 2 - 1;
	_right = right / DXEngine::GetInstance()->GetWidth() * 2 - 1;
	_top = -(top / DXEngine::GetInstance()->GetHeight() * 2 - 1);
	_bottom = -(bottom / DXEngine::GetInstance()->GetHeight() * 2 - 1);


}
