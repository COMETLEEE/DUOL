#include "pch.h"
#include "BasicParticlePass.h"

#include "VertexDesc.h"

BasicParticlePass::BasicParticlePass() : PassBase<RenderingData_Particle>(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
{
	CompileVertexShader(TEXT("Shader/BasicParticle_VS.hlsl"), "StreamOutVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize);

	CompileGeometryShader(TEXT("Shader/BasicParticle_GS.hlsl"), "StreamOutGS", true);

	CompileVertexShader(TEXT("Shader/BasicParticle_VS.hlsl"), "DrawVS", VertexDesc::BasicParticleVertex, VertexDesc::BasicParticleVertexSize, 1);

	CompileGeometryShader(TEXT("Shader/BasicParticle_GS.hlsl"), "DrawGS", false, 1);

	CompilePixelShader(TEXT("Shader/BasicParticle_PS.hlsl"), "DrawPS", 1);

	CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerObject_Particle));

	CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerFream_Particle));
}

void BasicParticlePass::SetConstants(RenderingData_Particle& renderingData)
{
	_d3dImmediateContext->VSSetSamplers(0, 1, &SamplerState::_wrapSamplerState);

	_d3dImmediateContext->GSSetSamplers(0, 1, &SamplerState::_wrapSamplerState);

	//_d3dImmediateContext->PSSetSamplers(0, 1, &SamplerState::_wrapSamplerState);

	auto particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(renderingData._objectID);

	auto& perfreamData = Renderer::GetPerfreamData();

	// Set constants.

	XMMATRIX world = renderingData._commonInfo->_transformMatrix; // 월트 메트릭스

	XMMATRIX view = perfreamData->_cameraInfo->_viewMatrix; // 카메라

	XMMATRIX proj = perfreamData->_cameraInfo->_projMatrix; // 카메라

	{
		ConstantBuffDesc::CB_PerObject_Particle data;

		// --------------------------------- CommonInfo ---------------------------------------------
		data._commonInfo.gEmitDirW = Vector3(world.r[1].m128_f32[0], world.r[1].m128_f32[1], world.r[1].m128_f32[2]);

		data._commonInfo.gEmitPosW = Vector3(world.r[3].m128_f32[0], world.r[3].m128_f32[1], world.r[3].m128_f32[2]);

		memcpy(data._commonInfo.gStartDelay, renderingData._commonInfo->_startDelay, sizeof(data._commonInfo.gStartDelay));

		memcpy(data._commonInfo.gStartLifeTime, renderingData._commonInfo->_startLifeTime, sizeof(data._commonInfo.gStartLifeTime));

		memcpy(data._commonInfo.gStartSpeed, renderingData._commonInfo->_startSpeed, sizeof(data._commonInfo.gStartSpeed));

		memcpy(data._commonInfo.gStartSize, renderingData._commonInfo->_startSize, sizeof(data._commonInfo.gStartSize));

		memcpy(data._commonInfo.gStartRotation, renderingData._commonInfo->_startRotation, sizeof(data._commonInfo.gStartRotation));

		memcpy(data._commonInfo.gStartColor, renderingData._commonInfo->_startColor, sizeof(data._commonInfo.gStartColor));

		memcpy(data._commonInfo.gGravityModifier, renderingData._commonInfo->_gravityModifier, sizeof(data._commonInfo.gGravityModifier));

		data._commonInfo.gMaxParticles = renderingData._commonInfo->_maxParticles;

		data._commonInfo.gDuration = renderingData._commonInfo->_duration;

		data._commonInfo.gisLooping = static_cast<int>(renderingData._commonInfo->_looping);

		// --------------------------------- Emission ----------------------------------------------
		data._emission.gEmissiveCount = renderingData._emission->_emissiveCount;

		data._emission.gEmissiveTime = renderingData._emission->_emissiveTime;

		data._commonInfo.gParticlePlayTime = renderingData._commonInfo->_playTime; // 게임 시간

		// --------------------------------- Color_Over_Lifetime ----------------------------------------------

		data._coloroverLifetime.gStartColor = renderingData._color_Over_Lifetime->_startColor;

		data._coloroverLifetime.gEndColor = renderingData._color_Over_Lifetime->_endColor;

		// --------------------------------- Velocity_over_Lifetime ----------------------------------------------

		data._velocityoverLifetime.gVelocity = renderingData._velocity_Over_Lifetime->_linearVelocity;

		// --------------------------------- Size_Over_Lifetime ----------------------------------------------

		memcpy(&data._sizeoverLifetime, renderingData._size_Over_Lifetime.get(), sizeof(ConstantBuffDesc::Size_Over_Lifetime));

		// --------------------------------- Size_Over_Lifetime ----------------------------------------------

		memcpy(&data._rotationoverLifetime, renderingData._rotation_Over_Lifetime.get(), sizeof(ConstantBuffDesc::Rotation_Over_Lifetime));

		int test = sizeof(ConstantBuffDesc::CB_PerObject_Particle);
		int a = sizeof(ConstantBuffDesc::CommonInfo);
		int b = sizeof(ConstantBuffDesc::Emission);
		int c = sizeof(ConstantBuffDesc::Color_over_Lifetime);
		UpdateConstantBuffer(0, data);
	}

	{
		ConstantBuffDesc::CB_PerFream_Particle data;

		data.gCameraPosW = perfreamData->_cameraInfo->_cameraWorldPosition; // 카메라의 좌표

		data.gScreenXY = Vector2(DXEngine::GetInstance()->GetWidth(), DXEngine::GetInstance()->GetHeight());

		data.gTimeStep = perfreamData->_deltaTime; // 1프레임당 시간

		data.gViewProj = view * proj;

		UpdateConstantBuffer(1, data);
	}

	UINT stride = sizeof(Vertex::Particle);

	UINT offset = 0;

	particleMesh->SetMaxParticleSize(renderingData._commonInfo->_maxParticles);

	particleMesh->SetEmitterCount(renderingData._emission->_emissiveCount);

	if (renderingData._commonInfo->_firstRun)
		_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetInitVB(), &stride, &offset);
	else
		_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetDrawVB(), &stride, &offset);


	auto RandomTex = DXEngine::GetInstance()->GetResourceManager()->GetTexture(TEXT("RandomTex"));

	auto DepthTex = RenderTarget::GetRenderTexture()[static_cast<int>(MutilRenderTexture::Depth)]->GetSRV();

	auto ParticleTex = static_cast<ID3D11ShaderResourceView*>(renderingData._commonInfo->_refTextureID);

	_d3dImmediateContext->GSSetShaderResources(0, 1, &RandomTex);

	_d3dImmediateContext->PSSetShaderResources(0, 1, &ParticleTex);

	_d3dImmediateContext->PSSetShaderResources(1, 1, &DepthTex);


}

void BasicParticlePass::Draw(RenderingData_Particle& renderingData)
{
	UINT stride = sizeof(Vertex::Particle);

	UINT offset = 0;

	auto particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(renderingData._objectID);

	_d3dImmediateContext->OMSetBlendState(BlendState::_srcAdditiveBlendState, nullptr, 0xffffffff);

	SetShader(0); // streamOut

	_d3dImmediateContext->SOSetTargets(1, particleMesh->GetStreamOutVB(), &offset);

	SetConstants(renderingData);

	DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();

	if (renderingData._commonInfo->_firstRun)
		_d3dImmediateContext->Draw(renderingData._emission->_emissiveCount, 0);
	else
		_d3dImmediateContext->DrawAuto();

	SetShader(1); // Draw

	ID3D11Buffer* bufferArray[1] = { nullptr };

	std::swap(*particleMesh->GetDrawVB(), *particleMesh->GetStreamOutVB()); // 더블 버퍼링과 매우 흡사한 것.!!!

	_d3dImmediateContext->SOSetTargets(1, bufferArray, &offset);

	DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil(0);

	SetConstants(renderingData);

	_d3dImmediateContext->DrawAuto();

	_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

}
