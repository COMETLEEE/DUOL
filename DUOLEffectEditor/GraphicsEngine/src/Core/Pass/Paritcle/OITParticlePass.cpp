#include "Core/Pass/Particle/OITParticlePass.h"

#include "Core/DirectX11/SamplerState.h"
#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/Renderer.h"
#include "Core/DirectX11/RenderTexture.h"
#include "Core/DirectX11/BlendState.h"
#include "Core/DirectX11/Depth.h"
#include "Core/DirectX11/RenderTarget.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/Resource/ParticleMesh.h"
#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/OrderIndependentTransparency.h"
#include "Core/Pass/ShaderFlagsManager.h"
#include "Core/DirectX11/RasterizerState.h"
#include "Core/Resource/Resource/ShaderClassInstance.h"
//D3D_SHADER_MACRO ps_Macros[] = { "Draw_Object_ID" ,"0",NULL };

namespace MuscleGrapics
{
	OITParticlePass::OITParticlePass() : Pass_Particle(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
	{
		const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

		PipeLineDesc pipeLineDesc;
		// --------------------------- ComputeShader Particle Update -------------------------------------------0
		resoureManager->CompileComputeShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_CS.hlsl"), "CS_Main");
		InsertShader(pipeLineDesc);

		// --------------------------- DepthPelling Draw -------------------------------------------1
		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "ComputeShaderDrawVS");
		resoureManager->CompileGeometryShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawGS", false);
		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawDepthPeelingPS");
		InsertShader(pipeLineDesc);
		// --------------------------- Trail DepthPelling Draw -------------------------------------------2
		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "ComputeShaderDrawVS");
		resoureManager->CompileGeometryShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawTrailGS", false);
		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawDepthPeelingPS");
		InsertShader(pipeLineDesc);
		// --------------------------- ResetParticle -------------------------------------------3
		resoureManager->CompileComputeShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_CS.hlsl"), "CS_ResetParticleBuffer");
		InsertShader(pipeLineDesc);
		// --------------------------- ClearTimer -------------------------------------------4
		resoureManager->CompileComputeShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_CS.hlsl"), "CS_ClearCounter");
		InsertShader(pipeLineDesc);

		CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerFream_Particle));

		CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerObject_Particle));

		CreateConstantBuffer(2, sizeof(ConstantBuffDesc::CB_DynamicBuffer));
	}

	void OITParticlePass::ParticleUpdate(RenderingData_Particle& renderingData)
	{
		if (OrderIndependentTransparency::Get().GetDrawCount() == 0)
		{
			// -------------- 카운터 초기화. ---------------------------
			SetShader(4);

			_particleMesh->ResetCounter();

			SetConstants(renderingData);

			SetShader(0); // streamOut

			auto& perfreamData = Renderer::GetPerfreamData();

			renderingData._emission._emissiveTimer += perfreamData.get()->_deltaTime;

			ConstantBuffDesc::CB_DynamicBuffer data;

			data.g_dim = _particleMesh->GetDim();

			data.g_EmiitionTime = renderingData._emission._emissiveTimer;

			UpdateConstantBuffer(2, data);

			if (renderingData._emission._emissiveTimer >= renderingData._emission._emissiveTime)
				renderingData._emission._emissiveTimer = 0;

			_particleMesh->ParticleUpdate();
		}
	}

	void OITParticlePass::DrawParticle(RenderingData_Particle& renderingData)
	{
		if (renderingData._renderer._renderMode == Particle_Renderer::RenderMode::None) return;

		SetConstants(renderingData);

		SetShader(1);

		DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil();

		OrderIndependentTransparency::Get().SetRenderTargetAndDepth();

		_d3dImmediateContext->Draw(renderingData._commonInfo._maxParticles, 0);

		_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void OITParticlePass::DrawTrail(RenderingData_Particle& renderingData)
	{
		if (!(renderingData.GetFlag() & static_cast<unsigned int>(BasicParticle::Flags::Trails))) return;

		SetConstants(renderingData);

		SetShader(2);

		auto ParticleTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(renderingData._renderer._traillTexturePath);

		_d3dImmediateContext->PSSetShaderResources(0, 1, &ParticleTex);

		DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil();

		OrderIndependentTransparency::Get().SetRenderTargetAndDepth();

		_d3dImmediateContext->Draw(renderingData._commonInfo._maxParticles, 0);

		_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void OITParticlePass::SetConstants(RenderingData_Particle& renderingData)
	{
		_d3dImmediateContext->VSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

		_d3dImmediateContext->CSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

		_d3dImmediateContext->GSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

		auto& perfreamData = Renderer::GetPerfreamData();

		{
			ConstantBuffDesc::CB_PerObject_Particle data(renderingData);

			UpdateConstantBuffer(1, data);
		}
		{
			ConstantBuffDesc::CB_PerFream_Particle data(*perfreamData);

			UpdateConstantBuffer(0, data);
		}

		_particleMesh->SetMaxParticleSize(renderingData._commonInfo._maxParticles);

		if (renderingData._commonInfo._firstRun)
		{
			renderingData._commonInfo._firstRun = false;
			SetShader(3);
			_particleMesh->ResetParticleBuffer();
		}

		_particleMesh->VSSetResource();

		auto RandomTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(TEXT("RandomTex"));

		auto NoiseTex = DXEngine::GetInstance()->GetResourceManager()->GetNoiseMap({ renderingData._noise._frequency, renderingData._noise._octaves,renderingData._noise._octaveMultiplier });

		auto DepthTex = RenderTarget::GetRenderTexture()[static_cast<int>(MutilRenderTexture::Depth)]->GetSRV();

		auto ParticleTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(renderingData._renderer._texturePath);

		_d3dImmediateContext->CSSetShaderResources(0, 1, &RandomTex);

		_d3dImmediateContext->CSSetShaderResources(1, 1, &NoiseTex);

		_d3dImmediateContext->PSSetShaderResources(0, 1, &ParticleTex);

		_d3dImmediateContext->PSSetShaderResources(1, 1, &DepthTex);

		RasterizerState::SetRasterizerState(static_cast<int>(renderingData._rasterizerState));
	}

	void OITParticlePass::Draw(RenderingData_Particle& renderingData)
	{
		unsigned int flag = renderingData.GetFlag();

		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::ParticleSystemCommonInfo))) return;
		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::Renderer))) return;
		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::Emission))) return;

		std::string str = "Particle" + std::to_string(renderingData._objectID);

		if (renderingData._commonInfo._firstRun)
			DXEngine::GetInstance()->GetResourceManager()->CreateParticleMesh(str);

		_particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetResource<ParticleMesh>(str);

		ParticleUpdate(renderingData);

		DrawParticle(renderingData);

		DrawTrail(renderingData);
	}

}
