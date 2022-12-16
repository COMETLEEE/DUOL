/**

    @file      ButtonBase.h
    @brief     ButtonBase class.
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLCommon/Event/Event.h"

#include "DUOLEditor/UI/Widgets/WidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief Base class for any button widget class.
	 */
	class ButtonBase : public WidgetBase
	{
	protected:
		void Draw_Impl() override = 0;

	public:
		DUOLCommon::Event<void> _clickedEvent;
	};
}