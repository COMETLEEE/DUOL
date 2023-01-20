#include "DUOLEditor/UI/Widgets/Edits//ColorEdit.h"

namespace DUOLEditor
{
	ColorEdit::ColorEdit(bool enableAlpha, const Color& defaultColor) :
		DataWidgetBase<Color>(&_color)
		, _isEnableAlpha(enableAlpha)
		, _color(defaultColor)
	{
	}

	void ColorEdit::Draw_Impl()
	{
		int flags = !_isEnableAlpha ? ImGuiColorEditFlags_NoAlpha : 0;

		bool valueChanged = false;

		if (_isEnableAlpha)
			valueChanged = ImGui::ColorEdit4(DUOLCommon::StringHelper::ToString(_tstringID).c_str(), &_color.r, flags);
		else
			valueChanged = ImGui::ColorEdit3(DUOLCommon::StringHelper::ToString(_tstringID).c_str(), &_color.r, flags);

		if (valueChanged)
		{
			_colorChangedEvent.Invoke(_color);

			Notify();
		}
	}
}