#include "pch.h"
#include "RasterizerState.h"

ID3D11RasterizerState* RasterizerState::m_WireframeRS = nullptr;
ID3D11RasterizerState* RasterizerState::m_SolidRS = nullptr;

RasterizerState::RasterizerState() // 렌더 스테이트 생성.
{
	//어떻게 출력할지 설정. 솔리드, 와이어..

	ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();
	HRESULT hr;

	// Render State 중 Rasterizer State
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	HR(_d3dDevice->CreateRasterizerState(&solidDesc, &m_SolidRS));


	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_NONE;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(_d3dDevice->CreateRasterizerState(&wireframeDesc, &m_WireframeRS));
}

RasterizerState::~RasterizerState()
{
	ReleaseCOM(m_WireframeRS);
	ReleaseCOM(m_SolidRS);
}
