#include "DUOLEditor/UI/Widgets/Texts/Text.h"

namespace DUOLEditor
{
	Text::Text(const DUOLCommon::tstring& text) :
		DataWidgetBase(&_text)	// �ּ� �����ְ�
		, _text(text)
	{

	}

	void Text::Draw_Impl()
	{
		ImGui::Text(DUOLCommon::StringHelper::ToString(_text).c_str());
	}
}