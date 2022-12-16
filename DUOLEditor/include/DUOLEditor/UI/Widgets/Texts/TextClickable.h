/**

    @file      TextClickable.h
    @brief     Clickable text.
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLEditor/UI/Widgets/Texts/Text.h"

#include "DUOLCommon/Event/Event.h"

namespace DUOLEditor
{
	/**
	 * \brief widget to display text that is also clickable.
	 */
	class TextClickable : public Text
	{
	public:
		TextClickable(const DUOLCommon::tstring& text = TEXT(""));

	protected:
		virtual void Draw_Impl() override;

	public:
		DUOLCommon::Event<void> _doubleClickedEvent;

		DUOLCommon::Event<void> _clickedEvent;
	};
}