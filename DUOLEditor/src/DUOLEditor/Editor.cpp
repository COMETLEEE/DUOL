#include "DUOLEditor/Editor.h"

#include "DUOLEditor/Modules/ContentsBrowser.h"
#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLEditor/UI/GUIManager.h"
#include "DUOLEditor/UI/Page/Page.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"


#include "DUOLEditor/Modules/Hierarchy.h"
#include "DUOLEditor/Modules/SceneView.h"
#include "DUOLEditor/Modules/GameView.h"
#include "DUOLEditor/Modules/GraphicsSetting.h"
#include "DUOLEditor/Modules/Inspector.h"
#include "DUOLEditor/Modules/MenuBar.h"
#include "DUOLEditor/Modules/Toolbar.h"
#include "DUOLGameEngine/Manager/ButtonEventManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

namespace DUOLEditor
{
	Editor::Editor() :
		_isRunning(true)
		, _gameEngine(nullptr)
		, _editorEventManager(nullptr)
		, _guiManager(nullptr)
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

		_editorEventManager->_editor = this;

		_gameengineManager = DUOLGameEngine::ButtonEventManager::GetInstance();

		// GUI를 만들어서 _guiManager에 넘겨줍시다 ..!
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
		setting.closable = false;
		setting.collapsable = true;
		setting.dockable = true;

		ImGuiIO& io = ImGui::GetIO();

		// 모든 ImGui Window는 타이틀 바의 움직임으로만 움직일 수 있도록 설정됩니다.
		io.ConfigWindowsMoveFromTitleBarOnly = true;

#pragma region MENU_BAR
		DUOLEditor::MenuBar* menuBar = _editorPage->AddPanel<DUOLEditor::MenuBar>();
#pragma endregion

#pragma region HIERARCHY_PANEL
		// Scene hierarchy panel.
		DUOLEditor::Hierarchy* hierarchy = _editorPage->AddPanel<DUOLEditor::Hierarchy>(TEXT("Hierarchy"), true, setting);
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

#pragma region CONTENTS_BROWSER
		// Contents Browser
		DUOLEditor::ContentsBrowser* contentsBrowser = _editorPage->AddPanel<DUOLEditor::ContentsBrowser>(TEXT("Contents Browser"), true, setting);
#pragma endregion

#pragma region TOOLBAR
		// Toolbar
		DUOLEditor::Toolbar* toolbar = _editorPage->AddPanel<DUOLEditor::Toolbar>(TEXT("Toolbar"), true, setting);
#pragma endregion

#pragma region GraphicsSetting
		{
			DUOLEditor::PanelWindowSetting graphicssetting;
			graphicssetting.closable = true;
			graphicssetting.collapsable = true;
			graphicssetting.dockable = true;

			DUOLEditor::GraphicsSetting* graphicsSetting = _editorPage->AddPanel<DUOLEditor::GraphicsSetting>(TEXT("GraphicsSetting"), false, graphicssetting);

			_panels.emplace(TEXT("GraphicsSetting"), graphicsSetting);
		}
		// Toolbar
#pragma endregion
	}

	void Editor::CreateEditorGUIs()
	{
		// 0. Page를 만듭니다.
		CreatePages();

		// 1. Panel 들을 만듭니다.
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

		// TODO - 이거 Debug Pass의 depth clear 문제 때문에 여기다가 놔둠 .. 빼야한다 ..!
		DUOLGameEngine::GraphicsManager::GetInstance()->ClearAllRenderTarget();
	}

	void Editor::UpdateEnginePlayMode()
	{
		// 그냥 단순한 업데이트만 하면 된다 ..
		_gameEngine->Update();

		// 업데이트를 하다가 GameEngine에서 exit가 되면 종료시켜줘야한다.
		if (_gameengineManager->_exit == true)
			DUOLEditor::EditorEventManager::GetInstance()->ExitEditor();
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

	DUOLEditor::PanelWindow* Editor::GetPanel(const DUOLCommon::tstring& panelName)
	{
		auto findPanel = _panels.find(panelName);

		if(findPanel == _panels.end())
		{
			return nullptr;
		}

		return findPanel->second;
	}

	void Editor::PostUpdate(float deltaTime)
	{
		// 엔진을 먼저 업데이트합니다.
		UpdateEngineCurrentEditorMode();

		// View 들은 그리기 전 Update가 필요합니다. (Widget 사이즈 조정 및 각 View 현재 상태 별 렌더 큐 쌓기)
		for (auto& view : _views)
			view->Update(deltaTime);
	}

	void Editor::LateUpdate(float deltaTime)
	{
		// GUI DrawData 를 만들어냅니다.
		_guiManager->Update(deltaTime);

		// Back Buffer을 Bind하고 그립니다.
		_guiManager->RenderGUI();
	}
}