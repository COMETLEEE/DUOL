#include "DUOLEditor/Editor.h"

#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLEditor/UI/GUIManager.h"
#include "DUOLEditor/UI/Page/Page.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"


#include "DUOLEditor/Modules/Hierarchy.h"
#include "DUOLEditor/Modules/SceneView.h"
#include "DUOLEditor/Modules/GameView.h"
#include "DUOLEditor/Modules/Inspector.h"
#include "DUOLEditor/Modules/Toolbar.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

namespace DUOLEditor
{
	Editor::Editor()
	{
		
	}

	Editor::~Editor()
	{

	}

	void Editor::Initialize(DUOLGameEngine::Engine* gameEngine)
	{
		_gameEngine = gameEngine;

		_guiManager = DUOLEditor::GUIManager::GetInstance();

		_editorEventManager = DUOLEditor::EditorEventManager::GetInstance();

		// GUI�� ���� _guiManager�� �Ѱ��ݽô� ..!
		CreateEditorGUIs();

#pragma region REGISTER_GAMEENGINE_EVENT
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

		ImGuiIO& io = ImGui::GetIO();

		// ��� ImGui Window�� Ÿ��Ʋ ���� ���������θ� ������ �� �ֵ��� �����˴ϴ�.
		io.ConfigWindowsMoveFromTitleBarOnly = true;

#pragma region HIERARCHY_PANEL
		// Scene hierarchy panel.
		DUOLEditor::Hierarchy* hierarchy = _editorPage->AddPanel<DUOLEditor::Hierarchy>(TEXT("Hierarchy"), true, setting);

		// set start scene current scene at hierarchy panel.
		// hierarchy->SetCurrentScene(DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene());
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

#pragma region TOOLBAR
		// Toolbar
		DUOLEditor::Toolbar* toolbar = _editorPage->AddPanel<DUOLEditor::Toolbar>(TEXT("Toolbar"), true, setting);
#pragma endregion
	}

	void Editor::CreateEditorGUIs()
	{
		// 0. Page�� ����ϴ�.
		CreatePages();

		// 1. Panel ���� ����ϴ�.
		CreatePanels();
	}

	void Editor::UpdateEngineCurrentEditorMode()
	{
		switch (_editorEventManager->_editorMode)
		{
			case EditorMode::Edit:
			{
				UpdateEngineEditMode();

				break;
			}

			case EditorMode::Play:
			{
				UpdateEnginePlayMode();

				break;
			}

			case EditorMode::Pause:
			{
				UpdateEnginePauseMode();

				break;
			}

			case EditorMode::FRAME_BY_FRAME:
			{
				UpdateEngineFrameMode();
				
				break;
			}
		}

		// TODO - �̰� Debug Pass�� depth clear ���� ������ ����ٰ� ���� .. �����Ѵ� ..!
		DUOLGameEngine::GraphicsManager::GetInstance()->ClearAllRenderTarget();
	}

	void Editor::UpdateEnginePlayMode()
	{
		// �׳� �ܼ��� ������Ʈ�� �ϸ� �ȴ� ..
		_gameEngine->Update();
	}

	void Editor::UpdateEnginePauseMode()
	{
		_gameEngine->UpdatePauseMode();
	}

	void Editor::UpdateEngineEditMode()
	{
		_gameEngine->UpdateEditMode();
	}

	void Editor::UpdateEngineFrameMode()
	{
		_gameEngine->UpdateFrameMode();
	}

	void Editor::PostUpdate(float deltaTime)
	{
		UpdateEngineCurrentEditorMode();

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