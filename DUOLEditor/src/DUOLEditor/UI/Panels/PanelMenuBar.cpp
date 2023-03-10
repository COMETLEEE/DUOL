#include "DUOLEditor/UI/Panels/PanelMenuBar.h"

namespace DUOLEditor
{
	void PanelMenuBar::Draw_Impl()
	{
		if (!_widgets.empty() &&ImGui::BeginMainMenuBar())
		{
			DrawWidgets();

			ImGui::EndMainMenuBar();
		}
	}
}