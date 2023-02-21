/**

    @file      ButtonImage.h
    @brief     이미지가 그려진 버튼 (게임 플레이, 일시 정지, 중지 버튼 등 ..)
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#include "DUOLEditor/UI/Widgets/Buttons/ButtonBase.h"

namespace DUOLEditor
{
	/**
	 * \brief button with an image;
	 */
	class ButtonImage : public ButtonBase
	{
	public:
		ButtonImage(void* textureID, const DUOLMath::Vector2& size, bool isDisabled = false);

	protected:
		void Draw_Impl() override;

	public:
		bool _isDisabled;

		bool _isThisFrameDisabled;

		Color _backgroundColor = { 0.f, 0.f, 0.f, 0.f};

		Color _tintColor = { 1.f, 1.f, 1.f, 1.f };

		void* _textureID;

		DUOLMath::Vector2 _size;
	};
}