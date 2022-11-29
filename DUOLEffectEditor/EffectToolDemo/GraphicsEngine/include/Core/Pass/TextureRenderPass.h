#pragma once
/**
	@class   TextureRenderPass
	@brief   ���۵� ������ ���� �н��� ���ҽ� �Ŵ������� �������� �ʴ´�. ��ȸ���̱� ����.
	@details ~

**/
#include "Core/Pass/PassBase.h"

namespace MuscleGrapics
{
	class TextureRenderPass : public PassBase<std::pair<ID3D11ShaderResourceView*, int>>
	{
	public:
		TextureRenderPass();

		virtual ~TextureRenderPass() override = default;

	private:
		//float left, right, top, bottom;
		float _left = -1.0f;

		float _right = 1.0f;

		float _top = 1.0f;

		float _bottom = -1.0f;

	protected:
		virtual void SetConstants(std::pair<ID3D11ShaderResourceView*, int>& renderingData) override;

	public:
		virtual void Draw(std::pair<ID3D11ShaderResourceView*, int>& renderingData) override;

		void SetDrawRectangle(float left, float right, float top, float bottom);
	};
}
