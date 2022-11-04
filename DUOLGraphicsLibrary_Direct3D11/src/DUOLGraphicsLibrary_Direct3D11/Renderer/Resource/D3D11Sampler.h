#pragma once
#include "DUOLGraphicsLibrary/Renderer/Sampler.h"
#include "DUOLGraphicsLibrary/SamplerFlags.h"
#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"

namespace DUOLGraphicsLibrary
{
	class D3D11Sampler : public Sampler
	{
	public:
		D3D11Sampler(const UINT64& guid, ID3D11Device* device,const SamplerDesc& samplerDesc);

	private:
		ComPtr<ID3D11SamplerState> _samplerState;

	public:
		ID3D11SamplerState* GetNativeSampler() const
		{
			return _samplerState.Get();
		}
	};
}
