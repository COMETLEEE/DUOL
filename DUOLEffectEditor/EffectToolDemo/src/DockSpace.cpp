#include "DockSpace.h"
#include "../Common/Imgui/imgui.h"
#include "EffectEditorManager.h"
DockSpace::DockSpace(std::shared_ptr<Muscle::GameObject> _gameObject) : Muscle::ImGuiRnedererBase(_gameObject)
{
}

void DockSpace::ShowFileMenu()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File.."))
		{
			ImGui::Separator();
			if (ImGui::MenuItem("New", "Ctrl + S"))
			{
				EffectEditorManager::Get().NewParticle();
			}
			if (ImGui::MenuItem("Save", ""))
			{
				EffectEditorManager::Get().SaveParticle();
			}
			if (ImGui::MenuItem("SaveAs", ""))
			{
				EffectEditorManager::Get().SaveAsParticle();
			}
			if (ImGui::MenuItem("Load", ""))
			{
				EffectEditorManager::Get().LoadParticle();
			}
			if (ImGui::MenuItem("PreviousVerLoad", ""))
			{
				EffectEditorManager::Get().LoadPreviousVersionParticle();
			}
			if (ImGui::MenuItem("UpdateAllFileToLatestVersionInFolder", ""))
			{
				EffectEditorManager::Get().UpdateAllFileToLastestVersionInFolder();
			}
			ImGui::Separator();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

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


	ShowFileMenu();

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
