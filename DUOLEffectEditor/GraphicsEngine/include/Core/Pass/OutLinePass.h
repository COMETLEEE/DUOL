#pragma once
/**
	@class   OutLinePass
	@brief   OutLine�� ���������� ȭ�鿡 ��Ÿ���� ���� �н�.
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

