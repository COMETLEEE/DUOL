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

	public:
		static ID3D11SamplerState** GetWrapSamplerState();

	};
}
