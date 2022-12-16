#include "DUOLEditor/UI/Widgets/Buttons/ButtonColored.h"

namespace DUOLEditor
{
	ButtonColored::ButtonColored(const DUOLCommon::tstring& text, const Color& color, bool usingAlpha):
		_label(text)
		, _color(color)
		, _usingAlpha(usingAlpha)
	{
	}

	void ButtonColored::Draw_Impl()
	{
		ImVec4 imColor = ImGuiHelper::ToImVec4(_color);

		if (ImGui::ColorButton(DUOLCommon::StringHelper::ToString(_label + _tstringID).c_str(), imColor,
			!_usingAlpha ? ImGuiColorEditFlags_NoAlpha : 0, ImGuiHelper::ToImVec2(_size)))
		{
			_clickedEvent.Invoke();
		}

		_color = ImGuiHelper::ToColor(imColor);
	}
}