#include "Core/Pass/Particle/ParticleOutLinePass.h"
#include "Core/DirectX11/SamplerState.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/Resource/ParticleMesh.h"
#include "Core/DirectX11/OrderIndependentTransparency.h"
#include "Core/DirectX11/RenderTarget.h"
#include "Core/Pass/ShaderFlagsManager.h"
#include "Core/DirectX11/RenderTexture.h"
#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/RasterizerState.h"

namespace MuscleGrapics
{
	ParticleOutLinePass::ParticleOutLinePass() : Pass_Particle(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
	{
		const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

		PipeLineDesc pipeLineDesc;

		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_VS.hlsl"), "ComputeShaderDrawVS");

		resoureManager->CompileGeometryShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_GS.hlsl"), "DrawGS", false);

		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicParticle_PS.hlsl"), "DrawOutLine");

		InsertShader(pipeLineDesc);

		CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerObject_Particle));

		CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerFream));
	}

	void ParticleOutLinePass::SetConstants(RenderingData_Particle& renderingData)
	{
		// 이미 다른 패스에서 ConstanceBuffer 업데이트를 해놨으니 그냥 진행한다.
	}

	void ParticleOutLinePass::Draw(RenderingData_Particle& renderingData)
	{
		unsigned int flag = renderingData.GetFlag();

		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::ParticleSystemCommonInfo))) return;
		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::Renderer))) return;
		if (!(flag & static_cast<unsigned int>(BasicParticle::Flags::Emission))) return;

		SetShader();

		_d3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

		DXEngine::GetInstance()->GetDepthStencil()->OffDepthStencil();

		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

		renderTarget->SetRenderTargetView(
			nullptr,
			1,
			renderTarget->GetRenderTexture()[static_cast<int>(MutilRenderTexture::OutLine)]->GetRTV()
		);

		_d3dImmediateContext->Draw(renderingData._commonInfo._maxParticles, 0);
	}
}
