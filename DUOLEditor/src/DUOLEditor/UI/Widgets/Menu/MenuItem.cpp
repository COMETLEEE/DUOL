#include "DUOLEditor/UI/Widgets/Menu/MenuItem.h"

namespace DUOLEditor
{
	MenuItem::MenuItem(const DUOLCommon::tstring& name, const DUOLCommon::tstring& shortcut, bool checkable,
		bool checked) :
		DataWidgetBase<bool>(&_isSelected)
		, _name(name)
		, _shortcut(shortcut)
		, _checkable(checkable)
		, _checked(checked)
	{
	}

	void MenuItem::Draw_Impl()
	{
		bool prevValue = _checked;

		if (ImGui::MenuItem(DUOLCommon::StringHelper::ToString(_name + _tstringID).c_str(), 
			DUOLCommon::StringHelper::ToString(_shortcut).c_str(), _checkable ? &_checked : nullptr, _isEnable))
		{
			_clickedEvent.Invoke();
		}

		if (_checked != prevValue)
		{
			_valueChangedEvent.Invoke(_checked);

			Notify();
		}
	}
}
