#include "Core/Pass/Particle/OITParticlePass.h"

#include "Core/DirectX11/SamplerState.h"
#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/Renderer.h"
#include "Core/DirectX11/RenderTexture.h"
#include "Core/DirectX11/BlendState.h"
#include "Core/DirectX11/Depth.h"
#include "Core/DirectX11/RenderTarget.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ParticleMesh.h"
#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/OrderIndependentTransparency.h"
#include "Core/Pass/ShaderFlagsManager.h"
#include "Core/DirectX11/RasterizerState.h"

//D3D_SHADER_MACRO ps_Macros[] = { "Draw_Object_ID" ,"0",NULL };

namespace MuscleGrapics
{
	OITParticlePass::OITParticlePass() : PassBase<RenderingData_Particle>(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
	{
		const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

		PassDesc passDesc;

		passDesc._vs = resoureManager->CompileVertexShader(TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "StreamOutVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize);

		passDesc._il = resoureManager->GetInputLayout(passDesc._vs);

		passDesc._gs = resoureManager->CompileGeometryShader(TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "StreamOutGS", true);

		InsertShader(passDesc);

		passDesc._vs = resoureManager->CompileVertexShader(TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "DrawVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize);

		passDesc._il = resoureManager->GetInputLayout(passDesc._vs);

		passDesc._gs = resoureManager->CompileGeometryShader(TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawGS", false);

		passDesc._ps = resoureManager->CompilePixelShader(TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawDepthPeelingPS");

		passDesc._shaderIndex = 1;

		InsertShader(passDesc);

		passDesc._vs = resoureManager->CompileVertexShader(TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "DrawVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize);

		passDesc._il = resoureManager->GetInputLayout(passDesc._vs);

		passDesc._gs = resoureManager->CompileGeometryShader(TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawTrailGS", false);

		passDesc._ps = resoureManager->CompilePixelShader(TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawDepthPeelingPS");

		passDesc._shaderIndex = 2;

		InsertShader(passDesc);

		CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerObject_Particle));

		CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerFream_Particle));
	}

	void OITParticlePass::DrawStreamOut(RenderingData_Particle& renderingData)
	{
		if (OrderIndependentTransparency::Get().GetDrawCount() == 0)
		{
			UINT offset = 0;

			SetShader(0); // streamOut

			SetConstants(renderingData);

			_d3dImmediateContext->SOSetTargets(1, _particleMesh->GetStreamOutVB(), &offset);

			DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();

			if (renderingData._commonInfo._firstRun)
				_d3dImmediateContext->Draw(renderingData._emission._emissiveCount, 0);
			else
				_d3dImmediateContext->DrawAuto();

			ID3D11Buffer* bufferArray[1] = { nullptr };

			std::swap(*_particleMesh->GetDrawVB(), *_particleMesh->GetStreamOutVB()); // 더블 버퍼링과 매우 흡사한 것.!!!

			_d3dImmediateContext->SOSetTargets(1, bufferArray, &offset);
		}
	}

	void OITParticlePass::DrawParticle(RenderingData_Particle& renderingData)
	{
		if (renderingData._renderer._renderMode == Particle_Renderer::RenderMode::None) return;

		SetShader(1);

		SetConstants(renderingData);

		DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil();

		OrderIndependentTransparency::Get().SetRenderTargetAndDepth();

		_d3dImmediateContext->DrawAuto();

		_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void OITParticlePass::DrawTrail(RenderingData_Particle& renderingData)
	{
		if (!(renderingData.GetFlag() & static_cast<unsigned int>(BasicParticle::Flags::Trails))) return;

		SetShader(2);

		SetConstants(renderingData);

		auto ParticleTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(renderingData._renderer._traillTexturePath);
		//auto ParticleTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(TEXT("TESTNoise"));

		_d3dImmediateContext->PSSetShaderResources(0, 1, &ParticleTex);

		DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil();

		OrderIndependentTransparency::Get().SetRenderTargetAndDepth();

		_d3dImmediateContext->DrawAuto();

		_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void OITParticlePass::SetConstants(RenderingData_Particle& renderingData)
	{
		_d3dImmediateContext->VSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

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

		UINT stride = sizeof(Vertex::Particle);

		UINT offset = 0;

		_particleMesh->SetMaxParticleSize(renderingData._commonInfo._maxParticles);

		_particleMesh->SetEmitterCount(renderingData._emission._emissiveCount);

		if (renderingData._commonInfo._firstRun)
			_d3dImmediateContext->IASetVertexBuffers(0, 1, _particleMesh->GetInitVB(), &stride, &offset);
		else
			_d3dImmediateContext->IASetVertexBuffers(0, 1, _particleMesh->GetDrawVB(), &stride, &offset);

		auto RandomTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(TEXT("RandomTex"));

		auto NoiseTex = DXEngine::GetInstance()->GetResourceManager()->GetNoiseMap({ renderingData._noise._frequency, renderingData._noise._octaves,renderingData._noise._octaveMultiplier });

		auto DepthTex = RenderTarget::GetRenderTexture()[static_cast<int>(MutilRenderTexture::Depth)]->GetSRV();

		auto ParticleTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(renderingData._renderer._texturePath);

		_d3dImmediateContext->GSSetShaderResources(0, 1, &RandomTex);

		_d3dImmediateContext->GSSetShaderResources(1, 1, &NoiseTex);

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

		if (renderingData._commonInfo._firstRun)
			DXEngine::GetInstance()->GetResourceManager()->InsertParticleMesh(renderingData._objectID);

		_particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(renderingData._objectID);

		DrawStreamOut(renderingData);

		DrawParticle(renderingData);

		DrawTrail(renderingData);
	}

}
