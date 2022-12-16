/**

    @file      Image.h
    @brief     draw an image.
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
	 * \brief draw an image.
	 */
	class Image : public WidgetBase
	{
	public:
		Image(void* textureID, const DUOLMath::Vector2 size);

	protected:
		virtual void Draw_Impl() override;

	public:
		void* _textureID;

		DUOLMath::Vector2 _size;
	};
}