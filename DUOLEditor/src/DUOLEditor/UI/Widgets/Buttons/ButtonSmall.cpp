#include "DUOLEditor/UI/Widgets/Buttons/ButtonSmall.h"

namespace DUOLEditor
{
	ButtonSmall::ButtonSmall(const DUOLCommon::tstring& label) :
		_label(label)
	{
		auto& style = ImGui::GetStyle();

		_normalColor = ImGuiHelper::ToColor(style.Colors[ImGuiCol_Button]);
		_highlightedColor = ImGuiHelper::ToColor(style.Colors[ImGuiCol_ButtonHovered]);
		_pressedColor = ImGuiHelper::ToColor(style.Colors[ImGuiCol_ButtonActive]);
		_textColor = ImGuiHelper::ToColor(style.Colors[ImGuiCol_Text]);
	}

	void ButtonSmall::Draw_Impl()
	{
		auto& style = ImGui::GetStyle();

		ImVec4 prevNormalColor = style.Colors[ImGuiCol_Button];
		ImVec4 prevHighlightedColor = style.Colors[ImGuiCol_ButtonHovered];
		ImVec4 prevPressedColor = style.Colors[ImGuiCol_ButtonActive];
		ImVec4 prevTextColor = style.Colors[ImGuiCol_Text];

		style.Colors[ImGuiCol_Button] = ImGuiHelper::ToImVec4(_normalColor);
		style.Colors[ImGuiCol_ButtonHovered] = ImGuiHelper::ToImVec4(_highlightedColor);
		style.Colors[ImGuiCol_ButtonActive] = ImGuiHelper::ToImVec4(_pressedColor);
		style.Colors[ImGuiCol_Text] = ImGuiHelper::ToImVec4(_textColor);

		if (ImGui::SmallButton(DUOLCommon::StringHelper::ToString(_label + _tstringID).c_str()))
			_clickedEvent.Invoke();

		style.Colors[ImGuiCol_Button] = prevNormalColor;
		style.Colors[ImGuiCol_ButtonHovered] = prevHighlightedColor;
		style.Colors[ImGuiCol_ButtonActive] = prevPressedColor;
		style.Colors[ImGuiCol_Text] = prevTextColor;
	}
}