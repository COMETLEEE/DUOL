#include "DUOLEditor/UI/Widgets/Layout/Spacing.h"

namespace DUOLEditor
{
	Spacing::Spacing(int spaces) :
		_spaces(spaces)
	{
	}

	void Spacing::Draw_Impl()
	{
		if (_spaces <= 0)
			return;

		for (int i = 0 ; i < _spaces ; i++)
		{
			ImGui::Spacing();

			// 같은 라인에서 스페이스를 만들고 스페이싱 끝나면 넘어가는걸로.
			if (i + 1 < _spaces)
				ImGui::SameLine();
		}
	}
}