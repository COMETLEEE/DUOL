#pragma once
/**

	@class   DeferredRenderPass
	@brief   디퍼드 렌더링 관련 패스는 리소스 매니저에서 관리하지 않는다. 일회용이기 때문.
	@details ~

**/
#include "PassBase.h"

namespace MuscleGrapics
{
	class DeferredRenderPass : public Pass_Texture
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
