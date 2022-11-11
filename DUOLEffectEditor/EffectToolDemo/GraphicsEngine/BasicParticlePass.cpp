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


	CreateConstantBuffer(&_perObjectBuffer, sizeof(ConstantBuffDesc::CB_PerObject_Particle));
	CreateConstantBuffer(&_perFreamBuffer, sizeof(ConstantBuffDesc::CB_PerFream_Particle));
}

void BasicParticlePass::SetConstants(RenderingData_Particle& renderingData)
{
	_d3dImmediateContext->VSSetSamplers(0, 1, &SamplerState::_wrapSamplerState);
	_d3dImmediateContext->GSSetSamplers(0, 1, &SamplerState::_wrapSamplerState);
	_d3dImmediateContext->PSSetSamplers(0, 1, &SamplerState::_wrapSamplerState);

	auto particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(renderingData.particleID);
	auto& perfreamData = Renderer::GetPerfreamData();
	//
	// Set constants.

	XMMATRIX world = renderingData._commonInfo->_transformMatrix; // 월트 메트릭스
	XMMATRIX view = perfreamData->_cameraInfo->_viewMatrix; // 카메라
	XMMATRIX proj = perfreamData->_cameraInfo->_projMatrix; // 카메라

	float static _GameTime = 0;
	_GameTime += perfreamData->_deltaTime;



	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		_d3dImmediateContext->Map(_perObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		ConstantBuffDesc::CB_PerObject_Particle* dataPtr = static_cast<ConstantBuffDesc::CB_PerObject_Particle*>(mappedResource.pData);

		dataPtr->gEmitPosW = Vector3(world.r[3].m128_f32[0], world.r[3].m128_f32[1], world.r[3].m128_f32[2]);
		dataPtr->gStartSpeed = renderingData._commonInfo->_startSpeed;
		dataPtr->gEmitDirW = Vector3(world.r[1].m128_f32[0], world.r[1].m128_f32[1], world.r[1].m128_f32[2]);
		dataPtr->gLifeTime = renderingData._commonInfo->_lifeTime;
		dataPtr->gStartSize = renderingData._commonInfo->_startSize;
		dataPtr->gEmissiveCount = renderingData._commonInfo->_emissiveCount;
		dataPtr->gEmissiveTime = renderingData._commonInfo->_emissiveTime;
		dataPtr->gAccelW = Vector3(0.0f, 7.8f, 0.0f);

		_d3dImmediateContext->Unmap(_perObjectBuffer, 0);
		constexpr unsigned slotNum = 0;
	}

	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		_d3dImmediateContext->Map(_perFreamBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		ConstantBuffDesc::CB_PerFream_Particle* dataPtr = static_cast<ConstantBuffDesc::CB_PerFream_Particle*>(mappedResource.pData);

		dataPtr->gCameraPosW = perfreamData->_cameraInfo->_cameraWorldPosition; // 카메라의 좌표
		dataPtr->gGameTime = _GameTime; // 게임 시간
		dataPtr->gScreenXY = Vector2(DXEngine::GetInstance()->GetWidth(), DXEngine::GetInstance()->GetHeight());
		dataPtr->gTimeStep = perfreamData->_deltaTime; // 1프레임당 시간
		dataPtr->gViewProj = view * proj;

		_d3dImmediateContext->Unmap(_perFreamBuffer, 0);
		constexpr unsigned slotNum = 1;
	}

	UINT stride = sizeof(Vertex::Particle);
	UINT offset = 0;




	//if (!renderingData._commonInfo->)
	//	_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetInitVB(), &stride, &offset);
	//else
	_d3dImmediateContext->IASetVertexBuffers(0, 1, particleMesh->GetDrawVB(), &stride, &offset);

	_d3dImmediateContext->VSSetConstantBuffers(0, 1, &_perObjectBuffer);
	_d3dImmediateContext->VSSetConstantBuffers(1, 1, &_perFreamBuffer);

	_d3dImmediateContext->GSSetConstantBuffers(0, 1, &_perObjectBuffer);
	_d3dImmediateContext->GSSetConstantBuffers(1, 1, &_perFreamBuffer);

	//_d3dImmediateContext->PSSetConstantBuffers(0, 1, &_perObjectBuffer);
	//_d3dImmediateContext->PSSetConstantBuffers(1, 1, &_perFreamBuffer);
}

void BasicParticlePass::Draw(RenderingData_Particle& renderingData)
{
	UINT stride = sizeof(Vertex::Particle);
	UINT offset = 0;

	auto particleMesh = DXEngine::GetInstance()->GetResourceManager()->GetParticleMesh(renderingData.particleID);

	SetShader(0); // streamOut
	SetConstants(renderingData);
	_d3dImmediateContext->SOSetTargets(1, particleMesh->GetStreamOutVB(), &offset);
	DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();
	//if (!_testfirstRun)
	//{
	//	_d3dImmediateContext->Draw(1, 0);
	//	_testfirstRun = true;
	//}
	//else
	_d3dImmediateContext->DrawAuto();

	SetShader(1); // Draw

	ID3D11Buffer* bufferArray[1] = { nullptr };
	std::swap(*particleMesh->GetDrawVB(), *particleMesh->GetStreamOutVB()); // 더블 버퍼링과 매우 흡사한 것.!!!
	_d3dImmediateContext->SOSetTargets(1, bufferArray, &offset);
	DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil(0);
	//SetConstants(renderingData);

	//_d3dImmediateContext->DrawAuto();

	_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}
