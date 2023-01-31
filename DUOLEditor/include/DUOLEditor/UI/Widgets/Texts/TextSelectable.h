/**

    @file      TextSelectable.h
    @brief     draw text that selectable.
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/Texts/Text.h"

namespace DUOLEditor
{
	/**
	 * \brief widget to display a selectable text.
	 */
	class TextSelectable : public Text
	{
	public:
		TextSelectable(const DUOLCommon::tstring& text = TEXT(""), bool isSelected = false, bool isDisabled = false);

	protected:
		virtual void Draw_Impl() override;

	public:
		bool _isSelected;

		bool _isDisabled;

		DUOLCommon::Event<void, bool> _clickedEvent;

		DUOLCommon::Event<void> _selectedEvent;

		DUOLCommon::Event<void> _unSelectedEvent;
	};
}