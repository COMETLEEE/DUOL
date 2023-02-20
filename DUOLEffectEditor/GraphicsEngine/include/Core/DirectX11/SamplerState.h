#pragma once
/**

	@class   SamplerState
	@brief   샘플러스테이트
	@details ~

**/
class ID3D11SamplerState;

namespace MuscleGrapics
{
	class SamplerState
	{
	public:
		SamplerState();

		~SamplerState();

	private:
		static ID3D11SamplerState* _wrapSamplerState;

		static ID3D11SamplerState* _min_Mag_LinearClamp_SamplerState;

		static ID3D11SamplerState* _LinearWrap_SamplerState;

	public:
		static ID3D11SamplerState** GetWrapSamplerState();

		static ID3D11SamplerState** GetLinearClampSamplerState();

		static ID3D11SamplerState** GetLinearWrapSamplerState();

	};
}
