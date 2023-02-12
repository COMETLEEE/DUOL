#pragma once
#include "Core/Pass/PassBase.h"

namespace MuscleGrapics
{
	class NoLit_OrthoGraphicsPass : public Pass_3D
	{
	public:
		NoLit_OrthoGraphicsPass();

		virtual ~NoLit_OrthoGraphicsPass() override = default;

	private:
		int _drawIndex;

	protected:
		virtual void SetConstants(RenderingData_3D& renderingData) override;

	public:
		virtual void Draw(RenderingData_3D& renderingData) override;

	};
}

