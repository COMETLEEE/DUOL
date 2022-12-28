#include "DUOLEditor/Editor.h"

#include "DUOLEditor/UI/GUIManager.h"
#include "DUOLEditor/UI/Page/Page.h"

#include "DUOLEditor/Modules/Hierarchy.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

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

		// GUI를 만들어서 _guiManager에 넘겨줍시다 ..!
		CreateEditorGUIs();
	}

	void Editor::CreatePages()
	{
		_editorPage = std::make_shared<DUOLEditor::Page>();

		_guiManager->SetPage(_editorPage);
	}

	void Editor::CreatePanels()
	{
		DUOLEditor::PanelWindowSetting setting;
		setting.closable = true;
		setting.collapsable = true;
		setting.dockable = true;

		// 1. Scene hierarchy panel.
		DUOLEditor::Hierarchy* hierarchy = _editorPage->AddPanel<DUOLEditor::Hierarchy>(TEXT("Hierarchy"), true, setting);

		// 2. set current scene.
		hierarchy->SetCurrentScene(DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene());
	}

	void Editor::CreateEditorGUIs()
	{
		// 0. Page를 만듭니다.
		CreatePages();

		// 1. Panel 들을 만듭니다.
		CreatePanels();
	}

	void Editor::Update(float deltaTime)
	{
		PostUpdate(deltaTime);

		LateUpdate(deltaTime);
	}

	void Editor::PostUpdate(float deltaTime)
	{
		// GUI DrawData 를 만들어냅니다.
		_guiManager->Update(deltaTime);
	}

	void Editor::LateUpdate(float deltaTime)
	{
		// Back Buffer을 Bind하고 그립니다.
		_guiManager->RenderGUI();
	}
}
