#include "Core/Pass/BlurPass.h"

#include "Core/DirectX11/BlendState.h"
#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/SamplerState.h"
#include "Core/DirectX11/RasterizerState.h"
#include "Core/DirectX11/RenderTarget.h"
#include "Core/DirectX11/RenderTexture.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/VBIBMesh.h"



MuscleGrapics::BlurPass::BlurPass() :
	PassBase<std::vector<std::pair<ID3D11ShaderResourceView*, int>>>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

	ID3D11VertexShader* vs = nullptr;
	ID3D11InputLayout* il = nullptr;
	ID3D11PixelShader* ps = nullptr;
	ID3D11GeometryShader* gs = nullptr;

	vs = resoureManager->CompileVertexShader(TEXT("Asset/Particle/Shader/DeferredRendering.hlsli"), "VS_MAIN", VertexDesc::DeferredVertexDesc, VertexDesc::DeferredVertexSize);

	il = resoureManager->GetInputLayout(vs);

	ps = resoureManager->CompilePixelShader(TEXT("Asset/Particle/Shader/PostProcessing.hlsl"), "PS_DownScaling");

	InsertShader(vs, il, nullptr, ps, 0);

	ps = resoureManager->CompilePixelShader(TEXT("Asset/Particle/Shader/PostProcessing.hlsl"), "PS_Blur");

	InsertShader(vs, il, nullptr, ps, 1);

	CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerFream_Particle));
}

void MuscleGrapics::BlurPass::SetConstants(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData)
{
	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(3);

	for (auto& iter : renderingData)
	{
		_d3dImmediateContext->PSSetShaderResources(iter.second, 1, &iter.first);
	}

	_d3dImmediateContext->PSSetSamplers(0, 1, SamplerState::GetMinMagSamplerState());

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

	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //���ؽ� ����

	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //�ε��� ����

	RasterizerState::SetRasterizerState(0);
}
// ������ ��Ҵ� ���� ���� ���.
void MuscleGrapics::BlurPass::Draw(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData)
{
	SetShader();

	SetConstants(renderingData);

	DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();

	auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();
	int index = static_cast<int>(MutilRenderTexture::Blur2x);

	ID3D11ShaderResourceView* srv[BlurCount];
	ID3D11RenderTargetView* rtv[BlurCount];

	for (int i = 0; i < BlurCount; i++)
	{
		srv[i] = renderTarget->GetRenderTexture()[index + i]->GetSRV();
		rtv[i] = renderTarget->GetRenderTexture()[index + i]->GetRenderTargetView();
	}

	UINT widht = DXEngine::GetInstance()->GetWidth();
	UINT height = DXEngine::GetInstance()->GetHeight();

	D3D11_VIEWPORT _Viwport; // ����Ʈ

	_Viwport.TopLeftX = 0;
	_Viwport.TopLeftY = 0;
	_Viwport.MinDepth = 0;
	_Viwport.MaxDepth = 1.0f;

	auto& perfreamData = Renderer::GetPerfreamData();
	////--------------------------------------- �ٿ� �����ϸ�. ---------------------------------
	for (int i = 0; i < BlurCount; i++)
	{
		_Viwport.Width = static_cast<float>(widht / pow(2, i + 1));
		_Viwport.Height = static_cast<float>(height / pow(2, i + 1));
		_d3dImmediateContext->RSSetViewports(1, &_Viwport);

		ConstantBuffDesc::CB_PerFream_Particle data;
		data.gScreenXY = DUOLMath::Vector2(widht, height);
		UpdateConstantBuffer(0, data);

		if (i != 0)
			_d3dImmediateContext->PSSetShaderResources(0, 1, &srv[i - 1]);
		renderTarget->SetRenderTargetView(nullptr, 1, rtv[i]);
		_d3dImmediateContext->DrawIndexed(6, 0, 0);
		renderTarget->SetRenderTargetView(nullptr, 0);

	}

	////--------------------------------------- ������ʹ� �� �����ϸ�. ---------------------------------
	SetShader(1);
	_d3dImmediateContext->OMSetBlendState(*BlendState::GetSrcDestAdditiveBlendState(), nullptr, 0xffffffff);
	for (int i = 1; i < BlurCount + 1; i++)
	{
		_Viwport.Width = static_cast<float>(widht / pow(2, BlurCount - i));
		_Viwport.Height = static_cast<float>(height / pow(2, BlurCount - i));
		_d3dImmediateContext->RSSetViewports(1, &_Viwport);

		ConstantBuffDesc::CB_PerFream_Particle data;
		data.gScreenXY = DUOLMath::Vector2(widht, height);
		UpdateConstantBuffer(0, data);

		_d3dImmediateContext->PSSetShaderResources(0, 1, &srv[BlurCount - i]);
		if (i != BlurCount)
			renderTarget->SetRenderTargetView(nullptr, 1, rtv[BlurCount - i - 1]);
		else
			renderTarget->SetRenderTargetView(nullptr, 1, renderTarget->GetRenderTargetView());
		_d3dImmediateContext->DrawIndexed(6, 0, 0);
		renderTarget->SetRenderTargetView(nullptr, 0);
	}

	_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);


	_Viwport.Width = widht;
	_Viwport.Height = height;
	_d3dImmediateContext->RSSetViewports(1, &_Viwport);

}

