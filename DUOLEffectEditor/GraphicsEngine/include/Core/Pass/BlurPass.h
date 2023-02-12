#pragma once
/**

	@class   BlurPass
	@brief   �ؽ�ó�� ��ȿ���� �Կ� �������ϴ� �н�.
	@details ~�ٿ� ������ �� ���������� �Ͽ� �� ȿ���� ����. �ִ� �ٿ���� Ƚ���� 4ȸ���� ������!

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