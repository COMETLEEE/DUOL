#include "Core/Pass/Particle/BasicParticleObjectIDPass.h"
#include <d3d11.h>

#include "Export/RenderingData.h"

#include "Core/DirectX11/SamplerState.h"
#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/Renderer.h"
#include "Core/DirectX11/RenderTexture.h"
#include "Core/DirectX11/BlendState.h"
#include "Core/DirectX11/RenderTarget.h"

#include "Core/DirectX11/DepthStencil.h"
#include "Core/Resource/ParticleMesh.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/DirectX11/OrderIndependentTransparency.h"
#include "Core/Pass/ShaderFlagsManager.h"

namespace MuscleGrapics
{
	BasicParticleObjectIDPass::BasicParticleObjectIDPass() : PassBase<RenderingData_Particle>(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
	{
		CompileVertexShader(TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "DrawVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize);

		CompileGeometryShader(TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawGS", false);

		D3D_SHADER_MACRO ps_Macros[] = { "Draw_Object_ID" ,"0",NULL };
		CompilePixelShader(TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawPS", 0, ps_Macros);

		CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerObject_Particle));

		CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerFream_Particle));
	}

	void BasicParticleObjectIDPass::SetConstants(RenderingData_Particle& renderingData)
	{
		_d3dImmediateContext->GSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

		auto particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(renderingData._objectID);

		auto& perfreamData = Renderer::GetPerfreamData();

		DirectX::XMMATRIX world = renderingData._commonInfo._transformMatrix; // 월트 메트릭스

		DirectX::XMMATRIX view = perfreamData->_cameraInfo._viewMatrix; // 카메라

		DirectX::XMMATRIX proj = perfreamData->_cameraInfo._projMatrix; // 카메라

		{
			ConstantBuffDesc::CB_PerObject_Particle data(renderingData);

			UpdateConstantBuffer(1, data);
		}

		{
			ConstantBuffDesc::CB_PerFream_Particle data;

			data.gCameraPosW = perfreamData->_cameraInfo._cameraWorldPosition; // 카메라의 좌표

			data.gScreenXY = DUOLMath::Vector2(DXEngine::GetInstance()->GetWidth(), DXEngine::GetInstance()->GetHeight());

			data.gTimeStep = perfreamData->_deltaTime; // 1프레임당 시간

			data.gViewProj = view * proj;

			UpdateConstantBuffer(0, data);
		}

		UINT stride = sizeof(Vertex::Particle);

		UINT offset = 0;

		particleMesh->SetMaxParticleSize(renderingData._commonInfo._maxParticles);

		particleMesh->SetEmitterCount(renderingData._emission._emissiveCount);

		if (renderingData._commonInfo._firstRun)
			_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetInitVB(), &stride, &offset);
		else
			_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetDrawVB(), &stride, &offset);

		auto DepthTex = RenderTarget::GetRenderTexture()[static_cast<int>(MutilRenderTexture::Depth)]->GetSRV();

		auto ParticleTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(renderingData._renderer._texturePath);

		_d3dImmediateContext->PSSetShaderResources(0, 1, &ParticleTex);

		_d3dImmediateContext->PSSetShaderResources(1, 1, &DepthTex);

		RasterizerState::SetRasterizerState(static_cast<int>(renderingData._rasterizerState));
	}

	void BasicParticleObjectIDPass::Draw(RenderingData_Particle& renderingData)
	{
		unsigned int flag = renderingData.GetFlag();

		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::ParticleSystemCommonInfo))) return;
		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::Renderer))) return;
		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::Emission))) return;

		if (OrderIndependentTransparency::Get().GetDrawCount() != 0) return;

		SetShader();

		DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();

		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

		renderTarget->SetRenderTargetView(
			nullptr,
			1,
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::ObjectID]->GetRenderTargetView()
		);

		_d3dImmediateContext->DrawAuto();

	}
}
