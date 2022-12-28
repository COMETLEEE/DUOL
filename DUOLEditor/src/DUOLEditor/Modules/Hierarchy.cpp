#include "DUOLEditor/Modules/Hierarchy.h"

#include "DUOLEditor/UI/AddOns/DragAndDropSource.h"
#include "DUOLEditor/UI/AddOns/DragAndDropDest.h"

#include "DUOLEditor/UI/Widgets/Layout/TreeNode.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLEditor
{
	void ExpandTreeNode(DUOLEditor::TreeNode* treeNodeToExpand)
	{
		treeNodeToExpand->Open();

		if (treeNodeToExpand->GetParent() != nullptr)
		{
			ExpandTreeNode(static_cast<DUOLEditor::TreeNode*>(treeNodeToExpand->GetParent()));
		}
	}

	Hierarchy::Hierarchy(const DUOLCommon::tstring& title, bool isOpened,
		const DUOLEditor::PanelWindowSetting& panelWindowSetting) :
		DUOLEditor::PanelWindow(title, isOpened, panelWindowSetting)
	{
		_gameObjectsWidgetsList = AddWidget<DUOLEditor::Container>();
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

		if (widget->GetParent() != nullptr)
		{
			ExpandTreeNode(static_cast<DUOLEditor::TreeNode*>(widget->GetParent()));
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
			// 기존 부모가 있다면 기존 부모에서 해방시켜줍니다.
			if (element.second->GetParent() != nullptr)
				element.second->GetParent()->UnConsiderWidget(element.second);

			// 들어온 녀석을 자식 위젯으로 해주고
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

		// 자식 오브젝트가 없다면 Leaf
		if (currTransform->GetChildren().size() == 0)
			textSelectable->_isLeaf = true;
		else
			textSelectable->_isLeaf = false;

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

		// TODO - All Editor Action으로 관리해야할 것 같습니다 .. => 여러 곳에서 해당 액션을 참조해야할 필요가 있습니다 ..!
		// textSelectable._clickedEvent
		// textSelectable._doubleClickedEvent
	}

	void Hierarchy::DeleteGameObjectByInstance(DUOLGameEngine::GameObject* gameObject)
	{

	}

	void Hierarchy::SetCurrentScene(DUOLGameEngine::Scene* scene)
	{
		Clear();

		_currentScene = scene;

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