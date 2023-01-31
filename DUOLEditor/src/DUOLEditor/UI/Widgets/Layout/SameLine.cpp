#include "DUOLEditor/UI/Widgets/Layout/SameLine.h"

namespace DUOLEditor
{
	SameLine::SameLine(float offsetFromStartX, float spacing) :
		 _offsetFromStartX(offsetFromStartX)
		, _spacing(spacing)
	{
	}

	SameLine::~SameLine()
	{
		// 딱히 정리할 일은 없습니다.
	}

	void DUOLEditor::SameLine::Draw_Impl()
	{
		ImGui::SameLine(_offsetFromStartX, _spacing);
	}
}
