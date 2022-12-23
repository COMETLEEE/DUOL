#include "Core/Pass/OITParticlePass.h"

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

namespace MuscleGrapics
{
	OITParticlePass::OITParticlePass() : PassBase<RenderingData_Particle>(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
	{

		CompileVertexShader(TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "StreamOutVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize);

		CompileGeometryShader(TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "StreamOutGS", true);

		CompileVertexShader(TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "DrawVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize, 1);

		CompileGeometryShader(TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawGS", false, 1);

		CompilePixelShader(TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawDepthPeelingPS", 1);

		CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerObject_Particle));

		CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerFream_Particle));

	}

	void OITParticlePass::SetConstants(RenderingData_Particle& renderingData)
	{
		_d3dImmediateContext->VSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

		_d3dImmediateContext->GSSetSamplers(0, 1, SamplerState::GetWrapSamplerState());

		auto particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(renderingData._objectID);

		auto& perfreamData = Renderer::GetPerfreamData();

		DirectX::XMMATRIX world = renderingData._commonInfo._transformMatrix; // 월트 메트릭스

		DirectX::XMMATRIX view = perfreamData->_cameraInfo._viewMatrix; // 카메라

		DirectX::XMMATRIX proj = perfreamData->_cameraInfo._projMatrix; // 카메라

		{
			ConstantBuffDesc::CB_PerObject_Particle data;

			// --------------------------------- CommonInfo ---------------------------------------------
			data._commonInfo.gEmitDirW = DUOLMath::Vector3(world.r[1].m128_f32[0], world.r[1].m128_f32[1], world.r[1].m128_f32[2]);

			data._commonInfo.gEmitPosW = DUOLMath::Vector3(world.r[3].m128_f32[0], world.r[3].m128_f32[1], world.r[3].m128_f32[2]);

			memcpy(data._commonInfo.gStartDelay, renderingData._commonInfo._startDelay, sizeof(data._commonInfo.gStartDelay));

			memcpy(data._commonInfo.gStartLifeTime, renderingData._commonInfo._startLifeTime, sizeof(data._commonInfo.gStartLifeTime));

			memcpy(data._commonInfo.gStartSpeed, renderingData._commonInfo._startSpeed, sizeof(data._commonInfo.gStartSpeed));

			memcpy(data._commonInfo.gStartSize, renderingData._commonInfo._startSize, sizeof(data._commonInfo.gStartSize));

			memcpy(data._commonInfo.gStartRotation, renderingData._commonInfo._startRotation, sizeof(data._commonInfo.gStartRotation));

			memcpy(data._commonInfo.gStartColor, renderingData._commonInfo._startColor, sizeof(data._commonInfo.gStartColor));

			memcpy(data._commonInfo.gGravityModifier, renderingData._commonInfo._gravityModifier, sizeof(data._commonInfo.gGravityModifier));

			memcpy(&data._commonInfo.gObjectID, &renderingData._objectID, sizeof(UINT));

			data._commonInfo.gMaxParticles = renderingData._commonInfo._maxParticles;

			data._commonInfo.gDuration = renderingData._commonInfo._duration;

			data._commonInfo.gisLooping = static_cast<int>(renderingData._commonInfo._looping);

			// --------------------------------- Emission ----------------------------------------------
			data._emission.gEmissiveCount = renderingData._emission._emissiveCount;

			data._emission.gEmissiveTime = renderingData._emission._emissiveTime;

			data._commonInfo.gParticlePlayTime = renderingData._commonInfo._playTime; // 게임 시간

			// --------------------------------- Color_Over_Lifetime ----------------------------------------------

			data._coloroverLifetime.gStartColor = renderingData._color_Over_Lifetime._startColor;

			data._coloroverLifetime.gEndColor = renderingData._color_Over_Lifetime._endColor;

			// --------------------------------- Velocity_over_Lifetime ----------------------------------------------

			data._velocityoverLifetime.gVelocity = renderingData._velocity_Over_Lifetime._linearVelocity;

			// --------------------------------- Size_Over_Lifetime ----------------------------------------------

			memcpy(&data._sizeoverLifetime, &renderingData._size_Over_Lifetime, sizeof(ConstantBuffDesc::Size_Over_Lifetime));

			// --------------------------------- Size_Over_Lifetime ----------------------------------------------

			memcpy(&data._rotationoverLifetime, &renderingData._rotation_Over_Lifetime, sizeof(ConstantBuffDesc::Rotation_Over_Lifetime));

			// --------------------------------- Size_Over_Lifetime ----------------------------------------------

			memcpy(&data._textureSheetAnimation, &renderingData._texture_Sheet_Animaition, sizeof(ConstantBuffDesc::Texture_Sheet_Animation));

			UpdateConstantBuffer(0, data);
		}

		{
			ConstantBuffDesc::CB_PerFream_Particle data;

			data.gCameraPosW = perfreamData->_cameraInfo._cameraWorldPosition; // 카메라의 좌표

			data.gScreenXY = DUOLMath::Vector2(DXEngine::GetInstance()->GetWidth(), DXEngine::GetInstance()->GetHeight());

			data.gTimeStep = perfreamData->_deltaTime; // 1프레임당 시간

			data.gViewProj = view * proj;

			UpdateConstantBuffer(1, data);
		}

		UINT stride = sizeof(Vertex::Particle);

		UINT offset = 0;

		particleMesh->SetMaxParticleSize(renderingData._commonInfo._maxParticles);

		particleMesh->SetEmitterCount(renderingData._emission._emissiveCount);

		if (renderingData._commonInfo._firstRun)
			_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetInitVB(), &stride, &offset);
		else
			_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetDrawVB(), &stride, &offset);

