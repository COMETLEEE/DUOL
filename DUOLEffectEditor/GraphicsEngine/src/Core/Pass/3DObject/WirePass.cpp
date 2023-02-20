#include "Core/Pass/3DObject/WirePass.h"

#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/RenderTarget.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/Resource/VBIBMesh.h"
#include "Core/DirectX11/RenderTexture.h"
#include "Core/DirectX11/RasterizerState.h"

namespace MuscleGrapics
{
	WirePass::WirePass() : Pass_3D(D3D11_PRIMITIVE_TOPOLOGY_LINELIST)
	{
		const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

		PipeLineDesc pipeLineDesc;

		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/Wire_VS.hlsl"), "main");

		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/Wire_PS.hlsl"), "main");

		InsertShader(pipeLineDesc);

		CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerObject));
	}

	void WirePass::SetConstants(RenderingData_3D& renderingData)
	{
		auto& perfreamData = Renderer::GetPerfreamData();

		ConstantBuffDesc::CB_PerObject data;

		data.worldViewProj = renderingData._geoInfo._worldViewProj;

		UpdateConstantBuffer(1, data);

		// 인덱스버퍼와 버텍스버퍼 셋팅
		constexpr UINT stride = sizeof(Vertex::Basic);

		constexpr UINT offset = 0;

		auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetResource<VBIBMesh>(renderingData._objectInfo._meshName);

		_drawIndex = vbibMesh->GetIndexSize();
		// 입력 배치 객체 셋팅

		_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼

		_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼

		RasterizerState::SetRasterizerState(1);
	}

	void WirePass::Draw(RenderingData_3D& renderingData)
	{
		SetShader();

		SetConstants(renderingData);

		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();
		auto depth = DXEngine::GetInstance()->GetDepthStencil();
		depth->OnDepthStencil();
		renderTarget->SetRenderTargetView(
			depth->GetDepthStencilView(0),
			6,
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Albedo]->GetRTV(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Depth]->GetRTV(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Normal]->GetRTV(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Position]->GetRTV(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::MetallicRoughnessAOSpecular]->GetRTV(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::ObjectID]->GetRTV()
		);

		_d3dImmediateContext->DrawIndexed(_drawIndex, 0, 0);
	}
}