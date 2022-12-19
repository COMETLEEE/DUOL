#include "DUOLEditor/UI/Panels/PanelUndecorated.h"

namespace DUOLEditor
{
	void PanelUndecorated::Draw_Impl()
	{
		auto& style = ImGui::GetStyle();
		ImVec2 previousPadding = style.WindowPadding;
		ImVec2 previousMinSize = style.WindowMinSize;
		style.WindowPadding = { 0, 0 };
		style.WindowMinSize = { 0, 0 };

		if (ImGui::Begin(DUOLCommon::StringHelper::ToString(_tstringID).c_str(), nullptr, GetFlags()))
		{
			style.WindowPadding = previousPadding;
			style.WindowMinSize = previousMinSize;

			Update();

			DrawWidgets();

			ImGui::End();
		}
		else
		{
			style.WindowPadding = previousPadding;
		}
	}

	int PanelUndecorated::GetFlags()
	{
		ImGuiWindowFlags flags =
			ImGuiWindowFlags_AlwaysUseWindowPadding |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoBackground;

		if (_autoSize) flags |= ImGuiWindowFlags_AlwaysAutoResize;

		return flags;
	}
}
