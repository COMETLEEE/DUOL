#include "DUOLEditor/Editor.h"

#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLCommon/ImGui/imgui_impl_dx11.h"
#include "DUOLCommon/ImGui/imgui_impl_win32.h"
#include "DUOLEditor/UI/GUIManager.h"

namespace DUOLEditor
{
	Editor::Editor(const std::shared_ptr<DUOLGameEngine::Engine>& engine) :
		_gameEngine(engine)
	{

	}

	Editor::~Editor()
	{
		_gameEngine.reset();
	}

	void Editor::Update(float deltaTime)
	{
		// GUIManager Update
		DUOLEditor::GUIManager::GetInstance()->Update(deltaTime);
	}
}