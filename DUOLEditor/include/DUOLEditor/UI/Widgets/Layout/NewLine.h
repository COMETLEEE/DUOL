#pragma once
#include "DUOLEditor/UI/Widgets/WidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief display empty line.
	 */
	class NewLine: public WidgetBase
	{
	protected:
		virtual void Draw_Impl() override;
	};
}