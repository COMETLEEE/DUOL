#include "DUOLEditor/UI/Widgets/Selections/CheckBox.h"

namespace DUOLEditor
{
	CheckBox::CheckBox(bool value, const DUOLCommon::tstring& label) :
		DataWidgetBase<bool>(&_value)
		, _value(value)
		, _label(label)
	{
	}

	void CheckBox::Draw_Impl()
	{
		bool previous = _value;

		ImGui::Checkbox(DUOLCommon::StringHelper::ToString(_label + _tstringID).c_str(), &_value);

		// ���� �ٲ���ٸ� �̺�Ʈ �� Notify. 
		if (_value != previous)
		{
			_valueChangedEvent.Invoke(_value);

			Notify();
		}
	}
}