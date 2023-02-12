#pragma once
/**
	@class   ParticleOutLinePass
	@brief   Particle의 Out라인을 생성하는 패스이다.
	@details ~

**/


#include "Core/Pass/PassBase.h"

namespace MuscleGrapics
{
	class ParticleOutLinePass : public Pass_Particle
	{
	public:
		ParticleOutLinePass();

		virtual ~ParticleOutLinePass() = default;

	protected:
		virtual void SetConstants(RenderingData_Particle& renderingData) override;

	public:
		virtual void Draw(RenderingData_Particle& renderingData) override;

	};
}