#include "DUOLEditor/Modules/MenuBar.h"

#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLEditor/UI/Widgets/Menu/MenuItem.h"
#include "DUOLEditor/UI/Widgets/Menu/MenuList.h"

namespace DUOLEditor
{
	MenuBar::MenuBar()
	{
		// Scene Save- Load Menu¸¦ ¸¸µì´Ï´Ù.
		CreateSceneMenu();

		AddReservedWidgets();
	}

	void MenuBar::CreateSceneMenu()
	{
		auto fileMenu = AddWidget<MenuList>(TEXT("File"));

		fileMenu->AddWidget<MenuItem>(TEXT("New Scene"), TEXT("CTRL + N"))->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::LoadEmptyScene, DUOLEditor::EditorEventManager::GetInstance().get());
		fileMenu->AddWidget<MenuItem>(TEXT("Save Scene"), TEXT("CTRL + S"))->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::SaveScene, DUOLEditor::EditorEventManager::GetInstance().get());
		fileMenu->AddWidget<MenuItem>(TEXT("Save Scene As..."), TEXT("CTRL + SHIFT + S"))->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::SaveAs, DUOLEditor::EditorEventManager::GetInstance().get());
		fileMenu->AddWidget<MenuItem>(TEXT("Open Scene"), TEXT("CTRL + O"))->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::OpenScene, DUOLEditor::EditorEventManager::GetInstance().get());
		fileMenu->AddWidget<MenuItem>(TEXT("Exit"), TEXT("ALT + F4"))->_clickedEvent += [] { DUOLEditor::EditorEventManager::GetInstance()->ExitEditor(); };
	}
}