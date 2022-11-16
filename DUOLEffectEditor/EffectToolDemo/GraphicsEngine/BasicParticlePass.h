#pragma once
class BasicParticlePass : public PassBase<RenderingData_Particle>
{
public:
	BasicParticlePass();

	virtual ~BasicParticlePass() = default;

protected:
	virtual void SetConstants(RenderingData_Particle& renderingData) override;

public:
	virtual void Draw(RenderingData_Particle& renderingData) override;
};
