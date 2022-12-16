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

		if (_isDockable)
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
		}

		for (auto& panel : _panels)
			panel->Draw();

		ImGui::Render();
	}
}