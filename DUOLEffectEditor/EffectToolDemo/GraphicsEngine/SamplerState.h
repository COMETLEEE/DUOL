#pragma once
/**

    @class   SamplerState
    @brief   ���÷�������Ʈ
    @details ~

**/
class SamplerState
{
public:
	SamplerState();

	~SamplerState();

public:

	static ID3D11SamplerState* _wrapSamplerState;
};

