#include "DUOLEditor/Modules/Hierarchy.h"

#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLEditor/UI/AddOns/DragAndDropSource.h"
#include "DUOLEditor/UI/AddOns/DragAndDropDest.h"

#include "DUOLEditor/UI/Widgets/Layout/TreeNode.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLEditor
{
	// ��� ����� Ʈ�� ������ �����մϴ�.
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
			// textSelectable���� ����� ���� ������Ʈ ������ ���� �θ� �ִٸ� ���� �θ𿡼� �ع�����ݴϴ�.
			if (element.second->GetParent() != nullptr)
				element.second->GetParent()->UnConsiderWidget(element.second);

			// ���� �༮�� textSelectable �������� ���ְ�
			rootNode->ConsiderWidget(element.second);

			// ������ ��, �� ������ �θ� ���赵 �ٽ� �������ݴϴ�.
			DUOLGameEngine::Transform* transform = element.first->GetTransform();

			// Root Object�� ������ݴϴ�.
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
		// _gameObjectWidgetMap�� ���� ��� GameObject�� ���� Widget�� ������ ����ֽ��ϴ�.
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
		
		// �θ� ���� ������Ʈ�� ���� (Ʈ����������) ��� Open �մϴ�.
		if (parent != nullptr)
		{
			ExpandTreeNode(parent);
		}
	}

	void Hierarchy::AttachGameObjectToParent(DUOLGameEngine::GameObject* gameObject)
	{
		if (_gameObjectWidgetMap.contains(gameObject))
		{
			// ������ ã�Ƽ� ���� ���� �θ� ���踦 �����ְ�
			auto gameObjectWidget = _gameObjectWidgetMap[gameObject];

			if (gameObjectWidget->HasParent())
			{
				gameObjectWidget->GetParent()->UnConsiderWidget(gameObjectWidget);
			}

			DUOLGameEngine::Transform* parentTransform = gameObject->GetTransform()->GetParent();

			// ���� ���� ������Ʈ�� �θ� �����ϴ� ������ ã�Ƽ� �������ݴϴ�.
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
		// ���� ������Ʈ�� ���� �ϳ��� ������ ���� ������Ʈ ���� ����Ʈ�� �־��ݴϴ�.
		auto textSelectable = _gameObjectsWidgetsList->AddWidget<DUOLEditor::TreeNode>(gameObject->GetName(), true);

		textSelectable->AddAddOn<DUOLEditor::DragAndDropSource<std::pair<DUOLGameEngine::GameObject*, DUOLEditor::TreeNode*>>>
			(TEXT("GameObject"), TEXT("Attach to .."), std::make_pair(gameObject, textSelectable));

		textSelectable->AddAddOn<DUOLEditor::DragAndDropDest<std::pair<DUOLGameEngine::GameObject*, DUOLEditor::TreeNode*>>>
			(TEXT("GameObject"))->_dataReceivedEvent += [gameObject, textSelectable](std::pair<DUOLGameEngine::GameObject*, DUOLEditor::TreeNode*> element)
		{
			// textSelectable���� ����� ���� ������Ʈ ������ ���� �θ� �ִٸ� ���� �θ𿡼� �ع�����ݴϴ�.
			if (element.second->GetParent() != nullptr)
				element.second->GetParent()->UnConsiderWidget(element.second);

			// ���� �༮�� textSelectable �������� ���ְ�
			textSelectable->ConsiderWidget(element.second);

			// ������ ��, �� ������ �θ� ���赵 �ٽ� �������ݴϴ�.
			DUOLGameEngine::Transform* transform = element.first->GetTransform();

			transform->SetParent(gameObject->GetTransform());
		};

		DUOLGameEngine::GameObject* targetPtr = gameObject;
		textSelectable->RegisterGatherer([targetPtr] { return targetPtr->GetName(); });

		_gameObjectWidgetMap.insert({ targetPtr, textSelectable });

		// GameObject�� Tree Node�� �ش� textSelectable �� Parent�� �����մϴ�.
		DUOLGameEngine::Transform* currTransform = gameObject->GetTransform();

		// �ڽ� ������Ʈ�� �� �̻� ���ٸ� Leaf Node
		currTransform->GetChildren().size() == 0 ? textSelectable->_isLeaf = true : textSelectable->_isLeaf = false;

		DUOLGameEngine::Transform* parentTransform = currTransform->GetParent();

		// �θ� �ִٸ� �θ� ���� Ʈ�� ��带 �θ�� �������ݴϴ�. �׸��� ��� Group List���� �������ݴϴ�.
		if (parentTransform != nullptr && _gameObjectWidgetMap.contains(parentTransform->GetGameObject()))
		{
			auto& widgetOfParent = _gameObjectWidgetMap[parentTransform->GetGameObject()];

			// ���� ������ �θ� ���踦 �����ְ�
			_gameObjectsWidgetsList->UnConsiderWidget(textSelectable);

			// ���� �θ� ������ �������ݴϴ�
			widgetOfParent->ConsiderWidget(textSelectable);
		}

		auto childs = gameObject->GetTransform()->GetChildGameObjects();

		for (auto& child : childs)
		{
			AddGameObjectByInstance(child);
		}

		textSelectable->_clickedEvent += 
			std::bind(&DUOLEditor::EditorEventManager::SelectGameObject, DUOLEditor::EditorEventManager::GetInstance(), gameObject);

		// TODO - �� �信�� Focus, �̵� ���� ..?
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