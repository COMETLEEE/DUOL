#include "DUOLEditor/Modules/HierarchyContextMenu.h"

#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLGameEngine/ECS/ObjectBase.h"

#include "DUOLEditor/UI/Widgets/Menu/MenuItem.h"
#include "DUOLEditor/UI/Widgets/Menu/MenuList.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

namespace DUOLEditor
{
	HierarchyContextMenu::HierarchyContextMenu(DUOLGameEngine::GameObject* gameObject, DUOLEditor::TreeNode* treeNode) :
		_targetGameObject(gameObject)
		, _treeNode(treeNode)
	{
		if (_targetGameObject != nullptr)
		{
			// Focus
			auto focusButton = AddWidget<DUOLEditor::MenuItem>(TEXT("Focus"));

			focusButton->_clickedEvent += [this]()
			{
				// TODO : Scene View의 Camera를 보간하면서 슥슥 가보자. 
			};

			// Delete
			auto deleteButton = AddWidget<DUOLEditor::MenuItem>(TEXT("Delete"));

			deleteButton->_clickedEvent += [this]()
			{
				DUOLGameEngine::ObjectBase::Destroy(reinterpret_cast<DUOLGameEngine::ObjectBase*>(_targetGameObject));

				DUOLEditor::EditorEventManager::GetInstance()->DeleteGameObject(_targetGameObject);
			};
		}

#pragma region CREATE_GAMEOBJECT
		auto createGameObject = AddWidget<DUOLEditor::MenuList>(TEXT("Create GameObject ..."));

		auto createEmpty = createGameObject->AddWidget<DUOLEditor::MenuItem>(TEXT("Empty"));

		createEmpty->_clickedEvent += []()
		{
			DUOLGameEngine::Scene* currScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

			if (currScene != nullptr)
			{
				DUOLGameEngine::GameObject* createdGameObject = currScene->CreateEmpty();

				// Hierarchy 창에 추가해줘야하는거 아니야 ?
				DUOLEditor::EditorEventManager::GetInstance()->CreateGameObject(createdGameObject);
			}
		};
#pragma endregion
	}

	HierarchyContextMenu::~HierarchyContextMenu()
	{
		
	}

	void HierarchyContextMenu::Execute()
	{
		DUOLEditor::ContextMenu::Execute();
	}
}
