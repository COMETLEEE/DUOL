#include "Core/DirectX11/SamplerState.h"

#include <d3d11.h>

#include "Core/DirectX11/DXEngine.h"

namespace MuscleGrapics
{
	ID3D11SamplerState* SamplerState::_wrapSamplerState = nullptr;

	ID3D11SamplerState* SamplerState::_min_Mag_SamplerState = nullptr;

	SamplerState::SamplerState()
	{
		D3D11_SAMPLER_DESC samDesc;

		auto device = DXEngine::GetInstance()->GetD3dDevice();

		samDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		samDesc.MipLODBias = 0.f;
		samDesc.MaxAnisotropy = 2;
		samDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samDesc.BorderColor[0] = 0.f;
		samDesc.BorderColor[1] = 0.f;
		samDesc.BorderColor[2] = 0.f;
		samDesc.BorderColor[3] = 0.f;

		samDesc.MinLOD = -FLT_MAX;
		samDesc.MaxLOD = FLT_MAX;

		device->CreateSamplerState(&samDesc, &_wrapSamplerState);

		samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		samDesc.MipLODBias = 0.f;
		samDesc.MaxAnisotropy = 2;
		samDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samDesc.BorderColor[0] = 0.f;
		samDesc.BorderColor[1] = 0.f;
		samDesc.BorderColor[2] = 0.f;
		samDesc.BorderColor[3] = 0.f;

		samDesc.MinLOD = -FLT_MAX;
		samDesc.MaxLOD = FLT_MAX;

		device->CreateSamplerState(&samDesc, &_min_Mag_SamplerState);
	}

	SamplerState::~SamplerState()
	{
		_wrapSamplerState->Release();
		_min_Mag_SamplerState->Release();
	}

	ID3D11SamplerState** SamplerState::GetWrapSamplerState()
	{
		return &_wrapSamplerState;
	}

	ID3D11SamplerState** SamplerState::GetMinMagSamplerState()
	{
		return &_min_Mag_SamplerState;
	}
}
