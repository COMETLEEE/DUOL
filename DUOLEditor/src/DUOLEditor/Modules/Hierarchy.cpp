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

	void ExpandTreeNodeAndCollapse(DUOLEditor::TreeNode* treeNodeToExpand)
	{
		// 기존에 열려 있지 않은 녀석만 열어줍니다 !
		if (!treeNodeToExpand->GetIsOpened())
		{
			treeNodeToExpand->Open();

			// 검색이 끝나면 다시 닫아줘야 합니다.
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

			// 검색 내용이 있다면
			if (!textLower.empty())
			{
				for (auto [gameObject, treeNode] : _gameObjectWidgetMap)
				{
					// 일단 모든 트리 노드들 다 꺼주자.
					treeNode->SetIsEnable(false);

					treeNode->SetIsSearched(false);

					auto treeNodeName = treeNode->_name;

					std::transform(treeNodeName.begin(), treeNodeName.end(), treeNodeName.begin(), ::tolower);

					// 만약 검색 문자열을 포함하고 있으면 검색된 목록에 넣습니다.
					if (treeNodeName.find(textLower) != DUOLCommon::tstring::npos)
					{
						founds.push_back(treeNode);
					}
				}

				// 검색된 노드들에 대한 로직
				for (auto node : founds)
				{
					// 전부 켜주고
					node->SetIsEnable(true);

					// 검색도 되었으니 눈에 띄도록 High lighting
					node->SetIsSearched(true);

					DUOLEditor::TreeNode* parentNode = dynamic_cast<DUOLEditor::TreeNode*>(node->GetParent());

					if (parentNode != nullptr)
						ExpandTreeNodeAndCollapse(parentNode);
				}
			}
			// 텍스트 내용이 없다면, 검색 이전에 닫혀 있던 트리 노드들을 다시 닫아줍니다.
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

		_gameObjectsWidgetsList->AddAddOn<DUOLEditor::HierarchyContextMenu>(nullptr, _gameObjectsWidgetsList);

		// 게임 오브젝트가 씬 상에 추가되면 하이어라키 위에 위젯도 추가해주어야 합니다.
		DUOLEditor::EditorEventManager::GetInstance()->GetCreateGameObjectEvent() +=
			std::bind(&Hierarchy::AddGameObjectByInstance, this, std::placeholders::_1, false);

		// 게임 오브젝트가 씬 상에서 제거되면 하이어라키에서 위젯도 삭제해야합니다.
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

	void Hierarchy::AddGameObjectByInstance(DUOLGameEngine::GameObject* gameObject, bool recursively)
	{
		// 게임 오브젝트에 대한 하나의 위젯을 게임 오브젝트 위젯 리스트에 넣어줍니다.
		auto textSelectable = _gameObjectsWidgetsList->AddWidget<DUOLEditor::TreeNode>(gameObject->GetName(), true);

		// ContextMenu
		textSelectable->AddAddOn<DUOLEditor::HierarchyContextMenu>(gameObject, textSelectable);

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

		// 게임 오브젝트가 삭제될 때 액션
		targetPtr->_destroyEventHandlers += [this, textSelectable]()
		{
			_gameObjectsWidgetsList->RemoveWidget(textSelectable);
		};

		// TODO : 여기 있어도 되는거 맞죠 ..?
		targetPtr->_destroyEventHandlers += [this]()
		{
			DUOLEditor::EditorEventManager::GetInstance()->UnselectGameObject();
		};

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

		// 재귀적 호출이라면 넣어줍시다.
		if (recursively)
		{
			auto childs = std::move(gameObject->GetTransform()->GetChildGameObjects());

			for (auto child : childs)
				AddGameObjectByInstance(child, recursively);
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
			AddGameObjectByInstance(rootObject, true);
	}

	DUOLGameEngine::Scene* Hierarchy::GetCurrentScene()
	{
		return _currentScene;
	}
}