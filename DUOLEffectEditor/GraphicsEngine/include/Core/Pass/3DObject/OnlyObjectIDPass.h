#pragma once
/**
	@class   OnlyObjectIDPass
	@brief   ObjectID만 따로 그려야 할 때 사용하는 패스이다.
	@details ~ 가능하면 한번 렌더링 할 때 오브젝트 ID까지 같이 그리고 사용할 일이 없도록 만들자.

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

