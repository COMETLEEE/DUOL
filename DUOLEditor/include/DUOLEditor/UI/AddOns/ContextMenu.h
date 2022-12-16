/**

    @file      ContextMenu.h
    @brief     우클릭으로 사용할 수 있는 컨텍스트 메뉴
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLEditor/UI/AddOns/AddOnBase.h"
#include "DUOLEditor/UI/Widgets/WidgetGroupBase.h"

namespace DUOLEditor
{
	/**
	 * \brief you can show context menu on right click.
	 * and you can add widgets to a context menu.
	 */
	class ContextMenu : public AddOnBase, public WidgetGroupBase
	{
	public:
		virtual ~ContextMenu() override = default;

		virtual void Execute() override;

		void Close();
	};
}