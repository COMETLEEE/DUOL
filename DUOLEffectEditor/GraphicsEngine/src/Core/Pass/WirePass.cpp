#include "Core/Pass/WirePass.h"

#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/RenderTarget.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/VBIBMesh.h"
#include "Core/DirectX11/RenderTexture.h"
namespace MuscleGrapics
{
	WirePass::WirePass() : PassBase<RenderingData_3D>(D3D11_PRIMITIVE_TOPOLOGY_LINELIST)
	{
		CompileVertexShader(TEXT("Asset/Particle/Shader/Wire_VS.hlsl"), "main", VertexDesc::BasicVertex, VertexDesc::BasicVertexSize);

		CompilePixelShader(TEXT("Asset/Particle/Shader/Wire_PS.hlsl"), "main");

		CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerObject));
	}

	void WirePass::SetConstants(RenderingData_3D& renderingData)
	{
		auto& perfreamData = Renderer::GetPerfreamData();

		ConstantBuffDesc::CB_PerObject data;

		data.worldViewProj = renderingData._geoInfo->_worldViewProj;

		UpdateConstantBuffer(0, data);

		// 인덱스버퍼와 버텍스버퍼 셋팅
		constexpr UINT stride = sizeof(Vertex::Basic);

		constexpr UINT offset = 0;

		auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(renderingData._objectInfo->_meshID);

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
		depth->OnDepthStencil(0);
		renderTarget->SetRenderTargetView(
			depth->GetDpethStencilView(0),
			7,
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Depth]->GetRenderTargetView(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Normal]->GetRenderTargetView(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Position]->GetRenderTargetView(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::Albedo]->GetRenderTargetView(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::MatDiffuse]->GetRenderTargetView(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::MatSpecular]->GetRenderTargetView(),
			renderTarget->GetRenderTexture()[(int)MutilRenderTexture::ObjectID]->GetRenderTargetView()
		);

		_d3dImmediateContext->DrawIndexed(_drawIndex, 0, 0);
	}
}