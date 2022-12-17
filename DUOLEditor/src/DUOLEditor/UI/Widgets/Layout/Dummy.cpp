#include "DUOLEditor/UI/Widgets/Layout/Dummy.h"

namespace DUOLEditor
{
	Dummy::Dummy(const DUOLMath::Vector2& size) :
		_size(size)
	{
	}

	void Dummy::Draw_Impl()
	{
		ImGui::Dummy(ImGuiHelper::ToImVec2(_size));
	}
}