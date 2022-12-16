#include "DUOLEditor/UI/Widgets/Texts/Text.h"

namespace DUOLEditor
{
	Text::Text(const DUOLCommon::tstring& text) :
		DataWidgetBase(&_text)	// 林家 带廉林绊
		, _text(text)
	{

	}

	void Text::Draw_Impl()
	{
		ImGui::Text(DUOLCommon::StringHelper::ToString(_text).c_str());
	}
}