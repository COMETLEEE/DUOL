#include "DUOLEditor/UI/Widgets/InputFields/InputTextStr.h"

#include <codecvt>
#include <locale>
#include <string>
#include <Windows.h>

namespace DUOLEditor
{
	InputTextStr::InputTextStr(const DUOLCommon::tstring& text, const DUOLCommon::tstring& label) :
		DataWidgetBase<std::string>(&_imguiStr)
		, _text(text)
		, _label(label)
		, _selectAllOnClick(false)
		, _isFullWidth(true)
	{
		char str[512];

		int end = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, _text.c_str(), lstrlenW(_text.c_str()), NULL, NULL, nullptr, nullptr);
		_imguiStr.reserve(std::max<int>(end, 0));

		WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, _text.c_str(), lstrlenW(_text.c_str()), str, end, nullptr, nullptr);
		str[end] = '\0';

		_imguiStr = str;
	}

	void InputTextStr::Draw_Impl()
	{

		DUOLCommon::tstring prevText = _text;

		/*	wchar_t* wstr = new wchar_t[strLen];

			MultiByteToWideChar(CP_UTF8,0,DUOLCommon::StringHelper::ToString(prevText).c_str(), -1, wstr, strLen);

			prevText = wstr;*/

		{
			// 400 정도 까지의 Full-Width 만 지원합니다.
			ImGui::SetNextItemWidth(400.f);

		}

		bool enterPressed = ImGui::InputText(DUOLCommon::StringHelper::ToString(_label + _tstringID).c_str(), reinterpret_cast<char*>(&_imguiStr[0]),
			256, ImGuiInputTextFlags_EnterReturnsTrue | (_selectAllOnClick ? ImGuiInputTextFlags_AutoSelectAll : 0));

		wchar_t buff[256];
		_imguiStr.resize(255);

		int end = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, _imguiStr.c_str(), _imguiStr.length(),
			buff, 256);
		buff[end] = L'\0';

		_text = buff;
		//_text.assign(_imguiStr.begin(), _imguiStr.end());
		//_text = DUOLCommon::StringHelper::ToTString(_imguiStr);

		if (_text != prevText)
		{
			_textChangedEvent.Invoke(_text);

			Notify();
		}

		if (enterPressed)
			_enterPressedEvent.Invoke(_text);
	}

	bool InputTextStr::GetIsFullWidth() const
	{
		return _isFullWidth;
	}

	void InputTextStr::SetIsFullWidth(bool value)
	{
		_isFullWidth = value;
	}
}
