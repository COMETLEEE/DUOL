#include "Core/Pass/Particle/ParticleOutLinePass.h"
#include "Core/DirectX11/SamplerState.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ParticleMesh.h"
#include "Core/DirectX11/OrderIndependentTransparency.h"
#include "Core/DirectX11/RenderTarget.h"
#include "Core/Pass/ShaderFlagsManager.h"
#include "Core/DirectX11/RenderTexture.h"
#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/RasterizerState.h"

namespace MuscleGrapics
{
	ParticleOutLinePass::ParticleOutLinePass() : PassBase<RenderingData_Particle>(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
	{
		const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

		ID3D11VertexShader* vs = nullptr;
		ID3D11InputLayout* il = nullptr;
		ID3D11PixelShader* ps = nullptr;
		ID3D11GeometryShader* gs = nullptr;

		vs = resoureManager->CompileVertexShader(TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "DrawVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize);

		il = resoureManager->GetInputLayout(vs);

		gs = resoureManager->CompileGeometryShader(TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawGS", false);

		ps = resoureManager->CompilePixelShader(TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawOutLine");

		InsertShader(vs, il, gs, ps, 0);

		CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerObject_Particle));

		CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerFream_Particle));
	}

	void ParticleOutLinePass::SetConstants(RenderingData_Particle& renderingData)
	{
		_d3dImmediateContext->VSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

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

	void ParticleOutLinePass::Draw(RenderingData_Particle& renderingData)
	{
		unsigned int flag = renderingData.GetFlag();

		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::ParticleSystemCommonInfo))) return;
		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::Renderer))) return;
		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::Emission))) return;

		if (OrderIndependentTransparency::Get().GetDrawCount() != 0) return;

		SetShader();

		_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

		DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();
		
		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

		renderTarget->SetRenderTargetView(
			nullptr,
			1,
			renderTarget->GetRenderTexture()[static_cast<int>(MutilRenderTexture::OutLine)]->GetRenderTargetView()
		);

		_d3dImmediateContext->DrawAuto();

	}
}
