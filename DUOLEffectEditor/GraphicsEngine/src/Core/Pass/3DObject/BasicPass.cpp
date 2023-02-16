#include "Core/Pass/3DObject/BasicPass.h"

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

#include "Core/DirectX11/OrderIndependentTransparency.h"
namespace MuscleGrapics
{
	BasicPass::BasicPass() : Pass_3D(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
		_drawIndex(0)
	{
		const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

		PipeLineDesc pipeLineDesc;

		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BaiscLight_VS.hlsl"), "main");

		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicLight_PS.hlsl"), "main");

		InsertShader(pipeLineDesc); // Basic;

		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BaiscLight_VS.hlsl"), "main");

		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicLight_PS.hlsl"), "OIT_BasicLight_PS");

		InsertShader(pipeLineDesc); // OIT;

		CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerObject));
	}

	void BasicPass::SetConstants(RenderingData_3D& renderingData)
	{
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

		_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼

		_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼

		RasterizerState::SetRasterizerState(static_cast<int>(renderingData._shaderInfo._rasterizerState));
	}

	void BasicPass::Draw(RenderingData_3D& renderingData)
	{
		SetConstants(renderingData);

		switch (renderingData._shaderInfo._blendState)
		{
		case ShaderInfo::BLENDDATA_TYPE::None:
		{
			SetShader(0);

			DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil();

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
			break;
		}
		case ShaderInfo::BLENDDATA_TYPE::OIT:
		{
			SetShader(1);

			DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil();

			RasterizerState::SetRasterizerState(2);

			auto DepthTex = RenderTarget::GetRenderTexture()[static_cast<int>(MutilRenderTexture::Depth)]->GetSRV();

			_d3dImmediateContext->PSSetShaderResources(1, 1, &DepthTex);

			OrderIndependentTransparency::Get().BindingResource_UAV();

			break;
		}
		case ShaderInfo::BLENDDATA_TYPE::AlphaSort:

			break;
		default:
			break;
		}


		_d3dImmediateContext->DrawIndexed(_drawIndex, 0, 0);
	}
}
