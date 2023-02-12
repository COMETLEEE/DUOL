#include "Core/Pass/3DObject/NoLit_OrthoGraphicsPass.h"

#include "util/VertexDesc.h"

#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/Renderer.h"
#include "Core/DirectX11/RenderTarget.h"

#include "Core/DirectX11/DepthStencil.h"
#include "Core/Resource/Resource/ParticleMesh.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/Resource/VBIBMesh.h"
#include "Core/DirectX11/RenderTexture.h"
#include "Core/DirectX11/RasterizerState.h"

namespace MuscleGrapics
{
	NoLit_OrthoGraphicsPass::NoLit_OrthoGraphicsPass() : Pass_3D(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
		_drawIndex(0)
	{
		const auto resoureManager = DXEngine::GetInstance()->GetResourceManager();

		PipeLineDesc pipeLineDesc;

		resoureManager->CompileVertexShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BaiscLight_VS.hlsl"), "main");

		std::vector<D3D_SHADER_MACRO> ps_Macros;
		ps_Macros.push_back(D3D_SHADER_MACRO("NOLIGHT", "0"));
		ps_Macros.push_back(D3D_SHADER_MACRO(NULL, NULL));

		resoureManager->CompilePixelShader(pipeLineDesc, TEXT("Asset/Particle/Shader/BasicLight_PS.hlsl"), "main",  ps_Macros);

		InsertShader(pipeLineDesc);

		CreateConstantBuffer(1, sizeof(ConstantBuffDesc::CB_PerObject));
	}

	void NoLit_OrthoGraphicsPass::SetConstants(RenderingData_3D& renderingData)
	{
		auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetResource<VBIBMesh>(renderingData._objectInfo._meshName);

		auto& perfreamData = Renderer::GetPerfreamData();

		ID3D11DeviceContext* _d3dImmediateContext = DXEngine::GetInstance()->Getd3dImmediateContext();

		_drawIndex = vbibMesh->GetIndexSize();

		UINT stride = sizeof(Vertex::BasicLight); UINT offset = 0;

		ConstantBuffDesc::CB_PerObject data;// = static_cast<ConstantBuffDesc::CB_PerObject*>(mappedResource.pData);

		ZeroMemory(&data, sizeof(ConstantBuffDesc::CB_PerObject));

		data.gWorld = renderingData._geoInfo._world;

		auto world = renderingData._geoInfo._world;
		auto view = perfreamData->_cameraInfo._viewMatrix;
		auto proj = perfreamData->_cameraInfo._orthographicProj;

		data.worldViewProj = world * view * proj;

		data.gWorldInvTranspose = renderingData._geoInfo._worldInvTranspose;

		memcpy(&data.gObjectID, &renderingData._objectInfo._objectID, sizeof(UINT));

		memcpy(&data.gColor, &renderingData._materialInfo._color, sizeof(DUOLMath::Vector4));

		UpdateConstantBuffer(1, data);

		_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼

		_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼

		RasterizerState::SetRasterizerState(static_cast<int>(renderingData._shaderInfo._rasterizerState));
	}

	void NoLit_OrthoGraphicsPass::Draw(RenderingData_3D& renderingData)
	{
		SetShader();

		SetConstants(renderingData);

		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

		auto depth = DXEngine::GetInstance()->GetDepthStencil();

		depth->OnDepthStencil();

		renderTarget->SetRenderTargetView(
			depth->GetDepthStencilView(0),
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