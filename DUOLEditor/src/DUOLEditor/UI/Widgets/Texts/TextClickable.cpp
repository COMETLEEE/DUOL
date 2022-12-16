#include "DUOLEditor/UI/Widgets/Texts/TextClickable.h"

namespace DUOLEditor
{
	TextClickable::TextClickable(const DUOLCommon::tstring& text) :
		Text(text)
	{
	}

	void TextClickable::Draw_Impl()
	{
		bool useless = false;

		if (ImGui::Selectable(DUOLCommon::StringHelper::ToString(_text + _tstringID).c_str(), &useless, ImGuiSelectableFlags_AllowDoubleClick))
		{
			if (ImGui::IsMouseDoubleClicked(0))
				_doubleClickedEvent.Invoke();
			else
				_clickedEvent.Invoke();
		}
	}
}
