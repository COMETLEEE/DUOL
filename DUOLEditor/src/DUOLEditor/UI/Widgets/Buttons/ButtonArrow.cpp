#include "DUOLEditor/UI/Widgets/Buttons/ButtonArrow.h"

namespace DUOLEditor
{
	ButtonArrow::ButtonArrow(ImGuiDir direction) :
		_direction(direction)
	{
	}

	void ButtonArrow::Draw_Impl()
	{
		if (ImGui::ArrowButton(DUOLCommon::StringHelper::ToString(_tstringID).c_str(), _direction))
			_clickedEvent.Invoke();
	}
}