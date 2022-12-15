/**

    @file      PanelBase.h
    @brief     페이지를 구성하며 한 가지 기능을 완성하는 UI의 베이스 클래스.
    @details   ~
    @author    COMETLEE
    @date      15.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLEditor/UI/GUIBase.h"
#include "DUOLEditor/UI/Widgets/WidgetGroupBase.h"

namespace DUOLEditor
{
	/**
	 * \brief page's gadgets. 
	 */
	class PanelBase : public DUOLEditor::GUIBase, public DUOLEditor::WidgetGroupBase
    {
	public:
        PanelBase();

        virtual ~PanelBase() override;

        bool GetIsEnable() const;

        void SetIsEnable(bool value);

        virtual void Draw() override;

	protected:
        virtual void Draw_Impl() = 0;

	private:
        bool _isEnable;
    };
}
