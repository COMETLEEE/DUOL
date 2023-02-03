/**

    @file      HierarchyContextMenu.h
    @brief     Hierarchy Contextual (Hover + Right mouse click) Menu class.
    @details   ~
    @author    COMETLEE
    @date      3.02.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include "DUOLEditor/UI/AddOns/ContextMenu.h"

namespace DUOLGameEngine
{
	class GameObject;
}

namespace DUOLEditor
{
	class TreeNode;
	/**
	 * \brief 하이어라키에서 우클릭을 통해 나오게 하는 편의 기능 제공 컨텍스트 메뉴 위젯
	 */
	class HierarchyContextMenu : public DUOLEditor::ContextMenu
	{
	public:
		HierarchyContextMenu(DUOLGameEngine::GameObject* gameObject, DUOLEditor::TreeNode* treeNode);

		virtual ~HierarchyContextMenu();

		virtual void Execute() override;

	private:
		DUOLGameEngine::GameObject*		_targetGameObject;
		
		DUOLEditor::TreeNode*			_treeNode;
	};
}