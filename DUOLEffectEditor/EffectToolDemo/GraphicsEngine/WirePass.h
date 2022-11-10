#pragma once
class Shader_Wire;

class WirePass : public PassBase<RenderingData_3D>
{
public:
	WirePass();

	virtual ~WirePass() = default;

private:
	int _drawIndex;

	ID3D11Buffer* _perObjectBuffer;

protected:
	virtual void SetConstants(RenderingData_3D& renderingData) override;

public:
	virtual void Draw(RenderingData_3D& renderingData) override;

};
