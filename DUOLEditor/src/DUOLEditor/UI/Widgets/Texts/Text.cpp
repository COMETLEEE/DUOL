#include "DUOLEditor/UI/Widgets/Texts/Text.h"

namespace DUOLEditor
{
	Text::Text(const DUOLCommon::tstring& text, std::function<void()> callbackAfter) :
		DataWidgetBase(&_text)	// �ּ� �����ְ�
		, _callbackAfter(callbackAfter)
		, _text(text)
	{

	}

	void Text::Draw_Impl()
	{
		ImGui::Text(DUOLCommon::StringHelper::ToString(_text).c_str());

		if (_callbackAfter != nullptr)
			_callbackAfter();
	}
}