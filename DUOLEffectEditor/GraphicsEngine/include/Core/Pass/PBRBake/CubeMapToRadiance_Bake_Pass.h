#pragma once
/**

	@class   CubeMapToRadiance_Bake_Pass
	@brief   Radiance를 미리 구워 놓는 패스.
	@details ~

**/
#include "Core/Pass/PassBase.h"

namespace MuscleGrapics
{
	class CubeMapToRadiance_Bake_Pass : public Pass_Texture
	{
	public:
		CubeMapToRadiance_Bake_Pass();

		virtual ~CubeMapToRadiance_Bake_Pass() override = default;

	protected:
		virtual void SetConstants(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;

		unsigned int _index;

		float _roughness;

	public:
		virtual void Draw(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;

		void SetResource(unsigned int index, float roughness)
		{
			_index = index;

			_roughness = roughness;
		}
	};
}
