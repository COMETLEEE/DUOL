#pragma once
/**
	@class   OutLinePass
	@brief   OutLine을 최종적으로 화면에 나타내기 위한 패스.
	@details ~

**/
#include "Core/Pass/PassBase.h"

namespace MuscleGrapics
{
	class OutLinePass : public Pass_Texture
	{
	public:
		OutLinePass();

		virtual ~OutLinePass() override = default;

	protected:
		virtual void SetConstants(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;

	public:
		virtual void Draw(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;
	};
}

