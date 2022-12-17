#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief Widget that can open a color picker on click
	 */
	class ColorEdit : public DataWidgetBase<Color>
	{
	public:
		ColorEdit(bool enableAlpha = false, const Color& defaultColor = {});

	protected:
		virtual void Draw_Impl() override;

	public:
		bool _isEnableAlpha;

		Color _color;

		DUOLCommon::Event<void, Color> _colorChangedEvent;
	};
}