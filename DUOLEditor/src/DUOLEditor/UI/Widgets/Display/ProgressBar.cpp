#include "DUOLEditor/UI/Widgets/Display/ProgressBar.h"

namespace DUOLEditor
{
	ProgressBar::ProgressBar(const DUOLCommon::tstring& overlay, float fraction, const DUOLMath::Vector2& size) :
		_fraction(fraction)
		, _size(size)
		, _overlay(overlay)
	{
	}

	void ProgressBar::Draw_Impl()
	{
		ImGui::ProgressBar(_fraction, ImGuiHelper::ToImVec2(_size), !_overlay.empty() ? DUOLCommon::StringHelper::ToString(_overlay).c_str() : nullptr);
	}
}