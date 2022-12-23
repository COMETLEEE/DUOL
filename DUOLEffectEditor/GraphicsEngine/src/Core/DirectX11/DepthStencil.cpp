#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/Depth.h"

namespace MuscleGrapics
{
	DepthStencil::DepthStencil() : _offDepthStencilState(nullptr)
	{
		for (int i = 0; i < 2; i++)
			_depthStencil[i] = new Depth();
	}
	DepthStencil::~DepthStencil()
	{
		Finalize();
	}
	void DepthStencil::OnDepthStencil()
	{
		DXEngine::GetInstance()->Getd3dImmediateContext()->OMSetDepthStencilState(_onDepthStencilState, 1);
	}
	ID3D11DepthStencilView* DepthStencil::GetDepthStencilView(int _Num)
	{
		return _depthStencil[_Num]->GetDepthStencilView();
	}

	Depth* DepthStencil::GetDepth(int num)
	{
		return _depthStencil[num];
	}

	void DepthStencil::OffDepthStencil()
	{
		DXEngine::GetInstance()->Getd3dImmediateContext()->OMSetDepthStencilState(_offDepthStencilState, 1);
	}
	void DepthStencil::Clear()
	{
		for (int i = 0; i < 2; i++)
			_depthStencil[i]->Clear();
	}
	void DepthStencil::OnResize()
	{
		for (int i = 0; i < 2; i++)
			_depthStencil[i]->OnResize();

		ReleaseCOM(_offDepthStencilState);

		ReleaseCOM(_onDepthStencilState);

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

		_Device->CreateDepthStencilState(&depthDisableStencilDesc, &_offDepthStencilState);

		depthDisableStencilDesc.DepthEnable = true;

		_Device->CreateDepthStencilState(&depthDisableStencilDesc, &_onDepthStencilState);

	}
	void DepthStencil::Finalize()
	{
		for (int i = 0; i < 2; i++)
		{
			delete _depthStencil[i];

			_depthStencil[i] = nullptr;
		}

		ReleaseCOM(_offDepthStencilState);
		ReleaseCOM(_onDepthStencilState);
	}
}
