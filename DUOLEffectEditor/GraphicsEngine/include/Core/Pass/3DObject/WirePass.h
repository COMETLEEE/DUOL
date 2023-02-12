#pragma once
#include "Core/Pass/PassBase.h"

namespace MuscleGrapics
{
	class WirePass : public Pass_3D
	{
	public:
		WirePass();

		virtual ~WirePass() override = default;

	private:
		int _drawIndex;

	protected:
		virtual void SetConstants(RenderingData_3D& renderingData) override;

	public:
		virtual void Draw(RenderingData_3D& renderingData) override;

	};
}
