#include "DUOLEditor/UI/Widgets/Display/Image.h"

namespace DUOLEditor
{
	Image::Image(void* textureID, const DUOLMath::Vector2 size) :
		_textureID(textureID)
		, _size(size)
	{
	}

	void Image::Draw_Impl()
	{
		ImGui::Image(_textureID, ImGuiHelper::ToImVec2(_size), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
	}
}