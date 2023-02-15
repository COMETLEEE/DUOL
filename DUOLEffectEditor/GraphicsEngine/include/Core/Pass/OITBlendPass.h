#pragma once
/**
	@class   OITBlendPass
	@brief   OIT, Pixel List�� �����ϰ� ����ϴ� �н�.
	@details ~

**/
#include "Core/Pass/PassBase.h"

namespace MuscleGrapics
{
	class OITBlendPass : public Pass_Texture
	{
	public:
		OITBlendPass();

		virtual ~OITBlendPass() override = default;

	protected:
		virtual void SetConstants(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;

	public:
		virtual void Draw(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;
	};

}
