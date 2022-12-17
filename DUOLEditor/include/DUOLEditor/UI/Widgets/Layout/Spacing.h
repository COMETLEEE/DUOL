#pragma once
#include "DUOLEditor/UI/Widgets/WidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief add spaces to panel.
	 */
	class Spacing : public WidgetBase
	{
	public:
		Spacing(int spaces = 1);

	protected:
		virtual void Draw_Impl() override;

	public:
		int _spaces;
	};
}