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
	/**
	 * \brief base class for any view.
	 */
	class ViewBase : public PanelWindow
	{
	public:
		ViewBase(const DUOLCommon::tstring& title, bool opened, const PanelWindowSetting& windowSettings);

		/**
		 * \brief Update the view.
		 * \param deltaTime time from prev frame to current.
		 */
		virtual void Update(float deltaTime);

		virtual void Draw_Impl() override;

		virtual void Render_Impl() = 0;

		void Render();

		
	};
}