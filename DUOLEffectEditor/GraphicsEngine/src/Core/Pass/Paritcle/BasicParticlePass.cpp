#include "Core/Pass/Particle/BasicParticlePass.h"

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

namespace MuscleGrapics
{
	BasicParticlePass::BasicParticlePass() : PassBase<RenderingData_Particle>(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
	{
		const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

		ID3D11VertexShader* vs = nullptr;
		ID3D11InputLayout* il = nullptr;
		ID3D11PixelShader* ps = nullptr;
		ID3D11GeometryShader* gs = nullptr;

		vs = resoureManager->CompileVertexShader(TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "StreamOutVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize);

		il = resoureManager->GetInputLayout(vs);

		gs = resoureManager->CompileGeometryShader(TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "StreamOutGS", true);

		InsertShader(vs, il, gs, ps, 0);

		vs = resoureManager->CompileVertexShader(TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "DrawVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize);

		il = resoureManager->GetInputLayout(vs);

		gs = resoureManager->CompileGeometryShader(TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawGS", false);

		ps = resoureManager->CompilePixelShader(TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawPS");

		InsertShader(vs, il, gs, ps, 1);

		std::vector<D3D_SHADER_MACRO> ps_Macros;
		ps_Macros.push_back(D3D_SHADER_MACRO("Draw_Depth", "1"));
		ps_Macros.push_back(D3D_SHADER_MACRO(NULL, NULL));

		vs = resoureManager->CompileVertexShader(TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "DrawVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize, ps_Macros);

		il = resoureManager->GetInputLayout(vs);

		gs = resoureManager->CompileGeometryShader(TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawGS", false, ps_Macros);

		ps = resoureManager->CompilePixelShader(TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawPS", ps_Macros);

		InsertShader(vs, il, gs, ps, 2);

		vs = resoureManager->CompileVertexShader(TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "DrawVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize);

		il = resoureManager->GetInputLayout(vs);

		gs = resoureManager->CompileGeometryShader(TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawTrailGS", false);

		ps = resoureManager->CompilePixelShader(TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawPS");

		InsertShader(vs, il, gs, ps, 3);

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

		std::swap(*_particleMesh->GetDrawVB(), *_particleMesh->GetStreamOutVB()); // ���� ���۸��� �ſ� ����� ��.!!!

		_d3dImmediateContext->SOSetTargets(1, bufferArray, &offset);
	}

	void BasicParticlePass::DrawParticle(RenderingData_Particle& renderingData)
	{
		SetShader(1);

		SetConstants(renderingData);

		_d3dImmediateContext->OMSetBlendState(*BlendState::GetAdditiveBlendState(), nullptr, 0xffffffff);

		DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(nullptr, 1, DXEngine::GetInstance()->GetRenderTarget()->GetRenderTargetView());

		_d3dImmediateContext->DrawAuto(); // Particle�� �� ���ۿ� �����Ѵ�. 

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

		_d3dImmediateContext->DrawAuto(); // ���� ���ۿ� ��ƼŬ�� ���� ���� ����Ѵ�.

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

		DUOLMath::Matrix view = perfreamData->_cameraInfo._viewMatrix; // ī�޶�

		DUOLMath::Matrix proj = perfreamData->_cameraInfo._projMatrix; // ī�޶�
		{
			ConstantBuffDesc::CB_PerObject_Particle data(renderingData);

			UpdateConstantBuffer(1, data);
		}
		{
			ConstantBuffDesc::CB_PerFream_Particle data;

			data.gCameraPosW = perfreamData->_cameraInfo._cameraWorldPosition; // ī�޶��� ��ǥ

			data.gScreenXY = DUOLMath::Vector2(DXEngine::GetInstance()->GetWidth(), DXEngine::GetInstance()->GetHeight());

			data.gTimeStep = perfreamData->_deltaTime; // 1�����Ӵ� �ð�

			data.gViewProj = view * proj;

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
			DXEngine::GetInstance()->GetResourceManager()->InsertParticleMesh(renderingData._objectID);

		_particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(renderingData._objectID);

		DrawStreamOut(renderingData);

		DrawParticle(renderingData);

		DrawTrail(renderingData);

		DrawDepth(renderingData);

		Renderer::EndEvent();
	}
}
