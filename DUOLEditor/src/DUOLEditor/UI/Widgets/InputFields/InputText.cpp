#include "DUOLEditor/UI/Widgets/InputFields/InputText.h"

namespace DUOLEditor
{
	InputText::InputText(const DUOLCommon::tstring& text, const DUOLCommon::tstring& label) :
		DataWidgetBase<DUOLCommon::tstring>(&_text)
		, _text(text)
		, _label(label)
		, _selectAllOnClick(false)
	{
	}

	void InputText::Draw_Impl()
	{
		DUOLCommon::tstring prevText = _text;

		_text.resize(256, '\0');

		bool enterPressed = ImGui::InputText(DUOLCommon::StringHelper::ToString(_label + _tstringID).c_str(), reinterpret_cast<char*>(&_text[0]),
			256, ImGuiInputTextFlags_EnterReturnsTrue | (_selectAllOnClick ? ImGuiInputTextFlags_AutoSelectAll : 0));
		
		if (_text != prevText)
		{
			_textChangedEvent.Invoke(_text);

			Notify();
		}

		if (enterPressed)
			_enterPressedEvent.Invoke(_text);
	}
}
