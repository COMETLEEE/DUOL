#pragma once
/**

	@class   BlurPass
	@brief   텍스처에 블러효과를 먹여 렌더링하는 패스.
	@details ~다운 스케일 후 업스케일을 하여 블러 효과를 낸다. 최대 다운스케일 횟수를 4회까지 만들어보자!

**/
#include "PassBase.h"

namespace MuscleGrapics
{
	class BlurPass : public Pass_Texture
	{
	public:
		BlurPass();

		virtual ~BlurPass() override = default;
	protected:
		virtual void SetConstants(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;

	public:
		virtual void Draw(std::vector<std::pair<ID3D11ShaderResourceView*, int>>& renderingData) override;
	};
}