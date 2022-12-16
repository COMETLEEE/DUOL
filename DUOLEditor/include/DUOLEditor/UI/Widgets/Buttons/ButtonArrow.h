/**

    @file      ButtonArrow.h
    @brief     arrow image button.
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLEditor/UI/Widgets/Buttons/ButtonBase.h"

namespace DUOLEditor
{
	/**
	 * \brief Button with an arrow image.
	 */
	class ButtonArrow : public ButtonBase
	{
	public:
		ButtonArrow(ImGuiDir direction = ImGuiDir_None);

	protected:
		virtual void Draw_Impl() override;
		
	private:
		ImGuiDir _direction;
	};
}