#pragma once
/**

	@class   BlendState
	@brief   ��ƼŬ ȥ���� ���� ���彺����Ʈ
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

