#include "D3D11Sampler.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXFlagMap.h"

namespace DUOLGraphicsLibrary
{
	D3D11Sampler::D3D11Sampler(const UINT64& guid, ID3D11Device* device, const SamplerDesc& samplerDesc):
		Sampler(guid)
	{
		D3D11_SAMPLER_DESC	d3dSamplerDesc;

		d3dSamplerDesc.AddressU = MapDXTextureAdressMode(samplerDesc._addressU);
		d3dSamplerDesc.AddressW = MapDXTextureAdressMode(samplerDesc._addressW);
		d3dSamplerDesc.AddressV = MapDXTextureAdressMode(samplerDesc._addressV);

		d3dSamplerDesc.ComparisonFunc = MapDXComparisonFunc(samplerDesc._comparisonFunc);
		d3dSamplerDesc.Filter = MapDXFilter(samplerDesc._samplerFilter);

		d3dSamplerDesc.MaxAnisotropy = samplerDesc._maxAnisotropy;
		d3dSamplerDesc.MaxLOD = samplerDesc._maxLOD;
		d3dSamplerDesc.MinLOD = samplerDesc._minLOD;
		d3dSamplerDesc.MipLODBias = samplerDesc._mipLODBias;

		d3dSamplerDesc.BorderColor[0] = samplerDesc._borderColor[0];
		d3dSamplerDesc.BorderColor[1] = samplerDesc._borderColor[1];
		d3dSamplerDesc.BorderColor[2] = samplerDesc._borderColor[2];
		d3dSamplerDesc.BorderColor[3] = samplerDesc._borderColor[3];

		HRESULT hr = device->CreateSamplerState(&d3dSamplerDesc, _samplerState.ReleaseAndGetAddressOf());
		DXThrowError(hr, "D3D11Sampler Failed");

#if defined(DEBUG) || defined(_DEBUG)  
	_samplerState->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(guid), &guid);
#endif
	}
}