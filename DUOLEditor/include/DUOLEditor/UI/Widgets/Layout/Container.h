#pragma once
#include "DUOLEditor/UI/Widgets/WidgetGroupBase.h"

namespace DUOLEditor
{
	/**
	 * \brief Widget that can contains other widgets.
	 */
	class Container : public WidgetBase, public WidgetGroupBase
	{
	protected:
		virtual void Draw_Impl() override;
	};
}