/**

    @file      Separator.h
    @brief     draw simple separator.
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLEditor/UI/Widgets/WidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief draw separator.
	 */
	class Separator : public WidgetBase
	{
	protected:
		virtual void Draw_Impl() override;
	};
}