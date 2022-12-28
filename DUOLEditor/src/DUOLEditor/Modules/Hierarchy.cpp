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

		if (widget->GetParent() != nullptr)
		{
			ExpandTreeNode(static_cast<DUOLEditor::TreeNode*>(widget->GetParent()));
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
			// ���� �θ� �ִٸ� ���� �θ𿡼� �ع�����ݴϴ�.
			if (element.second->GetParent() != nullptr)
				element.second->GetParent()->UnConsiderWidget(element.second);

			// ���� �༮�� �ڽ� �������� ���ְ�
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

		// �ڽ� ������Ʈ�� ���ٸ� Leaf
		if (currTransform->GetChildren().size() == 0)
			textSelectable->_isLeaf = true;
		else
			textSelectable->_isLeaf = false;

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

		// TODO - All Editor Action���� �����ؾ��� �� �����ϴ� .. => ���� ������ �ش� �׼��� �����ؾ��� �ʿ䰡 �ֽ��ϴ� ..!
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