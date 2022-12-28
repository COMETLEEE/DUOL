/**

    @file      ViewBase.h
    @brief     뷰 계열 패널들의 기본 클래스
    @details   ~
    @author    COMETLEE
    @date      26.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLEditor/UI/Panels/PanelWindow.h"

namespace DUOLEditor
{
	class Image;
}

namespace DUOLEditor
{
	/**
	 * \brief base class for any view.
	 */
	class ViewBase : public PanelWindow
	{
	public:
		ViewBase(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSettings);

		virtual void Draw_Impl() override;

		virtual void Render_Impl() = 0;

		void Render();

	protected:
		/**
		 * \brief 해당 view panel에서 그릴 Image widget.
		 */
		DUOLEditor::Image* _image;
	};
}