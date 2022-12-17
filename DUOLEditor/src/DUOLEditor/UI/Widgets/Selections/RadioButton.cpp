#include "DUOLEditor/UI/Widgets/Selections/RadioButton.h"

namespace DUOLEditor
{
	RadioButton::RadioButton(bool selected, const DUOLCommon::tstring& label) :
		DataWidgetBase<bool>(&_isSelected)
		, _label(label)
	{
	}

	void RadioButton::Draw_Impl()
	{
		if (ImGui::RadioButton(DUOLCommon::StringHelper::ToString(_label + _tstringID).c_str(), _isSelected))
		{
			_clickedEvent.Invoke(_radioID);

			Notify();
		}
	}
}