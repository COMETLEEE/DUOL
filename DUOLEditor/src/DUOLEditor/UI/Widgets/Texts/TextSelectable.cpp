#include "DUOLEditor/UI/Widgets/Texts/TextSelectable.h"

namespace DUOLEditor
{
	TextSelectable::TextSelectable(const DUOLCommon::tstring& text, bool isSelected, bool isDisabled) :
		Text(text)
		, _isSelected(isSelected)
		, _isDisabled(isDisabled)
	{
	}

	void TextSelectable::Draw_Impl()
	{
		if (ImGui::Selectable(DUOLCommon::StringHelper::ToString(_text + _tstringID).c_str(), &_isSelected, 
			_isDisabled ? ImGuiSelectableFlags_Disabled : ImGuiSelectableFlags_None))
		{
			_clickedEvent.Invoke(_isSelected);

			if (_isSelected)
				_selectedEvent.Invoke();
			else
				_unSelectedEvent.Invoke();
		}
	}
}