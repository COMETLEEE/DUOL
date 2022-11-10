#include "pch.h"
#include "BasicParticlePass.h"

BasicParticlePass::BasicParticlePass() : PassBase<RenderingData_Particle>(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST),
	_d3dImmediateContext(DXEngine::GetInstance()->Getd3dImmediateContext()),
	_topolgy(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST),
	_inputLayout(nullptr)
{
}

void BasicParticlePass::SetConstants(RenderingData_Particle& renderingData)
{
	auto particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(
		renderingData.particleID
	);
	auto& perfreamData = Renderer::GetPerfreamData();
	//
	// Set constants.

	XMMATRIX world = renderingData._commonInfo->_transformMatrix; // 월트 메트릭스
	XMMATRIX _View = perfreamData->_cameraInfo->_viewMatrix; // 카메라
	XMMATRIX _Proj = perfreamData->_cameraInfo->_projMatrix; // 카메라

	float static _GameTime = 0;
	_GameTime += perfreamData->_deltaTime;

	//Effects::ParticleFX->WorldViewProjUpdate(world, _View, _Proj);
	//Effects::ParticleFX->SetGameTime(_GameTime);
	//Effects::ParticleFX->SetTimeStep(perfreamData->_deltaTime);
	//Effects::ParticleFX->SetEyePosW(perfreamData->_cameraInfo->_cameraWorldPosition);

	//Effects::ParticleFX->SetTexArray(DXEngine::GetInstance()->GetResourceManager()->GetTexture(renderingData->_commonInfo->_refTextureID));
	//Effects::ParticleFX->SetRandomTex(DXEngine::GetInstance()->GetResourceManager()->GetTexture(1));

	//Effects::ParticleFX->SetEmissiveCount(renderingData->_commonInfo->_emissiveCount);
	//Effects::ParticleFX->SetEmissiveTime(renderingData->_commonInfo->_emissiveTime);
	//Effects::ParticleFX->SetLifeTime(renderingData->_commonInfo->_lifeTime);
	//Effects::ParticleFX->SetParticleSize(renderingData->_commonInfo->_startSize);
	//Effects::ParticleFX->SetStartSpeed(renderingData->_commonInfo->_startSpeed);
	//Effects::ParticleFX->SetDepthMap(RenderTarget::GetRenderTexture()[static_cast<int>(MutilRenderTexture::Depth)]->GetSRV());


	//
	// Set IA stage.
	//
	_d3dImmediateContext->IASetInputLayout(_inputLayout);
	_d3dImmediateContext->IASetPrimitiveTopology(_topolgy);

	UINT stride = sizeof(Vertex::Particle);
	UINT offset = 0;

	if (!_testfirstRun)
		_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetInitVB(), &stride, &offset);
	else
		_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetDrawVB(), &stride, &offset);

	_d3dImmediateContext->SOSetTargets(1, particleMesh->GetStreamOutVB(), &offset);
}

void BasicParticlePass::Draw(RenderingData_Particle& renderingData)
{

	SetConstants(renderingData);
	auto particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(
		renderingData.particleID
	);


	//D3DX11_TECHNIQUE_DESC techDesc;
	//Effects::ParticleFX->_streamOutTech->GetDesc(&techDesc);
	//for (UINT p = 0; p < techDesc.Passes; ++p)
	//{
		//Effects::ParticleFX->_streamOutTech->GetPassByIndex(p)->Apply(0, _d3dImmediateContext);
		if (!_testfirstRun)
		{
			_d3dImmediateContext->Draw(1, 0);
			_testfirstRun = true;
		}
		else
			_d3dImmediateContext->DrawAuto();
	//}

	UINT stride = sizeof(Vertex::Particle);
	UINT offset = 0;

	//// done streaming-out--unbind the vertex buffer
	ID3D11Buffer* bufferArray[1] = { nullptr };
	_d3dImmediateContext->SOSetTargets(1, bufferArray, &offset);

	// 더블 버퍼링과 매우 흡사한 것.!!!
	std::swap(*particleMesh->GetDrawVB(), *particleMesh->GetStreamOutVB());


	_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetDrawVB(), &stride, &offset);

	//Effects::ParticleFX->_drawTech->GetDesc(&techDesc);
	//for (UINT p = 0; p < techDesc.Passes; ++p)
	//{
		//Effects::ParticleFX->_drawTech->GetPassByIndex(p)->Apply(0, _d3dImmediateContext);

		_d3dImmediateContext->DrawAuto();
	//}
	_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}
