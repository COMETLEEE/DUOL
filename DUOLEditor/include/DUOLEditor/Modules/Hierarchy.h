#pragma once
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLCommon/Event/Event.h"

#include "DUOLEditor/UI/Panels/PanelWindow.h"
#include "DUOLEditor/UI/Widgets/Layout/Container.h"

namespace DUOLEditor
{
	class TreeNode;
}

namespace DUOLEditor
{
	/**
	 * \brief 하이어라키 창을 나타내는 UI Object
	 */
	class Hierarchy : public DUOLEditor::PanelWindow
	{
	public:
		Hierarchy(const DUOLCommon::tstring& title, bool isOpened, 
			const DUOLEditor::PanelWindowSetting& panelWindowSetting);

		/**
		 * \brief Clear
		 */
		void Clear();

		/**
		 * \brief Unselect every widgets.
		 */
		void UnselectAllGameObjects();

		/**
		 * \brief Select the widget corresponding to the given game object.
		 * \param gameObject GameObject to select.
		 */
		void SelectGameObjectByInstance(DUOLGameEngine::GameObject* gameObject);

		/**
		 * \brief Select the game object in panel.
		 * \param widget 
		 */
		void SelectGameObjectByWidget(DUOLEditor::TreeNode* widget);

		/**
		 * \brief Attach the given game object linked widget to its parent game object's widget.
		 * \param gameObject 
		 */
		void AttachGameObjectToParent(DUOLGameEngine::GameObject* gameObject);

		/**
		 * \brief Detach the given game object linked widget to its parent game object's widget.
		 * \param gameObject 
		 */
		void DetachGameObjectFromParent(DUOLGameEngine::GameObject* gameObject);

		/**
		 * \brief Add widget of given game object.
		 * \param gameObject GameObject to remove.
		 */
		void AddGameObjectByInstance(DUOLGameEngine::GameObject* gameObject);

		/**
		 * \brief Remove widget of given game object.
		 * \param gameObject GameObject to delete.
		 */
		void RemoveGameObjectByInstance(DUOLGameEngine::GameObject* gameObject);

		/**
		 * \brief Setting current scene and re-init hierarchy view.
		 * \param scene current scene.
		 */
		void SetCurrentScene(DUOLGameEngine::Scene* scene);

		/**
		 * \brief Create widgets of root game objects.
		 */
		void AddRootGameObjects();

		/**
		 * \brief Get current scene instance of hierarchy view.
		 * \return current scene of hierarchy view.
		 */
		DUOLGameEngine::Scene* GetCurrentScene();

	public:
		DUOLCommon::Event<void, DUOLGameEngine::GameObject*> gameObjectSelectedEvent;

		DUOLCommon::Event<void, DUOLGameEngine::GameObject*> gameObjectUnselectedEvent;

	private:
		DUOLGameEngine::Scene* _currentScene;

		DUOLEditor::TreeNode* _gameObjectsWidgetsList;

		std::unordered_map<DUOLGameEngine::GameObject*, DUOLEditor::TreeNode*> _gameObjectWidgetMap;
	};
}