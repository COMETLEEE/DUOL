#pragma once
#include "DUOLGraphicsLibrary/Renderer/Sampler.h"
#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"

namespace DUOLGraphicsLibrary
{
	class D3D11Sampler : public Sampler
	{

	private:
		ComPtr<ID3D11SamplerState> samplerState;

	public:
		ID3D11SamplerState* GetNativeSampler() const
		{
			return samplerState.Get();
		}
	};
}
