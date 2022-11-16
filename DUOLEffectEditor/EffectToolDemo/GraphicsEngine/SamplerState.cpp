#include "pch.h"
#include "SamplerState.h"

ID3D11SamplerState* SamplerState::_wrapSamplerState = nullptr;

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
}

SamplerState::~SamplerState()
{
	_wrapSamplerState->Release();
}
