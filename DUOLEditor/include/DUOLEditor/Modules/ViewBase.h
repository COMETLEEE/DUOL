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
	class ViewBase : public DUOLEditor::PanelWindow
	{
	public:
		ViewBase(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSettings);

		/**
		 * \brief 해당 View UI가 최신 화면을 그릴 수 있도록 업데이트한다.
		 * \param deltaTime 지난 시간
		 */
		virtual void Update(float deltaTime) abstract;

		virtual void Draw_Impl() override;

		DUOLMath::Vector2 GetSafeSize() const;

	protected:
		/**
		 * \brief View Panel에서 그릴 Image widget.
		 */
		DUOLEditor::Image* _image;
	};
}