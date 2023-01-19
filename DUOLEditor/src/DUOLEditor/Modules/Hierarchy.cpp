#include "DUOLEditor/Modules/Hierarchy.h"

#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLEditor/UI/AddOns/DragAndDropSource.h"
#include "DUOLEditor/UI/AddOns/DragAndDropDest.h"

#include "DUOLEditor/UI/Widgets/Layout/TreeNode.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLEditor
{
	// 모든 상단의 트리 노드들을 오픈합니다.
	void ExpandTreeNode(DUOLEditor::TreeNode* treeNodeToExpand)
	{
		treeNodeToExpand->Open();

		DUOLEditor::TreeNode* treeNode = dynamic_cast<DUOLEditor::TreeNode*>(treeNodeToExpand->GetParent());
		
		if (treeNode != nullptr)
		{
			ExpandTreeNode(treeNode);
		}
	}

	Hierarchy::Hierarchy(const DUOLCommon::tstring& title, bool isOpened,
		const DUOLEditor::PanelWindowSetting& panelWindowSetting) :
		DUOLEditor::PanelWindow(title, isOpened, panelWindowSetting)
	{
		_gameObjectsWidgetsList = AddWidget<DUOLEditor::TreeNode>(TEXT("SampleScene"), true);

		_gameObjectsWidgetsList->Open();

		DUOLEditor::TreeNode* rootNode = _gameObjectsWidgetsList;

		_gameObjectsWidgetsList->AddAddOn<DUOLEditor::DragAndDropDest<std::pair<DUOLGameEngine::GameObject*, DUOLEditor::TreeNode*>>>
			(TEXT("GameObject"))->_dataReceivedEvent += [rootNode](std::pair<DUOLGameEngine::GameObject*, DUOLEditor::TreeNode*> element)
		{
			// textSelectable에게 드랍된 게임 오브젝트 위젯의 기존 부모가 있다면 기존 부모에서 해방시켜줍니다.
			if (element.second->GetParent() != nullptr)
				element.second->GetParent()->UnConsiderWidget(element.second);

			// 들어온 녀석을 textSelectable 위젯으로 해주고
			rootNode->ConsiderWidget(element.second);

			// 씬에서 즉, 인 게임의 부모 관계도 다시 설정해줍니다.
			DUOLGameEngine::Transform* transform = element.first->GetTransform();

			// Root Object로 만들어줍니다.
			transform->SetParent(nullptr);
		};

		// Game Object Selected Event
		DUOLEditor::EditorEventManager::GetInstance()->GetGameObjectSelectedEvent() += 
			std::bind(&Hierarchy::SelectGameObjectByInstance, this, std::placeholders::_1);

		// Game Object Unselected Event
		DUOLEditor::EditorEventManager::GetInstance()->GetGameObjectUnselectedEvent() +=
			std::bind(&Hierarchy::UnselectAllGameObjects, this);
	}

	void Hierarchy::Clear()
	{
		_gameObjectsWidgetsList->RemoveAllWidgets();

		_gameObjectWidgetMap.clear();
	}

	void Hierarchy::UnselectAllGameObjects()
	{
		// _gameObjectWidgetMap은 현재 모든 GameObject에 대한 Widget의 참조가 들어있습니다.
		for (auto& widget : _gameObjectWidgetMap)
			widget.second->_isSelected = false;
	}

	void Hierarchy::SelectGameObjectByInstance(DUOLGameEngine::GameObject* gameObject)
	{
		if (_gameObjectWidgetMap.contains(gameObject))
		{
			SelectGameObjectByWidget(_gameObjectWidgetMap[gameObject]);
		}
	}

	void Hierarchy::SelectGameObjectByWidget(DUOLEditor::TreeNode* widget)
	{
		UnselectAllGameObjects();

		widget->_isSelected = true;

		DUOLEditor::TreeNode* parent = dynamic_cast<TreeNode*>(widget->GetParent());
		
		// 부모 게임 오브젝트의 위젯 (트리노드까지만) 모두 Open 합니다.
		if (parent != nullptr)
		{
			ExpandTreeNode(parent);
		}
	}

	void Hierarchy::AttachGameObjectToParent(DUOLGameEngine::GameObject* gameObject)
	{
		if (_gameObjectWidgetMap.contains(gameObject))
		{
			// 위젯을 찾아서 기존 위젯 부모 관계를 끊어주고
			auto gameObjectWidget = _gameObjectWidgetMap[gameObject];

			if (gameObjectWidget->HasParent())
			{
				gameObjectWidget->GetParent()->UnConsiderWidget(gameObjectWidget);
			}

			DUOLGameEngine::Transform* parentTransform = gameObject->GetTransform()->GetParent();

			// 현재 게임 오브젝트의 부모를 참조하는 위젯을 찾아서 연결해줍니다.
			if ((parentTransform != nullptr) && (_gameObjectWidgetMap.contains(parentTransform->GetGameObject())))
			{
				auto parentWidget = _gameObjectWidgetMap[parentTransform->GetGameObject()];

				parentWidget->_isLeaf = false;

				parentWidget->ConsiderWidget(gameObjectWidget);
			}
		}
	}

	void Hierarchy::DetachGameObjectFromParent(DUOLGameEngine::GameObject* gameObject)
	{
		if (_gameObjectWidgetMap.contains(gameObject))
		{
			auto gameObjectWidget = _gameObjectWidgetMap[gameObject];

			// auto parentObjectWidget 
		}
	}

	void Hierarchy::AddGameObjectByInstance(DUOLGameEngine::GameObject* gameObject)
	{
		// 게임 오브젝트에 대한 하나의 위젯을 게임 오브젝트 위젯 리스트에 넣어줍니다.
		auto textSelectable = _gameObjectsWidgetsList->AddWidget<DUOLEditor::TreeNode>(gameObject->GetName(), true);

		textSelectable->AddAddOn<DUOLEditor::DragAndDropSource<std::pair<DUOLGameEngine::GameObject*, DUOLEditor::TreeNode*>>>
			(TEXT("GameObject"), TEXT("Attach to .."), std::make_pair(gameObject, textSelectable));

		textSelectable->AddAddOn<DUOLEditor::DragAndDropDest<std::pair<DUOLGameEngine::GameObject*, DUOLEditor::TreeNode*>>>
			(TEXT("GameObject"))->_dataReceivedEvent += [gameObject, textSelectable](std::pair<DUOLGameEngine::GameObject*, DUOLEditor::TreeNode*> element)
		{
			// textSelectable에게 드랍된 게임 오브젝트 위젯의 기존 부모가 있다면 기존 부모에서 해방시켜줍니다.
			if (element.second->GetParent() != nullptr)
				element.second->GetParent()->UnConsiderWidget(element.second);

			// 들어온 녀석을 textSelectable 위젯으로 해주고
			textSelectable->ConsiderWidget(element.second);

			// 씬에서 즉, 인 게임의 부모 관계도 다시 설정해줍니다.
			DUOLGameEngine::Transform* transform = element.first->GetTransform();

			transform->SetParent(gameObject->GetTransform());
		};

		DUOLGameEngine::GameObject* targetPtr = gameObject;
		textSelectable->RegisterGatherer([targetPtr] { return targetPtr->GetName(); });

		_gameObjectWidgetMap.insert({ targetPtr, textSelectable });

		// GameObject의 Tree Node를 해당 textSelectable 의 Parent로 설정합니다.
		DUOLGameEngine::Transform* currTransform = gameObject->GetTransform();

		// 자식 오브젝트가 더 이상 없다면 Leaf Node
		currTransform->GetChildren().size() == 0 ? textSelectable->_isLeaf = true : textSelectable->_isLeaf = false;

		DUOLGameEngine::Transform* parentTransform = currTransform->GetParent();

		// 부모가 있다면 부모 위젯 트리 노드를 부모로 설정해줍니다. 그리고 기반 Group List에서 제외해줍니다.
		if (parentTransform != nullptr && _gameObjectWidgetMap.contains(parentTransform->GetGameObject()))
		{
			auto& widgetOfParent = _gameObjectWidgetMap[parentTransform->GetGameObject()];

			// 기존 위젯의 부모 관계를 끊어주고
			_gameObjectsWidgetsList->UnConsiderWidget(textSelectable);

			// 실제 부모 위젯과 연결해줍니다
			widgetOfParent->ConsiderWidget(textSelectable);
		}

		auto childs = gameObject->GetTransform()->GetChildGameObjects();

		for (auto& child : childs)
		{
			AddGameObjectByInstance(child);
		}

		textSelectable->_clickedEvent += 
			std::bind(&DUOLEditor::EditorEventManager::SelectGameObject, DUOLEditor::EditorEventManager::GetInstance(), gameObject);

		// TODO - 씬 뷰에서 Focus, 이동 정도 ..?
		// textSelectable->_doubleClickedEvent
	}

	void Hierarchy::RemoveGameObjectByInstance(DUOLGameEngine::GameObject* gameObject)
	{
		if (_gameObjectWidgetMap.contains(gameObject))
		{
			auto treenode = _gameObjectWidgetMap.at(gameObject);

			treenode->Destroy();

			_gameObjectWidgetMap.erase(gameObject);
		}
	}

	void Hierarchy::SetCurrentScene(DUOLGameEngine::Scene* scene)
	{
		if (scene == nullptr)
			return;

		Clear();

		_currentScene = scene;
		
		_gameObjectsWidgetsList->_name = _currentScene->GetName();

		AddRootGameObjects();
	}

	void Hierarchy::AddRootGameObjects()
	{
		if (_currentScene == nullptr)
			return;

		auto& rootObjects = _currentScene->GetRootObjects();

		for (auto rootObject : rootObjects)
			AddGameObjectByInstance(rootObject);
	}

	DUOLGameEngine::Scene* Hierarchy::GetCurrentScene()
	{
		return _currentScene;
	}
}