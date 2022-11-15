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
	//
	// Set constants.

	XMMATRIX world = renderingData._commonInfo->_transformMatrix; // 월트 메트릭스
	XMMATRIX view = perfreamData->_cameraInfo->_viewMatrix; // 카메라
	XMMATRIX proj = perfreamData->_cameraInfo->_projMatrix; // 카메라

	float static _GameTime = 0;
	_GameTime += perfreamData->_deltaTime;


	{
		ConstantBuffDesc::CB_PerObject_Particle data;

		data.gEmitPosW = Vector3(world.r[3].m128_f32[0], world.r[3].m128_f32[1], world.r[3].m128_f32[2]);
		data.gStartSpeed = renderingData._commonInfo->_startSpeed[0];
		data.gEmitDirW = Vector3(world.r[1].m128_f32[0], world.r[1].m128_f32[1], world.r[1].m128_f32[2]);
		data.gLifeTime = renderingData._commonInfo->_startLifeTime[0];
		data.gStartSize = renderingData._commonInfo->_startSize[0];
		data.gEmissiveCount = renderingData._commonInfo->_emissiveCount;
		data.gEmissiveTime = renderingData._commonInfo->_emissiveTime;
		data.gAccelW = Vector3(0.0f, 7.8f, 0.0f);

		UpdateConstantBuffer(0, data);
	}

	{
		ConstantBuffDesc::CB_PerFream_Particle data;

		data.gCameraPosW = perfreamData->_cameraInfo->_cameraWorldPosition; // 카메라의 좌표
		data.gGameTime = _GameTime; // 게임 시간
		data.gScreenXY = Vector2(DXEngine::GetInstance()->GetWidth(), DXEngine::GetInstance()->GetHeight());
		data.gTimeStep = perfreamData->_deltaTime; // 1프레임당 시간
		data.gViewProj = view * proj;

		UpdateConstantBuffer(1, data);
	}

	UINT stride = sizeof(Vertex::Particle);
	UINT offset = 0;




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
		_d3dImmediateContext->Draw(1, 0);
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
