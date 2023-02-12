#pragma once

#include "Core/Pass/PassBase.h"

namespace MuscleGrapics
{
	class SkyBoxPass : public Pass_3D
	{
	public:
		SkyBoxPass();

		virtual ~SkyBoxPass() override = default;

	private:
		int _drawIndex;

	protected:
		virtual void SetConstants(RenderingData_3D& renderingData) override;

	public:
		virtual void Draw(RenderingData_3D& renderingData) override;

	};

}
