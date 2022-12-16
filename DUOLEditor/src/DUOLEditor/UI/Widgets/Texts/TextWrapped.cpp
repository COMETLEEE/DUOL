#include "DUOLEditor/UI/Widgets/Texts/TextWrapped.h"

namespace DUOLEditor
{
	TextWrapped::TextWrapped(const DUOLCommon::tstring& text) :
		Text(text)
	{
	}

	void TextWrapped::Draw_Impl()
	{
		ImGui::TextWrapped(DUOLCommon::StringHelper::ToString(_text).c_str());
	}
}
