#include "pch.h"
#include "Depth.h"

Depth::Depth() :m_DepthStencilBuffer(), m_DepthStencilState(), m_DepthStencilView()
{

}

Depth::~Depth()
{
	Finalize();
}

void Depth::Clear()
{
	ID3D11DeviceContext* _DC = DXEngine::GetInstance()->Getd3dImmediateContext();
	_DC->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Depth::OnResize()
{
	Finalize();
	HRESULT hr;
	ID3D11Device* _Device = DXEngine::GetInstance()->GetD3dDevice();
	ID3D11DeviceContext* _DeviceContext = DXEngine::GetInstance()->Getd3dImmediateContext();

	D3D11_TEXTURE2D_DESC m_DepthStencilDesc; // 뎁스 스탠실 뷰

	UINT _Width = DXEngine::GetInstance()->GetWidth();
	UINT _Height = DXEngine::GetInstance()->GetHeight();

	ATLTRACE("OnResize 깊이 스텐실 버퍼 생성\n");
	// 깊이 스텐실 버퍼와 뷰 생성
	m_DepthStencilDesc.Width = _Width;
	m_DepthStencilDesc.Height = _Height;
	m_DepthStencilDesc.MipLevels = 1;
	m_DepthStencilDesc.ArraySize = 1;
	m_DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 4XMSAA를 사용하는가? 반드시교환사슬의MSAA설정과일치해야함.
	if (DXEngine::GetInstance()->GetEnable4xMsaa())
	{
		m_DepthStencilDesc.SampleDesc.Count = 4;
		m_DepthStencilDesc.SampleDesc.Quality = DXEngine::GetInstance()->Get4xMsaaQuality() - 1;
	}
	//MSAA를 사용하지 않음.
	else
	{
		m_DepthStencilDesc.SampleDesc.Count = 1;
		m_DepthStencilDesc.SampleDesc.Quality = 0;
	}

	m_DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	m_DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_DepthStencilDesc.CPUAccessFlags = 0;
	m_DepthStencilDesc.MiscFlags = 0;


	ATLTRACE("OnResize 깊이 스텐실 버퍼 생성\n");
	HR(_Device->CreateTexture2D(
		&m_DepthStencilDesc, //생성할 텍스처를 서술하는 구조체 .
		0,
		&m_DepthStencilBuffer)); // 깊이 스텐실 버퍼를 가리키는포인터를 돌려준다.



	ATLTRACE("OnResize 구조체 생성중\n");
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	// Stencil operations if pixel is front-facing. 
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing. 
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;




	ATLTRACE("OnResize 깊이 스텐실 설정\n");
	_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState);

	HR(_Device->CreateDepthStencilView(
		m_DepthStencilBuffer, // 뷰를 생성하고자 하는 자원.
		0,
		&m_DepthStencilView)); // 깊이.스텐실뷰를돌려준다.
}

void Depth::Finalize()
{
	ReleaseCOM(m_DepthStencilView)
		ReleaseCOM(m_DepthStencilBuffer)
		ReleaseCOM(m_DepthStencilState)
}

void Depth::OMsetDepth()
{
	DXEngine::GetInstance()->Getd3dImmediateContext()->OMSetDepthStencilState(m_DepthStencilState, 1);
}

ID3D11DepthStencilView* Depth::GetDeptStencilView()
{
	return m_DepthStencilView;
}
