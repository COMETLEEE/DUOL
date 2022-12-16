/**

    @file      ButtonColored.h
    @brief     button with one color.
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLEditor/UI/Widgets/Buttons/ButtonBase.h"
#include "DUOLEditor/Util/Color.h"

namespace DUOLEditor
{
	/**
	 * \brief single color button.
	 */
	class ButtonColored : public ButtonBase
	{
	public:
		ButtonColored(const DUOLCommon::tstring& label = TEXT(""),
			const Color& color = {}, bool usingAlpha = true);

	protected:
		virtual void Draw_Impl() override;

	private:
		DUOLCommon::tstring _label;

		Color _color;

		DUOLMath::Vector2 _size;

		bool _usingAlpha;
	};
}
