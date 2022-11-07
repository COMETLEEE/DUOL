#include "pch.h"
#include "BasicPass.h"

BasicPass::BasicPass():
	_d3dImmediateContext(DXEngine::GetInstance()->Getd3dImmediateContext()),
	_topolgy(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	_inputLayout(InputLayout::Light),
	_drawIndex(0)
{
}

void BasicPass::SetConstants(std::shared_ptr<RenderingData_3D>& renderingData)
{

	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(renderingData->_objectInfo->_meshID);
	auto& perfreamData = Renderer::GetPerfreamData();
	ID3D11DeviceContext* _d3dImmediateContext = DXEngine::GetInstance()->Getd3dImmediateContext();
	_drawIndex = vbibMesh->GetIndexSize();

	// �Է� ��ġ ��ü ����
	_d3dImmediateContext->IASetInputLayout(_inputLayout); // �Է� ��ġ/ ���� ����ü�� �����ϰ� direct3d ���� �˷��ִ� �Լ�.
	_d3dImmediateContext->IASetPrimitiveTopology(_topolgy); //�⺻ ������ ���� ������ ������ ������ ��κ� �ﰢ���� ���.
	// �ε������ۿ� ���ؽ����� ����
	UINT stride = sizeof(Vertex::Basic); UINT offset = 0;
	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //���ؽ� ����
	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //�ε��� ����

	// �ʰ� �Ӻ� �� �ϰ��־��.
	Effects::WireFX->SetColor(XMFLOAT4(1, 1, 1, 1));

	/// WVP TM���� ����
	// Set constants

	XMMATRIX world = renderingData->_geoInfo->_world; // ��Ʈ ��Ʈ����
	XMMATRIX _View = perfreamData->_cameraInfo->_viewMatrix; // ī�޶�
	XMMATRIX _Proj = perfreamData->_cameraInfo->_projMatrix; // ī�޶�

	Effects::LightFX->WorldViewProjUpdate(world, _View, _Proj);
}

void BasicPass::Draw(std::shared_ptr<RenderingData_3D>& renderingData)
{
	SetConstants(renderingData);
	D3DX11_TECHNIQUE_DESC techDesc;
	Effects::LightFX->m_LightTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil(p);
		DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(p);
		Effects::LightFX->m_LightTech->GetPassByIndex(p)->Apply(0, _d3dImmediateContext);
		_d3dImmediateContext->DrawIndexed(_drawIndex, 0, 0);
	}

}
