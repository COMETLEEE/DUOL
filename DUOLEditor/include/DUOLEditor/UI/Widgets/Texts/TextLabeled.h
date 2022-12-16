/**

    @file      TextLabelled.h
    @brief     draw labeled text
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
	 * \brief widget to display a labeled text.
	 */
	class TextLabeled : public Text
	{
	public:
		TextLabeled(const DUOLCommon::tstring& text = TEXT(""), const DUOLCommon::tstring& label = TEXT(""));

	protected:
		virtual void Draw_Impl() override;

	public:
		DUOLCommon::tstring _label;
	};
}