		auto RandomTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(TEXT("RandomTex"));

		auto DepthTex = RenderTarget::GetRenderTexture()[static_cast<int>(MutilRenderTexture::Depth)]->GetSRV();

		auto ParticleTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(renderingData._commonInfo._refTexturePath);

		_d3dImmediateContext->GSSetShaderResources(0, 1, &RandomTex);

		_d3dImmediateContext->PSSetShaderResources(0, 1, &ParticleTex);

		_d3dImmediateContext->PSSetShaderResources(1, 1, &DepthTex);

		RasterizerState::SetRasterizerState(static_cast<int>(renderingData._rasterizerState));
	}

	void OITParticlePass::Draw(RenderingData_Particle& renderingData)
	{
		if (OrderIndependentTransparency::Get().GetDrawCount() == 0)
		{

			UINT stride = sizeof(Vertex::Particle);

			UINT offset = 0;

			if (renderingData._commonInfo._firstRun)
				DXEngine::GetInstance()->GetResourceManager()->InsertParticleMesh(renderingData._objectID);

			auto particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(renderingData._objectID);

			SetShader(0); // streamOut

			_d3dImmediateContext->SOSetTargets(1, particleMesh->GetStreamOutVB(), &offset);

			SetConstants(renderingData);

			DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();

			if (renderingData._commonInfo._firstRun)
				_d3dImmediateContext->Draw(renderingData._emission._emissiveCount, 0);
			else
				_d3dImmediateContext->DrawAuto();

			ID3D11Buffer* bufferArray[1] = { nullptr };

			std::swap(*particleMesh->GetDrawVB(), *particleMesh->GetStreamOutVB()); // 더블 버퍼링과 매우 흡사한 것.!!!

			_d3dImmediateContext->SOSetTargets(1, bufferArray, &offset);
		}

		SetShader(1);

		_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		//_d3dImmediateContext->OMSetBlendState(*BlendState::GetAdditiveBlendState(), nullptr, 0xffffffff);

		SetConstants(renderingData);

		OrderIndependentTransparency::Get().SetRenderTargetAndDepth();

		DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil();

		_d3dImmediateContext->DrawAuto();

		//_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	}

}
