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
	 * \brief defines horizontal alignments.
	 */
	enum class HorizontalAlignment
    {
	    Left
        , Center
        , Right
    };

	/**
	 * \brief defines vertical alignment.
	 */
	enum class VerticalAlignment
    {
	    Top
        , Middle
        , Bottom
    };

	/**
	 * \brief defines panel windows setting.
	 */
	struct PanelWindowSetting
    {
		bool closable = false;
		bool resizable = true;
		bool movable = true;
		bool dockable = false;
		bool scrollable = true;
		bool hideBackground = false;
		bool forceHorizontalScrollbar = false;
		bool forceVerticalScrollbar = false;
		bool allowHorizontalScrollbar = false;
		bool bringToFrontOnFocus = true;
		bool collapsable = false;
		bool allowInputs = true;
		bool titleBar = true;
		bool autoSize = false;
    };

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
