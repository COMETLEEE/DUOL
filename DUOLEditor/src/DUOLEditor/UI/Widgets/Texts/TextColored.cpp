#include "DUOLEditor/UI/Widgets/Texts/TextColored.h"

namespace DUOLEditor
{
	TextColored::TextColored(const DUOLCommon::tstring& text, const Color color) :
		Text(_text)
		, _color(color)
	{
	}

	void TextColored::Draw_Impl()
	{
		ImGui::TextColored(ImGuiHelper::ToImVec4(_color), DUOLCommon::StringHelper::ToString(_text).c_str());
	}
}
