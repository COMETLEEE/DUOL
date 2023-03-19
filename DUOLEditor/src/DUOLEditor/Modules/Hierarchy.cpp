#include "DUOLEditor/Modules/Hierarchy.h"

#include <filesystem>

#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLEditor/Modules/HierarchyContextMenu.h"
#include "DUOLEditor/UI/AddOns/DragAndDropSource.h"
#include "DUOLEditor/UI/AddOns/DragAndDropDest.h"
#include "DUOLEditor/UI/Widgets/InputFields/InputText.h"

#include "DUOLEditor/UI/Widgets/Layout/TreeNode.h"
#include "DUOLEditor/UI/Widgets/Layout/SameLine.h"
#include "DUOLEditor/UI/Widgets/Layout/Columns.h"

#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

namespace DUOLEditor
{
	std::vector<DUOLEditor::TreeNode*> nodesToCollapse;
	std::vector<DUOLEditor::TreeNode*> founds;

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

	void ExpandTreeNodeAndCollapse(DUOLEditor::TreeNode* treeNodeToExpand)
	{
		// ������ ���� ���� ���� �༮�� �����ݴϴ� !
		if (!treeNodeToExpand->GetIsOpened())
		{
			treeNodeToExpand->Open();

			// �˻��� ������ �ٽ� �ݾ���� �մϴ�.
			nodesToCollapse.push_back(treeNodeToExpand);
		}
		
		treeNodeToExpand->SetIsEnable(true);

		DUOLEditor::TreeNode* treeNode = dynamic_cast<DUOLEditor::TreeNode*>(treeNodeToExpand->GetParent());

		if (treeNode != nullptr)
		{
			ExpandTreeNodeAndCollapse(treeNode);
		}
	}

	Hierarchy::Hierarchy(const DUOLCommon::tstring& title, bool isOpened,
		const DUOLEditor::PanelWindowSetting& panelWindowSetting) :
		DUOLEditor::PanelWindow(title, isOpened, panelWindowSetting)
	{
#pragma region SEARCH_GAMEOBJECT
		DUOLEditor::ImGuiHelper::DrawTitle(this, TEXT("Search Object"));

		AddWidget<DUOLEditor::SameLine>();

		auto search = AddWidget<DUOLEditor::InputText>();

		search->_textChangedEvent += [this](const DUOLCommon::tstring& text)
		{
			founds.clear();

			auto textLower = text;

			std::transform(textLower.begin(), textLower.end(), textLower.begin(), ::tolower);

			// �˻� ������ �ִٸ�
			if (!textLower.empty())
			{
				for (auto [gameObject, treeNode] : _gameObjectWidgetMap)
				{
					// �ϴ� ��� Ʈ�� ���� �� ������.
					treeNode->SetIsEnable(false);

					treeNode->SetIsSearched(false);

					auto treeNodeName = treeNode->_name;

					std::transform(treeNodeName.begin(), treeNodeName.end(), treeNodeName.begin(), ::tolower);

					// ���� �˻� ���ڿ��� �����ϰ� ������ �˻��� ��Ͽ� �ֽ��ϴ�.
					if (treeNodeName.find(textLower) != DUOLCommon::tstring::npos)
					{
						founds.push_back(treeNode);
					}
				}

				// �˻��� ���鿡 ���� ����
				for (auto node : founds)
				{
					// ���� ���ְ�
					node->SetIsEnable(true);

					// �˻��� �Ǿ����� ���� �絵�� High lighting
					node->SetIsSearched(true);

					DUOLEditor::TreeNode* parentNode = dynamic_cast<DUOLEditor::TreeNode*>(node->GetParent());

					if (parentNode != nullptr)
						ExpandTreeNodeAndCollapse(parentNode);
				}
			}
			// �ؽ�Ʈ ������ ���ٸ�, �˻� ������ ���� �ִ� Ʈ�� ������ �ٽ� �ݾ��ݴϴ�.
			else
			{
				for (auto [gameObject, treeNode] : _gameObjectWidgetMap)
				{
					treeNode->SetIsEnable(true);

					treeNode->SetIsSearched(false);
				}

				for (auto node : nodesToCollapse)
					node->Close();

				nodesToCollapse.clear();
			}
		};
#pragma endregion

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

		_gameObjectsWidgetsList->AddAddOn<DUOLEditor::HierarchyContextMenu>(nullptr, _gameObjectsWidgetsList);

		// ���� ������Ʈ�� �� �� �߰��Ǹ� ���̾��Ű ���� ������ �߰����־�� �մϴ�.
		DUOLEditor::EditorEventManager::GetInstance()->GetCreateGameObjectEvent() +=
			std::bind(&Hierarchy::AddGameObjectByInstance, this, std::placeholders::_1, false);

		// ���� ������Ʈ�� �� �󿡼� ���ŵǸ� ���̾��Ű���� ������ �����ؾ��մϴ�.
		DUOLEditor::EditorEventManager::GetInstance()->GetDeleteGameObjectEvent() +=
			std::bind(&Hierarchy::RemoveGameObjectByInstance, this, std::placeholders::_1);

		// Game Object Selected Event
		DUOLEditor::EditorEventManager::GetInstance()->GetGameObjectSelectedEvent() += 
			std::bind(&Hierarchy::SelectGameObjectByInstance, this, std::placeholders::_1);

		// Game Object Unselected Event
		DUOLEditor::EditorEventManager::GetInstance()->GetGameObjectUnselectedEvent() +=
			std::bind(&Hierarchy::UnselectAllGameObjects, this);

		// Scene Changed Event
		DUOLEditor::EditorEventManager::GetInstance()->GetSceneChangedEvent() += 
			std::bind(&Hierarchy::SetCurrentScene, this, std::placeholders::_1);
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

	void Hierarchy::AddGameObjectByInstance(DUOLGameEngine::GameObject* gameObject, bool recursively)
	{
		// ���� ������Ʈ�� ���� �ϳ��� ������ ���� ������Ʈ ���� ����Ʈ�� �־��ݴϴ�.
		auto textSelectable = _gameObjectsWidgetsList->AddWidget<DUOLEditor::TreeNode>(gameObject->GetName(), true);

		// ContextMenu
		textSelectable->AddAddOn<DUOLEditor::HierarchyContextMenu>(gameObject, textSelectable);

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

		// ���� ������Ʈ�� ������ �� �׼�
		targetPtr->_destroyEventHandlers += [this, textSelectable]()
		{
			_gameObjectsWidgetsList->RemoveWidget(textSelectable);
		};

		// TODO : ���� �־ �Ǵ°� ���� ..?
		targetPtr->_destroyEventHandlers += [this]()
		{
			DUOLEditor::EditorEventManager::GetInstance()->UnselectGameObject();
		};

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

		// ����� ȣ���̶�� �־��ݽô�.
		if (recursively)
		{
			auto childs = std::move(gameObject->GetTransform()->GetChildGameObjects());

			for (auto child : childs)
				AddGameObjectByInstance(child, recursively);
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
			AddGameObjectByInstance(rootObject, true);
	}

	DUOLGameEngine::Scene* Hierarchy::GetCurrentScene()
	{
		return _currentScene;
	}
}