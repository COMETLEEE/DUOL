#include "DUOLEditor/UI/Page/Page.h"

#include "DUOLEditor/UI/Panels/PanelBase.h"

#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLCommon/ImGui/imgui_impl_dx11.h"
#include "DUOLCommon/ImGui/imgui_impl_win32.h"

namespace DUOLEditor
{
	Page::Page() :
		_isDockable(true)
		, _panels(std::list<std::shared_ptr<DUOLEditor::PanelBase>>())
	{
	}

	Page::~Page()
	{
		RemoveAllPanels();
	}

	void Page::RemovePanel(DUOLEditor::PanelBase* panel)
	{
		std::erase_if(_panels, [&panel](auto& panelIn)
			{
				if (panelIn.get() == panel)
				{
					panelIn.reset();

					return true;
				}
				else
					return false;
			});
	}

	void Page::RemoveAllPanels()
	{
		for (auto& panel : _panels)
			panel.reset();

		_panels.clear();
	}

	void Page::SetIsDockable(bool value)
	{
		_isDockable = value;
	}

	bool Page::GetIsDockable()
	{
		return _isDockable;
	}

	void Page::Draw()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		/*if (_isDockable)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("##dockspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

			ImGui::SetWindowPos({ 0.f, 0.f });
			ImVec2 displaySize = ImGui::GetIO().DisplaySize;
			ImGui::SetWindowSize({ (float)displaySize.x, (float)displaySize.y });
			ImGui::End();

			ImGui::PopStyleVar(3);
		}*/

		for (auto& panel : _panels)
			panel->Draw();

#pragma region TEST_VIEW
		// Please do not forget this!
		ImGui::Begin("Example Bug");

		ImColor color = ImColor(20, 20, 20, 64);

		float lineHeight = -1.f;

		auto* drawList = ImGui::GetWindowDrawList();
		const auto& style = ImGui::GetStyle();

		if (lineHeight < 0)
		{
			lineHeight = ImGui::GetTextLineHeight();
		}
		lineHeight += style.ItemSpacing.y;

		float scrollOffsetH = ImGui::GetScrollX();
		float scrollOffsetV = ImGui::GetScrollY();
		float scrolledOutLines = floorf(scrollOffsetV / lineHeight);
		scrollOffsetV -= lineHeight * scrolledOutLines;

		ImVec2 clipRectMin(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
		ImVec2 clipRectMax(clipRectMin.x + ImGui::GetWindowWidth(), clipRectMin.y + ImGui::GetWindowHeight());

		if (ImGui::GetScrollMaxX() > 0)
		{
			clipRectMax.y -= style.ScrollbarSize;
		}

		drawList->PushClipRect(clipRectMin, clipRectMax);

		bool isOdd = (static_cast<int>(scrolledOutLines) % 2) == 0;

		float yMin = clipRectMin.y - scrollOffsetV + ImGui::GetCursorPosY();
		float yMax = clipRectMax.y - scrollOffsetV + lineHeight;
		float xMin = clipRectMin.x + scrollOffsetH + ImGui::GetWindowContentRegionMin().x;
		float xMax = clipRectMin.x + scrollOffsetH + ImGui::GetWindowContentRegionMax().x;

		for (float y = yMin; y < yMax; y += lineHeight, isOdd = !isOdd)
		{
			if (isOdd)
			{
				drawList->AddRectFilled({ xMin, y - style.ItemSpacing.y }, { xMax, y + lineHeight }, color);
			}
		}

		drawList->PopClipRect();

		// whatever code that draws list or tree items
		ImGui::End();
#pragma endregion

		// Draw Data±îÁö »Ì¾Æ³À´Ï´Ù.
		ImGui::Render();
	}
}