#include "DUOLEditor/UI/AddOns/ContextMenu.h"

namespace DUOLEditor
{
	void ContextMenu::Execute()
	{
		DestroyReservedWidgets();

		if (ImGui::BeginPopupContextItem())
		{
			DrawWidgets();

			ImGui::EndPopup();
		}

		AddReservedWidgets();
	}

	void ContextMenu::Close()
	{
		ImGui::CloseCurrentPopup();
	}
}
