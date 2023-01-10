#pragma once
#include "Core/Pass/PassBase.h"
namespace MuscleGrapics
{
	class BasicParticleObjectIDPass : public PassBase<RenderingData_Particle>
	{
	public:
		BasicParticleObjectIDPass();

		virtual ~BasicParticleObjectIDPass() override = default;

	protected:
		virtual void SetConstants(RenderingData_Particle& renderingData) override;

	public:
		virtual void Draw(RenderingData_Particle& renderingData) override;
	};
}
