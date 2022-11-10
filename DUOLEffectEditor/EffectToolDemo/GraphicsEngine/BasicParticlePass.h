#pragma once
class BasicParticlePass : public PassBase<RenderingData_Particle>
{
public:
	BasicParticlePass();

	virtual ~BasicParticlePass() = default;
private:
	ID3D11DeviceContext* _d3dImmediateContext;

	D3D11_PRIMITIVE_TOPOLOGY _topolgy;

	ID3D11InputLayout* _inputLayout;

	bool _testfirstRun = false;

protected:
	virtual void SetConstants(RenderingData_Particle& renderingData) override;

public:
	virtual void Draw(RenderingData_Particle& renderingData) override;
};
