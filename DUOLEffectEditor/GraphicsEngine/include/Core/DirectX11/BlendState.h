#pragma once
/**

	@class   BlendState
	@brief   파티클 혼합을 위한 블랜드스테이트
	@details ~

**/
class ID3D11BlendState;

namespace MuscleGrapics
{
	class BlendState
	{
	public:
		BlendState();

		~BlendState();
	private:
		static ID3D11BlendState* _srcDestAlphaBlendState;

		static ID3D11BlendState* _srcAdditiveBlendState;

		static ID3D11BlendState* _uiBlendState;

		static ID3D11BlendState* _srcDestAdditiveBlendState;

		static ID3D11BlendState* _oitBlendState;

	public:
		static ID3D11BlendState** GetDestAlphaBlendState();

		static ID3D11BlendState** GetAdditiveBlendState();

		static ID3D11BlendState** GetUiBlendState();

		static ID3D11BlendState** GetSrcDestAdditiveBlendState();

		static ID3D11BlendState** GetOitBlendState();
	};
}

