#include "Core/Pass/BasicPass.h"

#include "util/VertexDesc.h"

#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/Renderer.h"
#include "Core/DirectX11/RenderTarget.h"

#include "Core/DirectX11/DepthStencil.h"
#include "Core/Resource/ParticleMesh.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/VBIBMesh.h"
#include "Core/DirectX11/RenderTexture.h"
namespace MuscleGrapics
{
	BasicPass::BasicPass() : PassBase<RenderingData_3D>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
		_drawIndex(0)
	{
		CompileVertexShader(TEXT("Asset/Particle/Shader/BaiscLight_VS.hlsl"), "main", VertexDesc::BasicLightVertex, VertexDesc::BasicLightVertexSize);

		CompilePixelShader(TEXT("Asset/Particle/Shader/BasicLight_PS.hlsl"), "main");

		CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerObject));
	}

	void BasicPass::SetConstants(RenderingData_3D& renderingData)
	{
		auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(renderingData._objectInfo->_meshID);

		auto& perfreamData = Renderer::GetPerfreamData();

		ID3D11DeviceContext* _d3dImmediateContext = DXEngine::GetInstance()->Getd3dImmediateContext();

		_drawIndex = vbibMesh->GetIndexSize();

		UINT stride = sizeof(Vertex::BasicLight); UINT offset = 0;

		ConstantBuffDesc::CB_PerObject data;// = static_cast<ConstantBuffDesc::CB_PerObject*>(mappedResource.pData);

		ZeroMemory(&data, sizeof(ConstantBuffDesc::CB_PerObject));

		data.gWorld = renderingData._geoInfo->_world;

		data.worldViewProj = renderingData._geoInfo->_worldViewProj;

		data.gWorldInvTranspose = renderingData._geoInfo->_worldInvTranspose;

		memcpy(&data.gObjectID, &renderingData._objectInfo->_objectID, sizeof(UINT));

		memcpy(&data.gColor, &renderingData._materialInfo->_color, sizeof(DUOLMath::Vector4));

		UpdateConstantBuffer(0, data);

		_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼

		_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼

		RasterizerState::SetRasterizerState(static_cast<int>(renderingData._shaderInfo->_rasterizerState));
	}

	void BasicPass::Draw(RenderingData_3D& renderingData)
	{
		SetShader();

		SetConstants(renderingData);

		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

		auto depth = DXEngine::GetInstance()->GetDepthStencil();

		depth->OnDepthStencil();

		renderTarget->SetRenderTargetView(
			depth->GetDepthStencilView(0),
			7,
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Depth]->GetRenderTargetView(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Normal]->GetRenderTargetView(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Position]->GetRenderTargetView(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Albedo]->GetRenderTargetView(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::MatDiffuse]->GetRenderTargetView(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::MatSpecular]->GetRenderTargetView(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::ObjectID]->GetRenderTargetView()
		);

		_d3dImmediateContext->DrawIndexed(_drawIndex, 0, 0);
	}
}
