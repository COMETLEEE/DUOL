#include "pch.h"
#include "BasicParticlePass.h"

BasicParticlePass::BasicParticlePass() :
	_d3dImmediateContext(DXEngine::GetInstance()->Getd3dImmediateContext()),
	_topolgy(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	_inputLayout(InputLayout::BasicParticle)
{
}

void BasicParticlePass::SetConstants(std::shared_ptr<RenderingData_Particle>& renderingData)
{
	auto particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(
		renderingData->particleID
	);
	auto& perfreamData = Renderer::GetPerfreamData();
	//
	// Set constants.

	XMMATRIX world = renderingData->_shapeInfo->_transformMatrix; // 월트 메트릭스
	XMMATRIX _View = perfreamData->_cameraInfo->_viewMatrix; // 카메라
	XMMATRIX _Proj = perfreamData->_cameraInfo->_projMatrix; // 카메라

	Effects::ParticleFX->WorldViewProjUpdate(world, _View, _Proj);
	Effects::ParticleFX->SetGameTime(perfreamData->_deltaTime);
	Effects::ParticleFX->SetTimeStep(perfreamData->_deltaTime);
	Effects::ParticleFX->SetEyePosW(perfreamData->_cameraInfo->_cameraWorldPosition);



	//Effects::ParticleFX->SetTexArray(DXEngine::GetInstance()->GetResourceManager()->GetTexture(renderingData->_shapeInfo->_refTextureID));
	//Effects::ParticleFX->SetRandomTex(DXEngine::GetInstance()->GetResourceManager()->GetTexture(1));

	//
	// Set IA stage.
	//
	_d3dImmediateContext->IASetInputLayout(_inputLayout);
	_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(Vertex::Particle);
	UINT offset = 0;

	// On the first pass, use the initialization VB.  Otherwise, use
	// the VB that contains the current particle list.
	//if (renderingData->_initInfo->_firstRun)
	if (_testfirstRun)
		_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetInitVB(), &stride, &offset);
	else
		_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetDrawVB(), &stride, &offset);

	//
	// Draw the current particle list using stream-out only to update them.  
	// The updated vertices are streamed-out to the target VB. 
	//
	_d3dImmediateContext->SOSetTargets(1, particleMesh->GetStreamOutVB(), &offset);
}

void BasicParticlePass::Draw(std::shared_ptr<RenderingData_Particle>& renderingData)
{

	SetConstants(renderingData);
	auto particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(
		renderingData->particleID
	);

	
	D3DX11_TECHNIQUE_DESC techDesc;
	Effects::ParticleFX->_streamOutTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		Effects::ParticleFX->_streamOutTech->GetPassByIndex(p)->Apply(0, _d3dImmediateContext);
		if (!_testfirstRun)
		{
			_d3dImmediateContext->Draw(1, 0);
			_testfirstRun = true;
		}
		else
			_d3dImmediateContext->DrawAuto();
	}

	UINT stride = sizeof(Vertex::Particle);
	UINT offset = 0;

	//// done streaming-out--unbind the vertex buffer
	ID3D11Buffer* bufferArray[1] = { nullptr };
	_d3dImmediateContext->SOSetTargets(1, bufferArray, &offset);

	// 더블 버퍼링과 매우 흡사한 것.!!!
	std::swap(*particleMesh->GetDrawVB(), *particleMesh->GetStreamOutVB());

	_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetDrawVB(), &stride, &offset);

	Effects::ParticleFX->_drawTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil(p);
		DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(p);
		Effects::ParticleFX->_drawTech->GetPassByIndex(p)->Apply(0, _d3dImmediateContext);

		_d3dImmediateContext->DrawAuto();
	}

}
