#include "DUOLEditor/Editor.h"

#include "DUOLEditor/UI/GUIManager.h"
#include "DUOLEditor/UI/Page/Page.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLEditor/Modules/Hierarchy.h"
#include "DUOLEditor/Modules/SceneView.h"
#include "DUOLEditor/Modules/GameView.h"
#include "DUOLEditor/Modules/Inspector.h"

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

		// GUI�� ���� _guiManager�� �Ѱ��ݽô� ..!
		CreateEditorGUIs();

#pragma region REGISTER_EDITOR_EVENT
		DUOLGameEngine::EventManager::GetInstance()->RegisterEvent<std::any>(TEXT("SelecteGameObject"));

		DUOLGameEngine::EventManager::GetInstance()->RegisterEvent<std::any>(TEXT("UnselectGameObject"));
#pragma endregion
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

#pragma region HIERARCHY_PANEL
		// Scene hierarchy panel.
		DUOLEditor::Hierarchy* hierarchy = _editorPage->AddPanel<DUOLEditor::Hierarchy>(TEXT("Hierarchy"), true, setting);

		// set start scene current scene at hierarchy panel.
		hierarchy->SetCurrentScene(DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene());
#pragma endregion

#pragma region GAME_VIEW
		// Game View (display current game view play or not.)
		DUOLEditor::GameView* gameView = _editorPage->AddPanel<DUOLEditor::GameView>(TEXT("Game"), true, setting);

		// push game view to list of all view.
		_views.push_back(gameView);
#pragma endregion

#pragma region SCENE_VIEW
		// Scene View (use editing current scene.)
		DUOLEditor::SceneView* sceneView = _editorPage->AddPanel<DUOLEditor::SceneView>(TEXT("Scene"), true, setting);

		// push scene view to list of all view.
		_views.push_back(sceneView);
#pragma endregion

#pragma region INSPECTOR
		// Inspector
		DUOLEditor::Inspector* inspector = _editorPage->AddPanel<DUOLEditor::Inspector>(TEXT("Inspector"), true, setting);
#pragma endregion
	}

	void Editor::CreateEditorGUIs()
	{
		// 0. Page�� ����ϴ�.
		CreatePages();

		// 1. Panel ���� ����ϴ�.
		CreatePanels();
	}

	void Editor::PostUpdate(float deltaTime)
	{
		// View ���� �׸��� �� Update�� �ʿ��մϴ�. (Widget ������ ���� �� �� View ���� ���� �� ���� ť �ױ�)
		for (auto& view : _views)
			view->Update(deltaTime);
	}

	void Editor::LateUpdate(float deltaTime)
	{
		// GUI DrawData �� �������ϴ�.
		_guiManager->Update(deltaTime);

		// Back Buffer�� Bind�ϰ� �׸��ϴ�.
		_guiManager->RenderGUI();
	}
}