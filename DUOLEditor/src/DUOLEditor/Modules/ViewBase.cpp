#include "DUOLEditor/Modules/ViewBase.h"

#include "DUOLEditor/UI/Widgets/Display/Image.h"

namespace DUOLEditor
{
	ViewBase::ViewBase(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSettings) :
		PanelWindow(title, isOpened, windowSettings)
		, _image(nullptr)
	{
		_panelWindowSetting.scrollable = false;

		_image = AddWidget<DUOLEditor::Image>(nullptr, DUOLMath::Vector2(0.f, 0.f));
	}

	void ViewBase::Draw_Impl()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		
		PanelWindow::Draw_Impl();

		ImGui::PopStyleVar();
	}

	DUOLMath::Vector2 ViewBase::GetSafeSize() const
	{
		auto result = GetSize() - DUOLMath::Vector2{ 0.f, ImGui::GetFrameHeight() };

		return result;
	}
}