/**

    @file      TextWrapped.h
    @brief     자동 줄바꿈으로 긴 문장의 텍스트를 출력합니다.
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
	 * \brief widget to display a long text with word-wrap (자동 줄바꿈).
	 */
	class TextWrapped : public Text
	{
	public:
		TextWrapped(const DUOLCommon::tstring& text = TEXT(""));

	protected:
		virtual void Draw_Impl() override;
	};
}
