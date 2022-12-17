#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	class ColorPicker : public DataWidgetBase<Color>
	{
	public:
		ColorPicker(bool enableAlpha = false, const Color& defaultColor = {});

	protected:
		virtual void Draw_Impl() override;

	public:
		bool _isEnableAlpha;

		Color _color;

		DUOLCommon::Event<void, Color> _colorChangedEvent;
	};
}