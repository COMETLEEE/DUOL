#pragma once
/**

	@class   SamplerState
	@brief   ���÷�������Ʈ
	@details ~

**/
namespace MuscleGrapics
{
	class SamplerState
	{
	public:
		SamplerState();

		~SamplerState();

	public:
		static ID3D11SamplerState* _wrapSamplerState;

	};
}
