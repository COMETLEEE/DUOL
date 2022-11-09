#pragma once
class Shader_Wire;

class WirePass : public PassBase<RenderingData_3D>
{
public:
	WirePass();

	virtual ~WirePass() = default;

	int _drawIndex;
	Shader_Wire* test;

protected:
	virtual void SetConstants(std::shared_ptr<RenderingData_3D>& renderingData) override;

public:
	virtual void Draw(std::shared_ptr<RenderingData_3D>& renderingData) override;

};

