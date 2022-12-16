#include "DUOLEditor/UI/Widgets/Texts/TextLabeled.h"

namespace DUOLEditor
{
	TextLabeled::TextLabeled(const DUOLCommon::tstring& text, const DUOLCommon::tstring& label) :
		Text(text)
		, _label(label)
	{
	}

	void TextLabeled::Draw_Impl()
	{
		ImGui::LabelText(DUOLCommon::StringHelper::ToString(_label + _tstringID).c_str(),
			DUOLCommon::StringHelper::ToString(_text).c_str());
	}
}