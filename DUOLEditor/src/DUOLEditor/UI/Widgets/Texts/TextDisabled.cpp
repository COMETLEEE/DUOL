#include "DUOLEditor/UI/Widgets/Texts/TextDisabled.h"

namespace DUOLEditor
{
	TextDisabled::TextDisabled(const DUOLCommon::tstring& text) :
		Text(text)
	{
	}

	void TextDisabled::Draw_Impl()
	{
		ImGui::TextDisabled(DUOLCommon::StringHelper::ToString(_text).c_str());
	}
}
