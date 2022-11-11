#include "pch.h"
#include "WirePass.h"

#include "VertexDesc.h"

WirePass::WirePass() : PassBase<RenderingData_3D>(D3D11_PRIMITIVE_TOPOLOGY_LINELIST)
{
	CompileVertexShader(TEXT("Shader/Wire_VS.hlsl"), "main", VertexDesc::BasicVertex, VertexDesc::BasicVertexSize);
	CompilePixelShader(TEXT("Shader/Wire_PS.hlsl"), "main");

	CreateConstantBuffer(&_perObjectBuffer, sizeof(ConstantBuffDesc::CB_PerObject));
}

void WirePass::SetConstants(RenderingData_3D& renderingData)
{
	auto& perfreamData = Renderer::GetPerfreamData();

	Matrix worldViewProj = renderingData._geoInfo->_world * perfreamData->_cameraInfo->_viewMatrix * perfreamData->_cameraInfo->_projMatrix;


	// 상수 버퍼의 내용을 쓸 수 있도록 잠급니다.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	_d3dImmediateContext->Map(_perObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
	ConstantBuffDesc::CB_PerObject* dataPtr = static_cast<ConstantBuffDesc::CB_PerObject*>(mappedResource.pData);

	// 상수 버퍼에 행렬을 복사합니다.
	dataPtr->worldViewProj = worldViewProj;


	// 상수 버퍼의 잠금을 풉니다.
	_d3dImmediateContext->Unmap(_perObjectBuffer, 0);

	// 정점 셰이더에서의 상수 버퍼의 위치를 설정합니다.
	constexpr unsigned bufferNumber = 0;

	// 마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
	_d3dImmediateContext->VSSetConstantBuffers(bufferNumber, 1, &_perObjectBuffer);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	constexpr UINT stride = sizeof(Vertex::Basic);
	constexpr UINT offset = 0;

	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(renderingData._objectInfo->_meshID);

	_drawIndex = vbibMesh->GetIndexSize();
	// 입력 배치 객체 셋팅
	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼
	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼
}

void WirePass::Draw(RenderingData_3D& renderingData)
{
	SetShader();
	SetConstants(renderingData);


	DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil(0);
	DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(0);
	_d3dImmediateContext->DrawIndexed(_drawIndex, 0, 0);
}
