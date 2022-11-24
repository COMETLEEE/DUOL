#pragma once
/**

	@class   DeferredRenderPass
	@brief   ���۵� ������ ���� �н��� ���ҽ� �Ŵ������� �������� �ʴ´�. ��ȸ���̱� ����.
	@details ~

**/
#include "PassBase.h"

namespace MuscleGrapics
{
	class DeferredRenderPass : public PassBase<std::vector<std::pair<ID3D11ShaderResourceView*, int>>>
	{
	public:
		DeferredRenderPass();

		virtual ~DeferredRenderPass() override = default;

	protected:
		virtual void SetConstants(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;

	public:
		virtual void Draw(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;
	};
}
