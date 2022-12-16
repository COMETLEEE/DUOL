#include "DUOLEditor/UI/Widgets/Buttons/Button.h"

#include "DUOLEditor/Util/ImGuiHelper.h"

namespace DUOLEditor
{
	Button::Button(const DUOLCommon::tstring& label, const DUOLMath::Vector2& size, bool isDisabled):
			_label(label), _size(size), _isDisabled(isDisabled)
	{
		auto& style = ImGui::GetStyle();

		_normalColor = ImGuiHelper::ToColor(style.Colors[ImGuiCol_Button]);
		_highlightedColor = ImGuiHelper::ToColor(style.Colors[ImGuiCol_ButtonHovered]);
		_pressedColor = ImGuiHelper::ToColor(style.Colors[ImGuiCol_ButtonActive]);
		_textColor = ImGuiHelper::ToColor(style.Colors[ImGuiCol_Text]);
	}

	void Button::Draw_Impl()
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

		if (_isDisabled)
			ImGui::BeginDisabled(_isDisabled);

		if (ImGui::Button(DUOLCommon::StringHelper::ToString(_label + _tstringID).c_str(), ImGuiHelper::ToImVec2(_size)))
			_clickedEvent.Invoke();

		if (_isDisabled)
			ImGui::EndDisabled();

		style.Colors[ImGuiCol_Button] = prevNormalColor;
		style.Colors[ImGuiCol_ButtonHovered] = prevHighlightedColor;
		style.Colors[ImGuiCol_ButtonActive] = prevPressedColor;
		style.Colors[ImGuiCol_Text] = prevTextColor;
	}
}