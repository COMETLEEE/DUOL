#pragma once
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLCommon/Event/Event.h"

#include "DUOLEditor/UI/Panels/PanelWindow.h"

namespace DUOLEditor
{
	class TreeNode;

	class Hierarchy : public DUOLEditor::PanelWindow
	{
	public:
		Hierarchy(const DUOLCommon::tstring& title, bool opened, const DUOLEditor::PanelWindowSetting& panelWindowSetting);

		void Clear();

		void UnSelectGameObjectsWidgets();

		void SelectGameObjectByWidget();

		void AttachGameObjectToParent();

		void DetachGameObjectFromParent();

		void AddGameObjectByInstance();

	public:
		DUOLCommon::Event<void, DUOLGameEngine::GameObject*> gameObjectSelectedEvent;

		DUOLCommon::Event<void, DUOLGameEngine::GameObject*> gameObjectUnselectedEvent;

	private:
		DUOLEditor::TreeNode* _sceneRoot;

		std::unordered_map<DUOLGameEngine::GameObject*, DUOLEditor::TreeNode*> _gameObjectWidgetMap;
	};
}