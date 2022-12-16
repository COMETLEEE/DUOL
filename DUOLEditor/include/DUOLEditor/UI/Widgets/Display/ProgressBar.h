/**

    @file      ProgressBar.h
    @brief     Simple progressbar.
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLEditor/UI/Widgets/WidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief draw progress bar.
	 */
	class ProgressBar : public WidgetBase
	{
	public:
		ProgressBar(const DUOLCommon::tstring& overlay = TEXT(""), float fraction = 0.f, const DUOLMath::Vector2& size = DUOLMath::Vector2::Zero);

	protected:
		virtual void Draw_Impl() override;

	private:
		float _fraction;

		DUOLMath::Vector2 _size;

		DUOLCommon::tstring _overlay;
	};
}
