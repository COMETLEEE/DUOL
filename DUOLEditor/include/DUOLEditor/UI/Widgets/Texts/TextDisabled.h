/**

    @file      TextDisabled.h
    @brief     
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
	 * \brief Widget to display a disabled text.
	 */
	class TextDisabled : public Text
	{
	public:
		TextDisabled(const DUOLCommon::tstring& text = TEXT(""));

	protected:
		virtual void Draw_Impl() override;
	};
}
