// Forward 방식의 particleRender
#pragma once

#include "Core/Pass/PassBase.h"

namespace MuscleGrapics
{
	class ParticleMesh;

	class BasicParticlePass : public Pass_Particle
	{
	public:
		BasicParticlePass();

		virtual ~BasicParticlePass() override = default;
	private:
		ParticleMesh* _particleMesh; // 캐싱용.
	private:
		void ParticleUpdate(RenderingData_Particle& renderingData);

		void DrawParticle(RenderingData_Particle& renderingData);

		void DrawTrail(RenderingData_Particle& renderingData);

		void DrawDepth(RenderingData_Particle& renderingData);

	protected:
		virtual void SetConstants(RenderingData_Particle& renderingData) override;

	public:
		virtual void Draw(RenderingData_Particle& renderingData) override;
	};
}
