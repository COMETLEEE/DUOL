#include "DUOLEditor/UI/Widgets/Buttons/ButtonImage.h"

namespace DUOLEditor
{
	ButtonImage::ButtonImage(void* textureID, const DUOLMath::Vector2& size, bool _isDisabled) :
		_textureID(textureID)
		, _size(size)
		, _isDisabled(_isDisabled)
		, _isThisFrameDisabled(false)
	{
	}

	void ButtonImage::Draw_Impl()
	{
		// 이번 프레임에 Disabled 되었는가에 대한 체크.
		_isThisFrameDisabled = false;

		ImVec4 bg = ImGuiHelper::ToImVec4(_backgroundColor);

		ImVec4 tn = ImGuiHelper::ToImVec4(_tintColor);

		if (_isDisabled)
		{
			ImGui::BeginDisabled();

			_isThisFrameDisabled = true;
		}

		if (ImGui::ImageButton(_textureID, ImGuiHelper::ToImVec2(_size), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), -1, bg, tn))
			_clickedEvent.Invoke();

		if (_isDisabled && _isThisFrameDisabled)
		{
			ImGui::EndDisabled();
		}
	}
}