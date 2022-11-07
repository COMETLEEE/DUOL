#include "pch.h"
#include "WirePass.h"

WirePass::WirePass() :
	_d3dImmediateContext(DXEngine::GetInstance()->Getd3dImmediateContext()),
	_topolgy(D3D11_PRIMITIVE_TOPOLOGY_LINELIST),
	_inputLayout(InputLayout::Wire),
	_drawIndex(0)
{
}

void WirePass::SetConstants(std::shared_ptr<RenderingData_3D>& renderingData)
{

	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(renderingData->_objectInfo->_meshID);
	auto& perfreamData = Renderer::GetPerfreamData();
	ID3D11DeviceContext* _d3dImmediateContext = DXEngine::GetInstance()->Getd3dImmediateContext();
	_drawIndex = vbibMesh->GetIndexSize();
	// �Է� ��ġ ��ü ����
	_d3dImmediateContext->IASetInputLayout(_inputLayout); // �Է� ��ġ/ ���� ����ü�� �����ϰ� direct3d ���� �˷��ִ� �Լ�.
	_d3dImmediateContext->IASetPrimitiveTopology(_topolgy); //�⺻ ������ ���� ������ ������ ������ ��κ� �ﰢ���� ���.
	// �ε������ۿ� ���ؽ����� ����
	UINT stride = sizeof(Vertex::Basic);
	UINT offset = 0;
	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //���ؽ� ����
	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //�ε��� ����

	// �ʰ� �Ӻ� �� �ϰ��־��.
	Effects::WireFX->SetColor(XMFLOAT4(1, 1, 1, 1));

	/// WVP TM���� ����
	// Set constants
	
	XMMATRIX world = renderingData->_geoInfo->_world; // ��Ʈ ��Ʈ����
	XMMATRIX _View = perfreamData->_cameraInfo->_viewMatrix; // ī�޶�
	XMMATRIX _Proj = perfreamData->_cameraInfo->_projMatrix; // ī�޶�

	Effects::WireFX->WorldViewProjUpdate(world, _View, _Proj);
}

void WirePass::Draw(std::shared_ptr<RenderingData_3D>& renderingData)
{
	SetConstants(renderingData);
	D3DX11_TECHNIQUE_DESC techDesc;
	Effects::WireFX->m_WireTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil(p);
		DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(p);
		Effects::WireFX->m_WireTech->GetPassByIndex(p)->Apply(0, _d3dImmediateContext);
		_d3dImmediateContext->DrawIndexed(_drawIndex, 0, 0);
	}

}
