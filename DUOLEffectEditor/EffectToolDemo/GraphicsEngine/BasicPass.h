#pragma once
class BasicPass : public PassBase<RenderingData_3D>
{
public:
	BasicPass();

	virtual ~BasicPass() = default;
private:
	ID3D11DeviceContext* _d3dImmediateContext;

	D3D11_PRIMITIVE_TOPOLOGY _topolgy;

	ID3D11InputLayout* _inputLayout;

	int _drawIndex;

protected:
	virtual void SetConstants(RenderingData_3D& renderingData) override;

public:
	virtual void Draw(RenderingData_3D& renderingData) override;
};

