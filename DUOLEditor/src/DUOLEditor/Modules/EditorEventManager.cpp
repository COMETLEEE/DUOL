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

				// �� ������ ���� ������Ʈ�� ������ �� �����͵� �˾ƾ��Ѵ� ..!
				currentScene->_gameObjectCreatedEvent += std::bind(&DUOLEditor::EditorEventManager::CreateGameObject, this, std::placeholders::_1);

				// �� ������ ���� ������Ʈ�� ������ �� �����͵� �˾ƾ��Ѵ� ..!
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

		// TODO : ������, �ٸ� ������ ��������� ȣ�����ش�. �׸��� �װ� �´� �� ���� ..!
		// ��������� �ڽ� ������Ʈ�� ���� �̺�Ʈ�� �����մϴ�.
		/*auto&& childrens = gameObject->GetTransform()->GetAllChildGameObjects();

		for (auto& child : childrens)
			_createGameObjectEvent.Invoke(child);*/
	}

	void EditorEventManager::DeleteGameObject(DUOLGameEngine::GameObject* gameObject)
	{
		_deleteGameObjectEvent.Invoke(gameObject);

		// TODO : ������, �ٸ� ������ ��������� ȣ�����ش�. �׸��� �װ� �´� �� ���� ..!
		//// ��������� �ڽ� ������Ʈ�� ���� �̺�Ʈ�� �����մϴ�.
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
		// ����Ʈ ��忡�� ������ ��
		if (_editorMode == EditorMode::Edit)
		{
			// Play Event ..?

			// TODO : ������ ���Դϴ�. �׷��� �����Ͻðڽ��ϱ� ..?
			MessageBox_ message(TEXT("Play Mode"),
				TEXT("Is it okay to save \n and enter play mode? "),
				MessageBox_::MessageType::WARNING, MessageBox_::ButtonLayout::YES_NO, true);

			switch (message.GetUserChoice())
			{
				// ���� O
				case MessageBox_::UserChoice::YES: break;

				// ���� X
				case MessageBox_::UserChoice::NO: return;
			}

			// ������ ���� ���� �ø���������մϴ�. (����, ���̺갡 �� �Ǿ� ���� ���� �����ϱ� ..!)
			if (DUOLGameEngine::SceneManager::GetInstance()->GetIsCurrentSceneLoadedFromFile())
			{
				SaveLoadedFromFileScene();
			}
			else
			{
				SaveAs();
			}
		}
		// �ƴ� ���
		else
		{
			// �״�� ��ŸƮ �ϴ°ǰ� ?
		}

		// ���� ���� ���ؼ� ����Ű�� ���� ..!
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

			// ���� ������Ʈ �� �����ú��� ..
			_gameObjectUnselectedEvent.Invoke();

			// ���� �� ������ �ڵ����� �ε��ؼ� �����ɴϴ�. ���⼭ Scene Changed Event On..
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

		// �����͸� ���ݴϴ�.
		_editor->_isRunning = false;
	}

	void EditorEventManager::SaveLoadedFromFileScene()
	{
		DUOLGameEngine::SceneManager::GetInstance()->SaveCurrentSceneTo(DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetPath());
	}

	void EditorEventManager::SaveAs()
	{
		// ���� Ž���� �����.
		WriteFileDialog dialog(TEXT("Save Scene"));

		std::filesystem::path absolutePath = std::filesystem::absolute(TEXT("Asset/Scene/New Scene"));

		dialog.SetInitializeDirectory(absolutePath.c_str());
		dialog.DefineExtension(TEXT("DUOL Scene"), TEXT(".dscene"));
		dialog.ShowDialog();

		// �׸��� �̸�, �� �� ������ �� �ְ� �ϰ� ���� ..!
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
		// ����, ���� ������ ���� ���°� �ƴ϶�� �ƹ� �͵� ���� �ʴ´�.
		if (_editorMode != EditorMode::Edit)
			return;

		// ����, ���� ���� ���Ϸ� ���� �ҷ��� �Դٸ� ���� ������ ���� ���� ������ ��������.
		SaveScene();

		// Empty Scene �� �ε��մϴ�.
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

		// Yes ����
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

		// �׸��� �̸�, �� �� ������ �� �ְ� �ϰ� ���� ..!
		if (dialog.IsSucceeded())
		{
			// ������ �����Ѵٸ�.
			if (dialog.IsFileExisting())
			{
				// ���� ������Ʈ �� �����ú��� ..
				_gameObjectUnselectedEvent.Invoke();

				// ���� �� ������ �ڵ����� �ε��ؼ� �����ɴϴ�. ���⼭ Scene Changed Event On..
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

		// Yes ����
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

		// �׸��� �̸�, �� �� ������ �� �ְ� �ϰ� ���� ..!
		if (dialog.IsSucceeded())
		{
			// ������ �����Ѵٸ�.
			if (dialog.IsFileExisting())
			{
				// ���� ������Ʈ �� �����ú��� ..
				_gameObjectUnselectedEvent.Invoke();

				// ���� �� ������ �ڵ����� �ε��ؼ� �����ɴϴ�. ���⼭ Scene Changed Event On..
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
		// ����, ���� ������ ���� ���°� �ƴ϶�� �ƹ� �͵� ���� �ʴ´�.
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