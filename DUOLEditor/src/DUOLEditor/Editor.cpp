#include "DUOLEditor/Editor.h"

#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLCommon/ImGui/imgui_impl_dx11.h"
#include "DUOLCommon/ImGui/imgui_impl_win32.h"
#include "DUOLEditor/UI/GUIManager.h"

namespace DUOLEditor
{
	Editor::Editor()
	{

	}

	Editor::~Editor()
	{

	}

	void Editor::Initialize(DUOLGameEngine::Engine* gameEngine, DUOLGameEngine::EditorModeOption* editorModeOption)
	{
		_gameEngine = gameEngine;

		_editorModeOption = editorModeOption;

		_guiManager = DUOLEditor::GUIManager::GetInstance();
	}

	void Editor::Update(float deltaTime)
	{
		PostUpdate(deltaTime);

		_guiManager->Update(deltaTime);

		LateUpdate(deltaTime);
	}

	void Editor::PostUpdate(float deltaTime)
	{

	}

	void Editor::LateUpdate(float deltaTime)
	{

	}
}