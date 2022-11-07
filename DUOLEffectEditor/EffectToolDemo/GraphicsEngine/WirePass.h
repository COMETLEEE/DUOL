#pragma once
class WirePass : public IPass<RenderingData_3D>
{
public:
	WirePass();

	virtual ~WirePass() = default;
private:
	ID3D11DeviceContext* _d3dImmediateContext;

	D3D11_PRIMITIVE_TOPOLOGY _topolgy;

	ID3D11InputLayout* _inputLayout;

	int _drawIndex;
protected:
	virtual void SetConstants(std::shared_ptr<RenderingData_3D>& renderingData) override;

public:
	virtual void Draw(std::shared_ptr<RenderingData_3D>& renderingData) override;

};

