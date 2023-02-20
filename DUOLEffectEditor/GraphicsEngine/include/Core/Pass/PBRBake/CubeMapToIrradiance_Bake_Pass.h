#pragma once
/**

	@class   CubeMapToIrradiance_Bake_Pass
	@brief   Irradiance를 미리 구워 놓는 패스.
	@details ~

**/
#include "Core/Pass/PassBase.h"

namespace MuscleGrapics
{
	class CubeMapToIrradiance_Bake_Pass : public Pass_Texture
	{
	public:
		CubeMapToIrradiance_Bake_Pass();

		virtual ~CubeMapToIrradiance_Bake_Pass() override = default;

	private:
		unsigned int _index;

	protected:
		virtual void SetConstants(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;

	public:
		void SetIndex(unsigned int idx) { _index = idx; }

		virtual void Draw(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;

	};
}
