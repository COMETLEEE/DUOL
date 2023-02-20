#pragma once
/**

	@class   BRDFLookUpTable_Bake_Pass
	@brief   BRDF를 미리 구워 놓는 패스.
	@details ~

**/
#include "Core/Pass/PassBase.h"

namespace MuscleGrapics
{
	class BRDFLookUpTable_Bake_Pass : public Pass_Texture
	{
	public:
		BRDFLookUpTable_Bake_Pass();

		virtual ~BRDFLookUpTable_Bake_Pass() override = default;

	protected:
		virtual void SetConstants(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;

	public:
		virtual void Draw(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;

	};
}
