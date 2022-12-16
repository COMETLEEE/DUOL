/**

    @file      ButtonSmall.h
    @brief     small size button.
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
	 * \brief small button.
	 */
	class ButtonSmall : public ButtonBase
	{
	public:
		ButtonSmall(const DUOLCommon::tstring& label = TEXT(""));

	protected:
		virtual void Draw_Impl() override;

	private:
		DUOLCommon::tstring _label;

		Color _textColor;

		Color _normalColor;

		Color _highlightedColor;

		Color _pressedColor;
	};
}