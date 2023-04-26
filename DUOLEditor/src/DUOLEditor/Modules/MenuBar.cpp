#include "DUOLEditor/Modules/MenuBar.h"

#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLEditor/UI/Widgets/Menu/MenuItem.h"
#include "DUOLEditor/UI/Widgets/Menu/MenuList.h"
#include "DUOLEditor/UI/Widgets/Display/Separator.h"

namespace DUOLEditor
{
	MenuBar::MenuBar()
	{
		// Scene Save- Load Menu를 만듭니다.
		CreateSceneMenu();

		AddReservedWidgets();
	}

	void MenuBar::CreateSceneMenu()
	{
		auto fileMenu = AddWidget<MenuList>(TEXT("File"));

		fileMenu->AddWidget<MenuItem>(TEXT("New Scene"), TEXT("CTRL + N"))->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::LoadEmptyScene, DUOLEditor::EditorEventManager::GetInstance().get());

		fileMenu->AddWidget<Separator>();
		fileMenu->AddWidget<MenuItem>(TEXT("Save Scene"), TEXT("CTRL + S"))->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::SaveScene, DUOLEditor::EditorEventManager::GetInstance().get());
		fileMenu->AddWidget<MenuItem>(TEXT("Save Scene As..."), TEXT("CTRL + SHIFT + S"))->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::SaveAs, DUOLEditor::EditorEventManager::GetInstance().get());
		fileMenu->AddWidget<Separator>();

		fileMenu->AddWidget<MenuItem>(TEXT("Open Scene"), TEXT("CTRL + O"))->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::OpenScene, DUOLEditor::EditorEventManager::GetInstance().get());

		// Unity Scene을 만들어서 Import 하는 역할을 수행합니다.
		fileMenu->AddWidget<MenuItem>(TEXT("Open Unity Scene"), TEXT("CTRL + U"))->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::OpenUnityScene, DUOLEditor::EditorEventManager::GetInstance().get());

		fileMenu->AddWidget<Separator>();

		fileMenu->AddWidget<MenuItem>(TEXT("Exit"), TEXT("ALT + F4"))->_clickedEvent += [] { DUOLEditor::EditorEventManager::GetInstance()->ExitEditor(); };

		auto editorMenu = AddWidget<MenuList>(TEXT("Editor"));

		editorMenu->AddWidget<MenuItem>(TEXT("GraphicsSetting"))->_clickedEvent += [] {DUOLEditor::EditorEventManager::GetInstance()->OpenGraphicsSettingUI(); };
	}
}