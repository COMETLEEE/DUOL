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

	// 입력 배치 객체 셋팅
	_d3dImmediateContext->IASetInputLayout(_inputLayout); // 입력 배치/ 정점 구조체를 정의하고 direct3d 에게 알려주는 함수.
	_d3dImmediateContext->IASetPrimitiveTopology(_topolgy); //기본 도형의 위상 구조를 정의한 열거형 대부분 삼각형을 사용.
	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(Vertex::Basic); UINT offset = 0;
	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼
	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼

	// 너가 임보 좀 하고있어라.
	Effects::WireFX->SetColor(XMFLOAT4(1, 1, 1, 1));

	/// WVP TM등을 셋팅
	// Set constants

	XMMATRIX world = renderingData->_geoInfo->_world; // 월트 메트릭스
	XMMATRIX _View = perfreamData->_cameraInfo->_viewMatrix; // 카메라
	XMMATRIX _Proj = perfreamData->_cameraInfo->_projMatrix; // 카메라

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
