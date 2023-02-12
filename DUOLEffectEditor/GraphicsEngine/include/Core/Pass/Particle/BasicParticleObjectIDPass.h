#pragma once
#include "Core/Pass/PassBase.h"
namespace MuscleGrapics
{
	class BasicParticleObjectIDPass : public Pass_Particle
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
