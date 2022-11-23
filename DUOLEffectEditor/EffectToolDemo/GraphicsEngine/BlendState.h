#pragma once
/**

	@class   BlendState
	@brief   파티클 혼합을 위한 블랜드스테이트
	@details ~

**/
namespace MuscleGrapics
{
	class BlendState
	{
	public:
		BlendState();

		~BlendState();
	public:
		static ID3D11BlendState* _srcDestAlphaBlendState;

		static ID3D11BlendState* _srcAdditiveBlendState;

		static ID3D11BlendState* _uiBlendState;
	};
}

