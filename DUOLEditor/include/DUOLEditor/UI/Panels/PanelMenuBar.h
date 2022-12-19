#pragma once
#include "DUOLEditor/UI/Panels/PanelBase.h"

namespace DUOLEditor
{
	/**
	 * \brief display top side of the canvas.
	 */
	class PanelMenuBar : public PanelBase
	{
	protected:
		virtual void Draw_Impl() override;
	};
}
