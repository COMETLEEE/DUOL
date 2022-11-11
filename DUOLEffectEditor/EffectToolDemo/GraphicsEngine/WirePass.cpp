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


	// ��� ������ ������ �� �� �ֵ��� ��޴ϴ�.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	_d3dImmediateContext->Map(_perObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


	// ��� ������ �����Ϳ� ���� �����͸� �����ɴϴ�.
	ConstantBuffDesc::CB_PerObject* dataPtr = static_cast<ConstantBuffDesc::CB_PerObject*>(mappedResource.pData);

	// ��� ���ۿ� ����� �����մϴ�.
	dataPtr->worldViewProj = worldViewProj;


	// ��� ������ ����� Ǳ�ϴ�.
	_d3dImmediateContext->Unmap(_perObjectBuffer, 0);

	// ���� ���̴������� ��� ������ ��ġ�� �����մϴ�.
	constexpr unsigned bufferNumber = 0;

	// ���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲߴϴ�.
	_d3dImmediateContext->VSSetConstantBuffers(bufferNumber, 1, &_perObjectBuffer);

	// �ε������ۿ� ���ؽ����� ����
	constexpr UINT stride = sizeof(Vertex::Basic);
	constexpr UINT offset = 0;

	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(renderingData._objectInfo->_meshID);

	_drawIndex = vbibMesh->GetIndexSize();
	// �Է� ��ġ ��ü ����
	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //���ؽ� ����
	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //�ε��� ����
}

void WirePass::Draw(RenderingData_3D& renderingData)
{
	SetShader();
	SetConstants(renderingData);


	DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil(0);
	DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(0);
	_d3dImmediateContext->DrawIndexed(_drawIndex, 0, 0);
}
