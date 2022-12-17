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

			// ���� ���ο��� �����̽��� ����� �����̽� ������ �Ѿ�°ɷ�.
			if (i + 1 < _spaces)
				ImGui::SameLine();
		}
	}
}