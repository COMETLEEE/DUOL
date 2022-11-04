#include "pch.h"
#include "DepthStencil.h"

DepthStencil::DepthStencil() : m_OffDepthStencilState(nullptr)
{
	for (int i = 0; i < 2; i++)
	{
		m_DepthStencil[i] = new Depth();
	}
}

DepthStencil::~DepthStencil()
{
	Finalize();
}

void DepthStencil::OnDepthStencil(int _Num)
{
	m_DepthStencil[_Num]->OMsetDepth();
}

ID3D11DepthStencilView* DepthStencil::GetDpethStencilView(int _Num)
{
	return m_DepthStencil[_Num]->GetDeptStencilView();
}

void DepthStencil::OffDepthStencil()
{
	DXEngine::GetInstance()->Getd3dImmediateContext()->OMSetDepthStencilState(m_OffDepthStencilState, 1);
}

void DepthStencil::Clear()
{
	for (int i = 0; i < 2; i++)
	{ 
		m_DepthStencil[i]->Clear();
	}
}

void DepthStencil::OnResize()
{
	for (int i = 0; i < 2; i++)
	{
		m_DepthStencil[i]->OnResize();
	}


	ReleaseCOM(m_OffDepthStencilState);
	ID3D11Device* _Device = DXEngine::GetInstance()->GetD3dDevice();
	// Z-버퍼링을 끄는 뎁스 스텐실 스테이트를 생성해놓자.
	D3D11_DEPTH_STENCIL_DESC depthDisableStencilDesc;
	ZeroMemory(&depthDisableStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthDisableStencilDesc.DepthEnable = false; // todo 
	depthDisableStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisableStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthDisableStencilDesc.StencilEnable = true;
	depthDisableStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthDisableStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthDisableStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisableStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthDisableStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisableStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	_Device->CreateDepthStencilState(&depthDisableStencilDesc, &m_OffDepthStencilState);
}

void DepthStencil::Finalize()
{
	for (int i = 0; i < 2; i++)
	{
		delete m_DepthStencil[i];
	}
	ReleaseCOM(m_OffDepthStencilState)
}
