#pragma once
/**

	@class   SamplerState
	@brief   ���÷�������Ʈ
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

		static ID3D11SamplerState* _min_Mag_SamplerState;

	public:
		static ID3D11SamplerState** GetWrapSamplerState();

		static ID3D11SamplerState** GetMinMagSamplerState();

	};
}
