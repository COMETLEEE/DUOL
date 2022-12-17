#pragma once
#include "DUOLEditor/UI/Widgets/WidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief 일정한 사이즈만큼 패널에 공간 만드는 위젯
	 */
	class Dummy : public WidgetBase
	{
	public:
		Dummy(const DUOLMath::Vector2& size = { 0.f, 0.f });

	protected:
		virtual void Draw_Impl() override;

	public:
		DUOLMath::Vector2 _size;
	};
}