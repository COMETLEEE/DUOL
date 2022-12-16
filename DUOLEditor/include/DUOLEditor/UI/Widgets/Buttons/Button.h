/**

    @file      Button.h
    @brief     Simple button implemented by imgui.
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLCommon/StringHelper.h"
#include "DUOLEditor/UI/Widgets/Buttons/ButtonBase.h"
#include "DUOLEditor/Util/Color.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLEditor
{
	/**
	 * \brief Simple button.
	 */
	class Button : public DUOLEditor::ButtonBase
	{
	public:
		Button(const DUOLCommon::tstring& label = TEXT("Button"),
			const DUOLMath::Vector2& size = DUOLMath::Vector2::Zero, bool isDisabled = false);

	protected:
		virtual void Draw_Impl() override;

	private:
		DUOLCommon::tstring _label;

		DUOLMath::Vector2 _size;

		bool _isDisabled;

		Color _textColor;

		Color _normalColor;

		Color _highlightedColor;

		Color _pressedColor;
	};
}