#include "pch.h"
#include "BasicPass.h"

#include "VertexDesc.h"

BasicPass::BasicPass() : PassBase<RenderingData_3D>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
_drawIndex(0)
{
	CompileVertexShader(TEXT("Shader/BaiscLight_VS.hlsl"), "main", VertexDesc::BasicLightVertex, VertexDesc::BasicLightVertexSize);
	CompilePixelShader(TEXT("Shader/BasicLight_PS.hlsl"), "main");

	CreateConstantBuffer(&_perObjectBuffer, sizeof(ConstantBuffDesc::CB_PerObject));

}

void BasicPass::SetConstants(RenderingData_3D& renderingData)
{

	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(renderingData._objectInfo->_meshID);
	auto& perfreamData = Renderer::GetPerfreamData();
	ID3D11DeviceContext* _d3dImmediateContext = DXEngine::GetInstance()->Getd3dImmediateContext();
	_drawIndex = vbibMesh->GetIndexSize();

	// 입력 배치 객체 셋팅
	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(Vertex::BasicLight); UINT offset = 0;


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	_d3dImmediateContext->Map(_perObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	ConstantBuffDesc::CB_PerObject* dataPtr = static_cast<ConstantBuffDesc::CB_PerObject*>(mappedResource.pData);

	dataPtr->gWorld = renderingData._geoInfo->_world;
	dataPtr->worldViewProj = renderingData._geoInfo->_worldViewProj;
	dataPtr->gWorldInvTranspose = renderingData._geoInfo->_worldInvTranspose;

	_d3dImmediateContext->Unmap(_perObjectBuffer, 0);
	constexpr unsigned slotNum = 0;
	_d3dImmediateContext->VSSetConstantBuffers(slotNum, 1, &_perObjectBuffer);

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
	//}

}
