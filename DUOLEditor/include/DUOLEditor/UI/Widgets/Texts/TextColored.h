/**

    @file      TextColored.h
    @brief     text can be colored.
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLEditor/UI/Widgets/Texts/Text.h"

namespace DUOLEditor
{
	/**
	 * \brief widget to display text that can be colored.
	 */
	class TextColored : public Text
	{
	public:
		TextColored(const DUOLCommon::tstring& text = TEXT(""), const Color color = Color::Black);

	public:
		Color _color;

	protected:
		virtual void Draw_Impl() override;
	};
}