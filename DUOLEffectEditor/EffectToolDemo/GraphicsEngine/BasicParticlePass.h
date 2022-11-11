#pragma once
class BasicParticlePass : public PassBase<RenderingData_Particle>
{
public:
	BasicParticlePass();

	virtual ~BasicParticlePass() = default;

private:
	ID3D11Buffer* _perObjectBuffer;
	ID3D11Buffer* _perFreamBuffer;

protected:
	virtual void SetConstants(RenderingData_Particle& renderingData) override;

public:
	virtual void Draw(RenderingData_Particle& renderingData) override;
};
