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

namespace MuscleGrapics
{
	BasicParticlePass::BasicParticlePass() : PassBase<RenderingData_Particle>(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
	{
		const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

		PipeLineDesc pipeLineDesc;

		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "StreamOutVS");

		resoureManager->CompileGeometryShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "StreamOutGS", true);

		InsertShader(pipeLineDesc);

		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "DrawVS");

		resoureManager->CompileGeometryShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawGS", false);

		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawPS");

		InsertShader(pipeLineDesc);

		std::vector<D3D_SHADER_MACRO> ps_Macros;
		ps_Macros.push_back(D3D_SHADER_MACRO("Draw_Depth", "1"));
		ps_Macros.push_back(D3D_SHADER_MACRO(NULL, NULL));

		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "DrawVS", ps_Macros);

		resoureManager->CompileGeometryShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawGS", false, ps_Macros);

		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawPS", ps_Macros);

		InsertShader(pipeLineDesc);

		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "DrawVS");

		resoureManager->CompileGeometryShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawTrailGS", false);

		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawPS");

		InsertShader(pipeLineDesc);

		CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerObject_Particle));

		CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerFream_Particle));
	}

	void BasicParticlePass::DrawStreamOut(RenderingData_Particle& renderingData)
	{
		SetShader(0); // streamOut

		SetConstants(renderingData);

		UINT offset = 0;

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

	void BasicParticlePass::DrawParticle(RenderingData_Particle& renderingData)
	{
		SetShader(1);

		SetConstants(renderingData);

		_d3dImmediateContext->OMSetBlendState(*BlendState::GetAdditiveBlendState(), nullptr, 0xffffffff);

		DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(nullptr, 1, DXEngine::GetInstance()->GetRenderTarget()->GetRenderTargetView());

		_d3dImmediateContext->DrawAuto(); // Particle을 백 버퍼에 렌더한다. 

		_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void BasicParticlePass::DrawTrail(RenderingData_Particle& renderingData)
	{
		if (!(renderingData.GetFlag() & static_cast<unsigned int>(BasicParticle::Flags::Trails))) return;

		SetShader(3);

		SetConstants(renderingData);

		auto ParticleTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(renderingData._renderer._traillTexturePath);

		_d3dImmediateContext->PSSetShaderResources(0, 1, &ParticleTex);

		_d3dImmediateContext->OMSetBlendState(*BlendState::GetAdditiveBlendState(), nullptr, 0xffffffff);

		DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(nullptr, 1, DXEngine::GetInstance()->GetRenderTarget()->GetRenderTargetView());

		_d3dImmediateContext->DrawAuto();

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

		_d3dImmediateContext->DrawAuto(); // 뎁스 버퍼에 파티클의 깊이 값을 기록한다.

		SetShader(3);

		_d3dImmediateContext->DrawAuto();

		nulltexture = nullptr;

		_d3dImmediateContext->PSSetShaderResources(1, 1, &nulltexture);

		DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(nullptr, 0);

		Renderer::EndEvent();
	}

	void BasicParticlePass::SetConstants(RenderingData_Particle& renderingData)
	{
		_d3dImmediateContext->VSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

		_d3dImmediateContext->GSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

		auto& perfreamData = Renderer::GetPerfreamData();

		DUOLMath::Matrix view = perfreamData->_cameraInfo._viewMatrix; // 카메라

		DUOLMath::Matrix proj = perfreamData->_cameraInfo._projMatrix; // 카메라
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

		auto DepthTex = RenderTarget::GetRenderTexture()[static_cast<int>(MutilRenderTexture::Depth)]->GetSRV();

		auto ParticleTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(renderingData._renderer._texturePath);

		_d3dImmediateContext->GSSetShaderResources(0, 1, &RandomTex);

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


		if (renderingData._commonInfo._firstRun)
			DXEngine::GetInstance()->GetResourceManager()->CreateParticleMesh("Particle" + renderingData._objectID);

		_particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetResource<ParticleMesh>("Particle" + renderingData._objectID);

		DrawStreamOut(renderingData);

		DrawParticle(renderingData);

		DrawTrail(renderingData);

		DrawDepth(renderingData);

		Renderer::EndEvent();
	}
}
