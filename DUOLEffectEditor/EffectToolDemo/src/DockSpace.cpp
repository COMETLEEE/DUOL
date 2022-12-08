#include "DockSpace.h"
#include "../Common/Imgui/imgui.h"

DockSpace::DockSpace(std::shared_ptr<Muscle::GameObject> _gameObject) : Muscle::ImGuiRnedererBase(_gameObject)
{
}

void DockSpace::ShowDockSpace()
{
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->WorkPos);

	ImGui::SetNextWindowSize(viewport->WorkSize);

	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpace Demo", (bool*)0, window_flags);

	ImGui::PopStyleVar();

	ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	else
	{
		//ShowDockingDisabledMessage();
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::Separator();
			if (ImGui::MenuItem("New", " what? "))
			{
				dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
			}
			if (ImGui::MenuItem("Save", "")) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("SaveAs", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Load", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			ImGui::Separator();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void DockSpace::SetRenderingFunc()
{
	auto temp = [&]()
	{
		ShowDockSpace();
	};
	_renderingFunc = temp;
}

void DockSpace::Start()
{
	SetRenderingFunc();
}
