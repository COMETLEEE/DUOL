#include "Core/DirectX11/Depth.h"
#include "Core/DirectX11/DXEngine.h"
#include "util/TypeDefine.h"

namespace MuscleGrapics
{
	Depth::Depth() :
		_depthStencilBuffer(),
		_depthStencilState(),
		_depthStencilView()
	{}

	Depth::~Depth()
	{
		Finalize();
	}

	void Depth::Clear()
	{
		ID3D11DeviceContext* _DC = DXEngine::GetInstance()->Getd3dImmediateContext();

		_DC->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void Depth::OnResize()
	{
		Finalize();

		ID3D11Device* _Device = DXEngine::GetInstance()->GetD3dDevice();

		ID3D11DeviceContext* _DeviceContext = DXEngine::GetInstance()->Getd3dImmediateContext();

		D3D11_TEXTURE2D_DESC m_DepthStencilDesc; // ���� ���Ľ� ��

		UINT _Width = DXEngine::GetInstance()->GetWidth();

		UINT _Height = DXEngine::GetInstance()->GetHeight();

		ATLTRACE("OnResize ���� ���ٽ� ���� ����\n"); // ���� ���ٽ� ���ۿ� �� ����

		m_DepthStencilDesc.Width = _Width;
		m_DepthStencilDesc.Height = _Height;
		m_DepthStencilDesc.MipLevels = 1;
		m_DepthStencilDesc.ArraySize = 1;
		m_DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;


		if (DXEngine::GetInstance()->GetEnable4xMsaa()) // 4XMSAA�� ����ϴ°�? �ݵ�ñ�ȯ�罽��MSAA��������ġ�ؾ���.
		{
			m_DepthStencilDesc.SampleDesc.Count = 4;
			m_DepthStencilDesc.SampleDesc.Quality = DXEngine::GetInstance()->Get4xMsaaQuality() - 1;
		}
		else //MSAA�� ������� ����.
		{
			m_DepthStencilDesc.SampleDesc.Count = 1;
			m_DepthStencilDesc.SampleDesc.Quality = 0;
		}

		m_DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		m_DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		m_DepthStencilDesc.CPUAccessFlags = 0;
		m_DepthStencilDesc.MiscFlags = 0;


		ATLTRACE("OnResize ���� ���ٽ� ���� ����\n");

		HR(_Device->CreateTexture2D(
			&m_DepthStencilDesc, //������ �ؽ�ó�� �����ϴ� ����ü .
			0,
			&_depthStencilBuffer)); // ���� ���ٽ� ���۸� ����Ű�������͸� �����ش�.

		ATLTRACE("OnResize ����ü ������\n");

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		ATLTRACE("OnResize ���� ���ٽ� ����\n");
		_Device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);

		HR(_Device->CreateDepthStencilView(
			_depthStencilBuffer, // �並 �����ϰ��� �ϴ� �ڿ�.
			0,
			&_depthStencilView)); // ����.���ٽǺ並�����ش�.
	}

	void Depth::Finalize()
	{
		ReleaseCOM(_depthStencilView);

		ReleaseCOM(_depthStencilBuffer);

		ReleaseCOM(_depthStencilState);
	}

	void Depth::OMsetDepth()
	{
		DXEngine::GetInstance()->Getd3dImmediateContext()->OMSetDepthStencilState(_depthStencilState, 1);
	}

	ID3D11DepthStencilView* Depth::GetDeptStencilView()
	{
		return _depthStencilView;
	}
}
