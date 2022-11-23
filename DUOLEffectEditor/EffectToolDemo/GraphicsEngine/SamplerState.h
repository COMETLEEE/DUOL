#pragma once
/**

	@class   SamplerState
	@brief   샘플러스테이트
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
