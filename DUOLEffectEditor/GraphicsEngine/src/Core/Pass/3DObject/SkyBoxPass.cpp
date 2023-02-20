#include "Core/Pass/3DObject/SkyBoxPass.h"

#include "util/VertexDesc.h"

#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/Renderer.h"
#include "Core/DirectX11/RenderTarget.h"

#include "Core/DirectX11/DepthStencil.h"
#include "Core/Resource/Resource/ParticleMesh.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/Resource/VBIBMesh.h"
#include "Core/DirectX11/RenderTexture.h"
#include "Core/DirectX11/RasterizerState.h"
#include "Core/DirectX11/SamplerState.h"

MuscleGrapics::SkyBoxPass::SkyBoxPass() : Pass_3D(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
_drawIndex(0)
{
	const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

	PipeLineDesc pipeLineDesc;

	resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/SkyBox.hlsl"), "vs_main");

	resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/SkyBox.hlsl"), "ps_main");

	InsertShader(pipeLineDesc); // Basic;

	CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerObject));

}

void MuscleGrapics::SkyBoxPass::SetConstants(RenderingData_3D& renderingData)
{
	auto albedoTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(renderingData._materialInfo._albedoTexturePath);

	_d3dImmediateContext->VSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

	_d3dImmediateContext->PSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetResource<VBIBMesh>(renderingData._objectInfo._meshName);

	auto& perfreamData = Renderer::GetPerfreamData();

	ID3D11DeviceContext* _d3dImmediateContext = DXEngine::GetInstance()->Getd3dImmediateContext();

	_drawIndex = vbibMesh->GetIndexSize();

	UINT stride = sizeof(Vertex::BasicLight); UINT offset = 0;

	ConstantBuffDesc::CB_PerObject data;

	ZeroMemory(&data, sizeof(ConstantBuffDesc::CB_PerObject));

	data.gWorld = renderingData._geoInfo._world;

	data.worldViewProj = renderingData._geoInfo._worldViewProj;

	data.gWorldInvTranspose = renderingData._geoInfo._worldInvTranspose;

	memcpy(&data.gObjectID, &renderingData._objectInfo._objectID, sizeof(UINT));

	memcpy(&data.gColor, &renderingData._materialInfo._color, sizeof(DUOLMath::Vector4));

	UpdateConstantBuffer(1, data);

	_d3dImmediateContext->PSSetShaderResources(0, 1, &albedoTex);

	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼

	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼

	RasterizerState::SetRasterizerState(static_cast<int>(renderingData._shaderInfo._rasterizerState));
}

void MuscleGrapics::SkyBoxPass::Draw(RenderingData_3D& renderingData)
{
	SetConstants(renderingData);

	SetShader(0);

	RasterizerState::SetRasterizerState(2);

	DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();

	auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

	renderTarget->SetRenderTargetView(
		nullptr,
		6,
		renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Albedo]->GetRTV(),
		renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Depth]->GetRTV(),
		renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Normal]->GetRTV(),
		renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Position]->GetRTV(),
		renderTarget->GetRenderTexture()[(int)MutilRenderTexture::MetallicRoughnessAOSpecular]->GetRTV(),
		renderTarget->GetRenderTexture()[(int)MutilRenderTexture::ObjectID]->GetRTV()
	);

	_d3dImmediateContext->DrawIndexed(_drawIndex, 0, 0);
}
