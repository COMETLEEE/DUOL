#include "Core/Pass/BasicPass.h"

#include "util/VertexDesc.h"

#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/Renderer.h"
#include "Core/DirectX11/RenderTarget.h"

#include "Core/DirectX11/DepthStencil.h"
#include "Core/Resource/ParticleMesh.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/VBIBMesh.h"

namespace MuscleGrapics
{
	BasicPass::BasicPass() : PassBase<RenderingData_3D>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
		_drawIndex(0)
	{
		CompileVertexShader(TEXT("Shader/BaiscLight_VS.hlsl"), "main", VertexDesc::BasicLightVertex, VertexDesc::BasicLightVertexSize);

		CompilePixelShader(TEXT("Shader/BasicLight_PS.hlsl"), "main");

		CreateConstantBuffer(0, sizeof(ConstantBuffDesc::CB_PerObject));
	}

	void BasicPass::SetConstants(RenderingData_3D& renderingData)
	{
		auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(renderingData._objectInfo->_meshID);

		auto& perfreamData = Renderer::GetPerfreamData();

		ID3D11DeviceContext* _d3dImmediateContext = DXEngine::GetInstance()->Getd3dImmediateContext();

		_drawIndex = vbibMesh->GetIndexSize();

		UINT stride = sizeof(Vertex::BasicLight); UINT offset = 0;

		ConstantBuffDesc::CB_PerObject data;// = static_cast<ConstantBuffDesc::CB_PerObject*>(mappedResource.pData);

		data.gWorld = renderingData._geoInfo->_world;

		data.worldViewProj = renderingData._geoInfo->_worldViewProj;

		data.gWorldInvTranspose = renderingData._geoInfo->_worldInvTranspose;

		UpdateConstantBuffer(0, data);

		_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼

		_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼
	}

	void BasicPass::Draw(RenderingData_3D& renderingData)
	{
		SetShader();

		SetConstants(renderingData);

		DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil(0);

		DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(0);

		_d3dImmediateContext->DrawIndexed(_drawIndex, 0, 0);
	}
}
