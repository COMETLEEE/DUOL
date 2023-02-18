#include "Core/Pass/Particle/BasicParticlePass.h"

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

namespace MuscleGrapics
{
	BasicParticlePass::BasicParticlePass() : Pass_Particle(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
	{
		const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

		PipeLineDesc pipeLineDesc;
		// --------------------------- ComputeShader Particle Update -------------------------------------------0
		resoureManager->CompileComputeShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_CS.hlsl"), "CS_Main");
		InsertShader(pipeLineDesc);

		// --------------------------- Particle PS Draw  -------------------------------------------1
		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "ComputeShaderDrawVS");
		resoureManager->CompileGeometryShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawGS", false);
		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawPS");
		InsertShader(pipeLineDesc);
		// --------------------------- Trail PS Draw   -------------------------------------------2
		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "ComputeShaderDrawVS");
		resoureManager->CompileGeometryShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawTrailGS", false);
		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawPS");
		InsertShader(pipeLineDesc);

		std::vector<D3D_SHADER_MACRO> ps_Macros;
		ps_Macros.push_back(D3D_SHADER_MACRO("Draw_Depth", "1"));
		ps_Macros.push_back(D3D_SHADER_MACRO(NULL, NULL));
		// --------------------------- Depth Particle Draw ------------------------------------------- 뎁스를 또 그리는 이유는 OIT와 섞어서 사용하기 위함.
		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "ComputeShaderDrawVS", ps_Macros);
		resoureManager->CompileGeometryShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawGS", false, ps_Macros);
		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawPS", ps_Macros);
		InsertShader(pipeLineDesc);

		CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerFream_Particle));

		CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerObject_Particle));

		CreateConstantBuffer(2, sizeof(ConstantBuffDesc::CB_DynamicBuffer));
	}

	void BasicParticlePass::ParticleUpdate(RenderingData_Particle& renderingData)
	{
		// -------------- 카운터 초기화. ---------------------------
		_particleMesh->ResetCounter();
		// -------------- 카운터 초기화. ---------------------------
		SetConstants(renderingData);

		SetShader(0); // streamOut

		ConstantBuffDesc::CB_DynamicBuffer data;

		data.g_dim = _particleMesh->GetDim();

		data.g_EmiitionTime = renderingData._emission._emissiveTimer;

		UpdateConstantBuffer(2, data);

		_particleMesh->ParticleUpdate();

		renderingData._particleCount = _particleMesh->GetParticleCount();
	}

	void BasicParticlePass::DrawParticle(RenderingData_Particle& renderingData)
	{
		SetConstants(renderingData);

		SetShader(1);

		_d3dImmediateContext->OMSetBlendState(*BlendState::GetAdditiveBlendState(), nullptr, 0xffffffff);

		_d3dImmediateContext->Draw(renderingData._commonInfo._maxParticles, 0); // Particle을 백 버퍼에 렌더한다. 

		_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	}

	void BasicParticlePass::DrawTrail(RenderingData_Particle& renderingData)
	{
		if (!(renderingData.GetFlag() & static_cast<unsigned int>(BasicParticle::Flags::Trails))) return;

		SetConstants(renderingData);

		SetShader(2);

		auto ParticleTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(renderingData._renderer._traillTexturePath);

		_d3dImmediateContext->PSSetShaderResources(0, 1, &ParticleTex);

		_d3dImmediateContext->OMSetBlendState(*BlendState::GetAdditiveBlendState(), nullptr, 0xffffffff);

		_d3dImmediateContext->Draw(renderingData._commonInfo._maxParticles, 0);

		_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void BasicParticlePass::DrawDepth(RenderingData_Particle& renderingData)
	{
		Renderer::BeginEvent(TEXT("Depth_Draw"));

		SetShader(2);

		DXEngine::GetInstance()->GetRenderTarget()->GetRenderTexture()[static_cast<int>(MutilRenderTexture::NullTexture)]->ClearRenderTarget();

		auto nulltexture = DXEngine::GetInstance()->GetRenderTarget()->GetRenderTexture()[static_cast<int>(MutilRenderTexture::NullTexture)]->GetSRV();

		auto DepthTex = RenderTarget::GetRenderTexture()[static_cast<int>(MutilRenderTexture::Depth)]->GetRenderTargetView();

		_d3dImmediateContext->PSSetShaderResources(1, 1, &nulltexture);

		DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(DXEngine::GetInstance()->GetDepthStencil()->GetDepthStencilView(0), 1, DepthTex);

		DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil();

		_d3dImmediateContext->Draw(renderingData._commonInfo._maxParticles, 0);

		SetShader(3);

		_d3dImmediateContext->Draw(renderingData._commonInfo._maxParticles, 0);

		nulltexture = nullptr;

		_d3dImmediateContext->PSSetShaderResources(1, 1, &nulltexture);

		DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(nullptr, 0);

		Renderer::EndEvent();
	}

	void BasicParticlePass::SetConstants(RenderingData_Particle& renderingData)
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
			_particleMesh->ResetParticleBuffer();

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

	void BasicParticlePass::Draw(RenderingData_Particle& renderingData)
	{
		Renderer::BeginEvent(TEXT("BasicParticlePass"));

		unsigned int flag = renderingData.GetFlag();

		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::ParticleSystemCommonInfo))) return;
		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::Renderer))) return;
		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::Emission))) return;

		std::string str = "Particle" + std::to_string(renderingData._objectID);

		if (renderingData._commonInfo._firstRun)
			DXEngine::GetInstance()->GetResourceManager()->CreateParticleMesh(str);

		_particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetResource<ParticleMesh>(str);

		ParticleUpdate(renderingData);

		auto rtv = DXEngine::GetInstance()->GetRenderTarget()->GetDeferredRTV();

		_d3dImmediateContext->OMSetRenderTargetsAndUnorderedAccessViews(1, &rtv,
			nullptr, 0, 0, nullptr, nullptr);

		DrawParticle(renderingData);

		DrawTrail(renderingData);

		//DrawDepth(renderingData);

		Renderer::EndEvent();
	}
}
