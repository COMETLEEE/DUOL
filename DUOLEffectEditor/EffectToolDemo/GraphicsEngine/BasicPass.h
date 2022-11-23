#pragma once
namespace MuscleGrapics
{
	class BasicPass : public PassBase<RenderingData_3D>
	{
	public:
		BasicPass();

		virtual ~BasicPass() override = default;

	private:
		int _drawIndex;

	protected:
		virtual void SetConstants(RenderingData_3D& renderingData) override;

	public:
		virtual void Draw(RenderingData_3D& renderingData) override;

	};
}

