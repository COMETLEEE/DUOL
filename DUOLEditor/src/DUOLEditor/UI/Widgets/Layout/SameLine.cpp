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
		// ���� ������ ���� �����ϴ�.
	}

	void DUOLEditor::SameLine::Draw_Impl()
	{
		ImGui::SameLine(_offsetFromStartX, _spacing);
	}
}
