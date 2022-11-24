#pragma once
/**

	@class   BlendState
	@brief   ��ƼŬ ȥ���� ���� ���彺����Ʈ
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

	public:
		static ID3D11BlendState** GetDestAlphaBlendState();

		static ID3D11BlendState** GetAdditiveBlendState();

		static ID3D11BlendState** GetUiBlendState();
	};
}

