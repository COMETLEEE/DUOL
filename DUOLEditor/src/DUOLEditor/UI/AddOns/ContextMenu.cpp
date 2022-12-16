#include "DUOLEditor/UI/AddOns/ContextMenu.h"

namespace DUOLEditor
{
	void ContextMenu::Execute()
	{
		if (ImGui::BeginPopupContextItem())
		{
			DrawWidgets();

			ImGui::EndPopup();
		}
	}

	void ContextMenu::Close()
	{
		ImGui::CloseCurrentPopup();
	}
}
