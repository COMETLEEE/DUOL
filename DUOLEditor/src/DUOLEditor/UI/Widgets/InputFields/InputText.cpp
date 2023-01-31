#include "DUOLEditor/UI/Widgets/InputFields/InputText.h"

namespace DUOLEditor
{
	InputText::InputText(const DUOLCommon::tstring& text, const DUOLCommon::tstring& label) :
		DataWidgetBase<DUOLCommon::tstring>(&_text)
		, _text(text)
		, _label(label)
		, _selectAllOnClick(false)
		, _isFullWidth(true)
	{
	}

	void InputText::Draw_Impl()
	{
		DUOLCommon::tstring prevText = _text;

		_text.resize(256, '\0');

		// 임구이는 'std::string', 'char*' 의 텍스트에 대해서만 호환됩니다.
		std::string temp = DUOLCommon::StringHelper::ToString(_text);

		if (_isFullWidth)
		{
			// 400 정도 까지의 Full-Width 만 지원합니다.
			ImGui::SetNextItemWidth(400.f);
		}

		bool enterPressed = ImGui::InputText(DUOLCommon::StringHelper::ToString(_label + _tstringID).c_str(), reinterpret_cast<char*>(&temp[0]),
			256, ImGuiInputTextFlags_EnterReturnsTrue | (_selectAllOnClick ? ImGuiInputTextFlags_AutoSelectAll : 0));

		_text = DUOLCommon::StringHelper::ToTString(temp);

		if (_text != prevText)
		{
			_textChangedEvent.Invoke(_text);

			Notify();
		}

		if (enterPressed)
			_enterPressedEvent.Invoke(_text);
	}

	bool InputText::GetIsFullWidth() const
	{
		return _isFullWidth;
	}

	void InputText::SetIsFullWidth(bool value)
	{
		_isFullWidth = value;
	}
}
