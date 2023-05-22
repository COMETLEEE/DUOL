#include "Core/DirectX11/BlendState.h"
#include <d3d11.h>
#include "Core/DirectX11/DXEngine.h"

namespace MuscleGrapics
{
	ID3D11BlendState* BlendState::_srcDestAlphaBlendState = nullptr;

	ID3D11BlendState* BlendState::_srcAdditiveBlendState = nullptr;

	ID3D11BlendState* BlendState::_uiBlendState = nullptr;

	ID3D11BlendState* BlendState::_srcDestAdditiveBlendState = nullptr;

	ID3D11BlendState* BlendState::_oitBlendState = nullptr;

	BlendState::BlendState()
	{
		D3D11_BLEND_DESC desc1;

		auto device = DXEngine::GetInstance()->GetD3dDevice();

		ZeroMemory(&desc1, sizeof(D3D11_BLEND_DESC));

		desc1.RenderTarget[0].BlendEnable = TRUE; // 블랜딩을 할지 안할지 여부.

		desc1.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // 원본 이미지의 색상
		desc1.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA; // 대상 이미지의 색상
		desc1.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // 블랜딩 방식
		// 그린 이미지의 Alpha 값을 지정하는 필드. 보통 변동이 없게 하기 위해 Source에 1, Dest에 Zero, OP에 Add를 넣어서 1로 만들어줌.
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
		desc3.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc3.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc3.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		device->CreateBlendState(&desc3, &_uiBlendState);


		D3D11_BLEND_DESC desc4;
		desc4.RenderTarget[0].BlendEnable = TRUE;
		ZeroMemory(&desc4, sizeof(D3D11_BLEND_DESC));

		desc4.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		desc4.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
		desc4.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc4.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc4.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc4.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc4.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		device->CreateBlendState(&desc3, &_srcDestAdditiveBlendState);

		ZeroMemory(&desc4, sizeof(D3D11_BLEND_DESC));
		desc4.IndependentBlendEnable = false;

		desc4.RenderTarget[0].BlendEnable = TRUE;
		desc4.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		desc4.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc4.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		desc4.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc4.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		desc4.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc4.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		device->CreateBlendState(&desc4, &_oitBlendState);
	}

	BlendState::~BlendState()
	{
		_srcDestAlphaBlendState->Release();

		_srcAdditiveBlendState->Release();

		_uiBlendState->Release();

		_srcDestAdditiveBlendState->Release();

		_oitBlendState->Release();
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

	ID3D11BlendState** BlendState::GetSrcDestAdditiveBlendState()
	{
		return &_srcDestAdditiveBlendState;
	}

	ID3D11BlendState** BlendState::GetOitBlendState()
	{
		return &_oitBlendState;
	}
}
