#pragma once
#include "DUOLEditor/UI/Widgets/WidgetBase.h"

namespace DUOLEditor
{
	class SameLine : public DUOLEditor::WidgetBase
	{
	public:
		SameLine(float offsetFromStartX = 0.f, float spacing = -1.f);

		~SameLine();

	private:
		float _offsetFromStartX;

		float _spacing;

	public:
		virtual void Draw_Impl() override;
	};
}