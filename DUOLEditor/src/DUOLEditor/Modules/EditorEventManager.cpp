#include "DUOLEditor/Modules/EditorEventManager.h"

#include <filesystem>

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SerializeManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLEditor/Editor.h"
#include "DUOLEditor/Util/FileDialog/ReadFileDialog.h"
#include "DUOLEditor/Util/FileDialog/WriteFileDialog.h"
#include "DUOLEditor/Util/MessageBox/MessageBox.h"
#include "DUOLGameEngine/Manager/UnityMigrator/UnityMigrator.h"

namespace DUOLEditor
{
	EditorEventManager::EditorEventManager() :
		_editorMode(EditorMode::Edit)
	{
		DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SceneChanging"), [this]()
			{
				DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

				// 씬 내에서 게임 오브젝트가 생성될 때 에디터도 알아야한다 ..!
				currentScene->_gameObjectCreatedEvent += std::bind(&DUOLEditor::EditorEventManager::CreateGameObject, this, std::placeholders::_1);

				// 씬 내에서 게임 오브젝트가 삭제될 때 에디터도 알아야한다 ..!
				currentScene->_gameObjectDeletedEvent += std::bind(&DUOLEditor::EditorEventManager::DeleteGameObject, this, std::placeholders::_1);

				_sceneChangedEvent.Invoke(currentScene);
			});
	}

	EditorEventManager::~EditorEventManager()
	{
	}

	void EditorEventManager::CreateGameObject(DUOLGameEngine::GameObject* gameObject)
	{
		_createGameObjectEvent.Invoke(gameObject);

		// TODO : 어차피, 다른 곳에서 재귀적으로 호출해준다. 그리고 그게 맞는 것 같다 ..!
		// 재귀적으로 자식 오브젝트에 대한 이벤트도 실행합니다.
		/*auto&& childrens = gameObject->GetTransform()->GetAllChildGameObjects();

		for (auto& child : childrens)
			_createGameObjectEvent.Invoke(child);*/
	}

	void EditorEventManager::DeleteGameObject(DUOLGameEngine::GameObject* gameObject)
	{
		_deleteGameObjectEvent.Invoke(gameObject);

		// TODO : 어차피, 다른 곳에서 재귀적으로 호출해준다. 그리고 그게 맞는 것 같다 ..!
		//// 재귀적으로 자식 오브젝트에 대한 이벤트도 실행합니다.
		//auto&& childrens = gameObject->GetTransform()->GetAllChildGameObjects();

		//for (auto& child : childrens)
		//	_deleteGameObjectEvent.Invoke(child);
	}

	void EditorEventManager::SelectGameObject(DUOLGameEngine::GameObject* gameObject)
	{
		_gameObjectSelectedEvent.Invoke(gameObject);
	}

	void EditorEventManager::UnselectGameObject()
	{
		_gameObjectUnselectedEvent.Invoke();
	}

	void EditorEventManager::SetEditorMode(DUOLEditor::EditorMode editorMode)
	{
		_editorMode = editorMode;
		_editorModeChangedEvent.Invoke(_editorMode);
	}

	void EditorEventManager::StartGame()
	{
		// 에디트 모드에서 실행할 때
		if (_editorMode == EditorMode::Edit)
		{
			// Play Event ..?

			// TODO : 저장할 것입니다. 그래도 실행하시겠습니까 ..?
			MessageBox_ message(TEXT("Play Mode"),
				TEXT("Is it okay to save \n and enter play mode? "),
				MessageBox_::MessageType::WARNING, MessageBox_::ButtonLayout::YES_NO, true);

			switch (message.GetUserChoice())
			{
				// 진입 O
				case MessageBox_::UserChoice::YES: break;

				// 진입 X
				case MessageBox_::UserChoice::NO: return;
			}

			// 강제로 현재 씬을 시리얼라이즈합니다. (저장, 세이브가 안 되어 있을 수도 있으니까 ..!)
			if (DUOLGameEngine::SceneManager::GetInstance()->GetIsCurrentSceneLoadedFromFile())
			{
				SaveLoadedFromFileScene();
			}
			else
			{
				SaveAs();
			}
		}
		// 아닌 경우
		else
		{
			// 그대로 스타트 하는건가 ?
		}

		// 현재 씬에 대해서 일으키고 시작 ..!
		DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		currentScene->AwakeAllGameObject();

		currentScene->StartAllGameObject();

		SetEditorMode(EditorMode::Play);
	}

	void EditorEventManager::PauseGame()
	{
		SetEditorMode(EditorMode::Pause);
	}

	void EditorEventManager::StopGame()
	{
		if (_editorMode != EditorMode::Edit)
		{
			DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

			const DUOLCommon::tstring& currentSceneName = currentScene->GetName();

			// 게임 오브젝트 언 셀렉팅부터 ..
			_gameObjectUnselectedEvent.Invoke();

			// 현재 씬 파일을 자동으로 로드해서 가져옵니다. 여기서 Scene Changed Event On..
			DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(currentScene->GetPath());
		}

		SetEditorMode(EditorMode::Edit);
	}

	void EditorEventManager::NextFrame()
	{
		if (_editorMode == EditorMode::Play || _editorMode == EditorMode::Pause)
			SetEditorMode(EditorMode::FRAME_BY_FRAME);
	}

	void EditorEventManager::ExitEditor()
	{
		if (_editorMode != EditorMode::Edit)
			SetEditorMode(EditorMode::Edit);

		// 에디터를 꺼줍니다.
		_editor->_isRunning = false;
	}

	void EditorEventManager::SaveLoadedFromFileScene()
	{
		DUOLGameEngine::SceneManager::GetInstance()->SaveCurrentSceneTo(DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetPath());
	}

	void EditorEventManager::SaveAs()
	{
		// 파일 탐색기 열어라.
		WriteFileDialog dialog(TEXT("Save Scene"));

		std::filesystem::path absolutePath = std::filesystem::absolute(TEXT("Asset/Scene/New Scene"));

		dialog.SetInitializeDirectory(absolutePath.c_str());
		dialog.DefineExtension(TEXT("DUOL Scene"), TEXT(".dscene"));
		dialog.ShowDialog();

		// 그리고 이름, 뭐 등 지정할 수 있게 하고 저장 ..!
		if (dialog.IsSucceeded())
		{
			if (dialog.IsFileExisting())
			{
				MessageBox_ message(TEXT("File already exists !"),
					TEXT("The File \"") + dialog.GetSelectedFileName()+ TEXT("\"already exists.\n\nUsing this file as the new home for your scene will erase any content stored in this file.\n\nAre you ok with that?"),
					MessageBox_::MessageType::WARNING, MessageBox_::ButtonLayout::YES_NO, true);

				switch (message.GetUserChoice())
				{
					case MessageBox_::UserChoice::YES: break;
					case MessageBox_::UserChoice::NO: return;
				}
			}

			DUOLGameEngine::SceneManager::GetInstance()->SaveCurrentSceneTo(dialog.GetSelectedFilePath());
			DUOL_INFO(DUOL_CONSOLE, "Scene saved to : " + DUOLCommon::StringHelper::ToString(dialog.GetSelectedFilePath()));
		}
	}

	void EditorEventManager::LoadEmptyScene()
	{
		// 만약, 현재 에디팅 중인 상태가 아니라면 아무 것도 하지 않는다.
		if (_editorMode != EditorMode::Edit)
			return;

		// 만약, 현재 씬이 파일로 부터 불려져 왔다면 수정 사항이 있을 수도 있으니 저장하자.
		SaveScene();

		// Empty Scene 을 로드합니다.
		DUOLGameEngine::SceneManager::GetInstance()->LoadEmptyScene();
	}

	void EditorEventManager::OpenScene()
	{
		if (_editorMode != EditorMode::Edit)
			return;

		// 1. Before open saved scene, save current scene.
		MessageBox_ message(TEXT("Save Scene"),
			TEXT("Before open scene file, do you want save current scene ?"),
			MessageBox_::MessageType::QUESTION, MessageBox_::ButtonLayout::YES_NO, true);

		// Yes 고르면
		switch (message.GetUserChoice())
		{
			case MessageBox_::UserChoice::YES:
			{
				SaveScene();

				break;
			}

			case MessageBox_::UserChoice::NO: break;
		}

		// 2. Open saved scene file.
		ReadFileDialog dialog(TEXT("Open Scene"));

		std::filesystem::path absolutePath = std::filesystem::absolute(TEXT("Asset/Scene/New"));

		dialog.SetInitializeDirectory(absolutePath);
		dialog.AddFileType(TEXT("DUOL Scene (*.dscene)"), TEXT("*.dscene;"));
		dialog.ShowDialog(ExplorerFlags::PATHMUSTEXIST | ExplorerFlags::NOCHANGEDIR);

		// 그리고 이름, 뭐 등 지정할 수 있게 하고 저장 ..!
		if (dialog.IsSucceeded())
		{
			// 파일이 존재한다면.
			if (dialog.IsFileExisting())
			{
				// 게임 오브젝트 언 셀렉팅부터 ..
				_gameObjectUnselectedEvent.Invoke();

				// 현재 씬 파일을 자동으로 로드해서 가져옵니다. 여기서 Scene Changed Event On..
				if (DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(dialog.GetSelectedFilePath()))
					DUOL_INFO(DUOL_CONSOLE, "Open saved scene from : " + DUOLCommon::StringHelper::ToString(dialog.GetSelectedFilePath()))
				else
					DUOL_WARN(DUOL_CONSOLE, "Failed to open saved scene from : " + DUOLCommon::StringHelper::ToString(dialog.GetSelectedFilePath()))
			}
		}
	}

	void EditorEventManager::OpenUnityScene()
	{
		if (_editorMode != EditorMode::Edit)
			return;

		// 1. Before open saved scene, save current scene.
		MessageBox_ message(TEXT("Save Scene"),
			TEXT("Before open scene file, do you want save current scene ?"),
			MessageBox_::MessageType::QUESTION, MessageBox_::ButtonLayout::YES_NO, true);

		// Yes 고르면
		switch (message.GetUserChoice())
		{
		case MessageBox_::UserChoice::YES:
		{
			SaveScene();

			break;
		}

		case MessageBox_::UserChoice::NO: break;
		}

		// 2. Open saved scene file.
		ReadFileDialog dialog(TEXT("Open Scene"));

		std::filesystem::path absolutePath = std::filesystem::absolute(TEXT("Asset/Scene_Unity/New"));

		dialog.SetInitializeDirectory(absolutePath);
		dialog.AddFileType(TEXT("Unity Scene (*.unity)"), TEXT("*.unity;"));
		dialog.ShowDialog(ExplorerFlags::PATHMUSTEXIST | ExplorerFlags::NOCHANGEDIR);

		// 그리고 이름, 뭐 등 지정할 수 있게 하고 저장 ..!
		if (dialog.IsSucceeded())
		{
			// 파일이 존재한다면.
			if (dialog.IsFileExisting())
			{
				// 게임 오브젝트 언 셀렉팅부터 ..
				_gameObjectUnselectedEvent.Invoke();

				// 현재 씬 파일을 자동으로 로드해서 가져옵니다. 여기서 Scene Changed Event On..
				if (DUOLGameEngine::SceneManager::GetInstance()->LoadUnityScene(dialog.GetSelectedFilePath()) != nullptr)
				{
					DUOL_INFO(DUOL_CONSOLE, "Open unity scene from : " + DUOLCommon::StringHelper::ToString(dialog.GetSelectedFilePath()))
				}
				else
					DUOL_WARN(DUOL_CONSOLE, "Failed to open unity scene from : " + DUOLCommon::StringHelper::ToString(dialog.GetSelectedFilePath()))
			}
		}
	}

	void EditorEventManager::SaveScene()
	{
		// 만약, 현재 에디팅 중인 상태가 아니라면 아무 것도 하지 않는다.
		if (_editorMode != EditorMode::Edit)
			return;

		if (DUOLGameEngine::SceneManager::GetInstance()->GetIsCurrentSceneLoadedFromFile())
		{
			SaveLoadedFromFileScene();
		}
		else
		{
			SaveAs();
		}
	}

	DUOLCommon::Event<void, DUOLGameEngine::GameObject*>& EditorEventManager::GetGameObjectSelectedEvent()
	{
		return _gameObjectSelectedEvent;
	}

	DUOLCommon::Event<void>& EditorEventManager::GetGameObjectUnselectedEvent()
	{
		return _gameObjectUnselectedEvent;
	}

	DUOLCommon::Event<void, DUOLGameEngine::GameObject*>& EditorEventManager::GetCreateGameObjectEvent()
	{
		return _createGameObjectEvent;
	}

	DUOLCommon::Event<void, DUOLGameEngine::GameObject*>& EditorEventManager::GetDeleteGameObjectEvent()
	{
		return _deleteGameObjectEvent;
	}

	DUOLCommon::Event<void, DUOLEditor::EditorMode>& EditorEventManager::GetEditorModeChangedEvent()
	{
		return _editorModeChangedEvent;
	}

	DUOLCommon::Event<void, DUOLGameEngine::Scene*>& EditorEventManager::GetSceneChangedEvent()
	{
		return _sceneChangedEvent;
	}
}