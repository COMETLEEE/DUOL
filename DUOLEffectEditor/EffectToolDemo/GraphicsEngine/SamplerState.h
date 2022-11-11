#pragma once

class SamplerState
{
public:
	SamplerState();

	~SamplerState();

public:

	static ID3D11SamplerState* _wrapSamplerState;
};

