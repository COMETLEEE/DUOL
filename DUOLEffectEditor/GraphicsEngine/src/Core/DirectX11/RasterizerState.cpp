#include "Core/DirectX11/RasterizerState.h"

#include <d3d11.h>

#include "Core/DirectX11/DXEngine.h"

namespace MuscleGrapics
{
	ID3D11RasterizerState* RasterizerState::_wireframeRS = nullptr;

	ID3D11RasterizerState* RasterizerState::_solidRS = nullptr;

	RasterizerState::RasterizerState() // 렌더 스테이트 생성.
	{
		ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();

		// Render State 중 Rasterizer State
		D3D11_RASTERIZER_DESC solidDesc;

		ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));

		solidDesc.FillMode = D3D11_FILL_SOLID;
		solidDesc.CullMode = D3D11_CULL_BACK;
		solidDesc.FrontCounterClockwise = false;
		solidDesc.DepthClipEnable = true;

		HR(_d3dDevice->CreateRasterizerState(&solidDesc, &_solidRS));


		D3D11_RASTERIZER_DESC wireframeDesc;

		ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));

		wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		wireframeDesc.CullMode = D3D11_CULL_NONE;
		wireframeDesc.FrontCounterClockwise = false;
		wireframeDesc.DepthClipEnable = true;

		HR(_d3dDevice->CreateRasterizerState(&wireframeDesc, &_wireframeRS));
	}

	RasterizerState::~RasterizerState()
	{
		ReleaseCOM(_wireframeRS);

		ReleaseCOM(_solidRS);
	}

	void RasterizerState::SetRasterizerState(int state)
	{
		auto dc = DXEngine::GetInstance()->Getd3dImmediateContext();
		switch (state)
		{
		case 0:
			dc->RSSetState(_solidRS);
			break;
		case 1:
			dc->RSSetState(_wireframeRS);
			break;
		default:
			break;
		}
	}
}
