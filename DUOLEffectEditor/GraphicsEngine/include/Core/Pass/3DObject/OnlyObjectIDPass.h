#pragma once
/**
	@class   OnlyObjectIDPass
	@brief   ObjectID�� ���� �׷��� �� �� ����ϴ� �н��̴�.
	@details ~ �����ϸ� �ѹ� ������ �� �� ������Ʈ ID���� ���� �׸��� ����� ���� ������ ������.

**/


#include "Core/Pass/PassBase.h"

namespace MuscleGrapics
{
	class OnlyObjectIDPass : public Pass_3D
	{
	public:
		OnlyObjectIDPass();

		virtual ~OnlyObjectIDPass() = default;

	private:
		int _drawIndex;

	protected:
		virtual void SetConstants(RenderingData_3D& renderingData) override;

	public:
		virtual void Draw(RenderingData_3D& renderingData) override;

	};
}

