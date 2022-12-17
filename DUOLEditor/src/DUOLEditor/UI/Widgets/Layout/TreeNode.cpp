#include "DUOLEditor/UI/Widgets/Layout/TreeNode.h"

namespace DUOLEditor
{
	TreeNode::TreeNode(const DUOLCommon::tstring& name, bool isEnableArrowClickToOpen) :
		DataWidgetBase<DUOLCommon::tstring>(&_name)
		, _name(name)
		, _isEnableArrowClickToOpen(isEnableArrowClickToOpen)
	{
		_autoExecuteAddOns = false;
	}

	void TreeNode::Open()
	{
		_shouldOpen = true;

		_shouldClose = false;
	}

	void TreeNode::Close()
	{
		_shouldClose = true;

		_shouldOpen = false;
	}

	bool TreeNode::GetIsOpened()
	{
		return _isOpened;
	}

	void TreeNode::Draw_Impl()
	{
		bool prevOpened = _isOpened;

		if (_shouldOpen)
		{
			ImGui::SetNextItemOpen(true);

			_shouldOpen = false;
		}
		else if (_shouldClose)
		{
			ImGui::SetNextItemOpen(false);

			_shouldClose = false;
		}

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		if (_isEnableArrowClickToOpen) flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		if (_isSelected) flags |= ImGuiTreeNodeFlags_Selected;
		if (_isLeaf) flags |= ImGuiTreeNodeFlags_Leaf;

		bool opened = ImGui::TreeNodeEx(DUOLCommon::StringHelper::ToString(_name + _tstringID).c_str(), flags);

		if (ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
		{
			_clickedEvent.Invoke();

			if (ImGui::IsMouseDoubleClicked(0))
			{
				_doubleClickedEvent.Invoke();
			}
		}

		if (opened)
		{
			if (!prevOpened)
				_openedEvent.Invoke();

			_isOpened = true;

			ExecuteAllAddOns();

			DrawWidgets();

			ImGui::TreePop();
		}
		else
		{
			if (prevOpened)
				_closedEvent.Invoke();

			_isOpened = false;

			ExecuteAllAddOns();
		}
	}
}