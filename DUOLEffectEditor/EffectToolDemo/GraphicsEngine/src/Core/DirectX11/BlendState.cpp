#include "Core/DirectX11/BlendState.h"
#include <d3d11.h>
#include "Core/DirectX11/DXEngine.h"

namespace MuscleGrapics
{
	ID3D11BlendState* BlendState::_srcDestAlphaBlendState = nullptr;

	ID3D11BlendState* BlendState::_srcAdditiveBlendState = nullptr;

	ID3D11BlendState* BlendState::_uiBlendState = nullptr;

	BlendState::BlendState()
	{
		D3D11_BLEND_DESC desc1;

		auto device = DXEngine::GetInstance()->GetD3dDevice();

		ZeroMemory(&desc1, sizeof(D3D11_BLEND_DESC));

		desc1.RenderTarget[0].BlendEnable = TRUE; // ������ ���� ������ ����.

		desc1.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // ���� �̹����� ����
		desc1.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA; // ��� �̹����� ����
		desc1.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // ���� ���
		// �׸� �̹����� Alpha ���� �����ϴ� �ʵ�. ���� ������ ���� �ϱ� ���� Source�� 1, Dest�� Zero, OP�� Add�� �־ 1�� �������.
		desc1.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc1.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc1.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc1.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		device->CreateBlendState(&desc1, &_srcDestAlphaBlendState);

		D3D11_BLEND_DESC desc2;

		ZeroMemory(&desc2, sizeof(D3D11_BLEND_DESC));

		desc2.RenderTarget[0].BlendEnable = TRUE;
		desc2.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc2.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		desc2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		device->CreateBlendState(&desc2, &_srcAdditiveBlendState);

		D3D11_BLEND_DESC desc3;

		ZeroMemory(&desc3, sizeof(D3D11_BLEND_DESC));

		desc3.RenderTarget[0].BlendEnable = TRUE;
		desc3.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc3.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc3.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc3.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc3.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc3.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc3.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		device->CreateBlendState(&desc3, &_uiBlendState);
	}

	BlendState::~BlendState()
	{
		_srcDestAlphaBlendState->Release();

		_srcAdditiveBlendState->Release();

		_uiBlendState->Release();
	}

	ID3D11BlendState** BlendState::GetDestAlphaBlendState()
	{
		return &_srcDestAlphaBlendState;
	}

	ID3D11BlendState** BlendState::GetAdditiveBlendState()
	{
		return &_srcAdditiveBlendState;
	}

	ID3D11BlendState** BlendState::GetUiBlendState()
	{
		return &_uiBlendState;
	}
}
