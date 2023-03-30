#include "DUOLEditor/UI/Widgets/Layout/TreeNode.h"

namespace DUOLEditor
{
	TreeNode::TreeNode(const DUOLCommon::tstring& name, bool isEnableArrowClickToOpen) :
		DataWidgetBase<DUOLCommon::tstring>(&_name)
		, _name(name)
		, _isEnableArrowClickToOpen(isEnableArrowClickToOpen)
		, _isSearched(false)
	{
		_autoExecuteAddOns = false;
	}

	void TreeNode::Open()
	{
		_shouldOpen = true;

		_shouldClose = false;

		_isOpened = true;

		if (_prevOpened != _isOpened)
			_openedEvent.Invoke();
	}

	void TreeNode::Close()
	{
		_shouldClose = true;

		_shouldOpen = false;

		_isOpened = false;

		if (_prevOpened != _isOpened)
			_closedEvent.Invoke();
	}

	bool TreeNode::GetIsOpened()
	{
		return _isOpened;
	}

	void TreeNode::SetIsSearched(bool value)
	{
		_isSearched = value;
	}

	bool TreeNode::GetIsSearched() const
	{
		return _isSearched;
	}

	void* TreeNode::GetDragAndDropData() const
	{
		return _dragAndDropData;
	}

	void TreeNode::SetDragAndDropData(void* data)
	{
		_dragAndDropData = data;
	}

	const uint64_t& TreeNode::GetDragAndDropDataSize() const
	{
		return _dragAndDropDataSize;
	}

	void TreeNode::SetDragAndDropDataSize(const uint64_t& size)
	{
		_dragAndDropDataSize = size;
	}

	void TreeNode::Draw_Impl()
	{
		_prevOpened = _isOpened;

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

		// 가로 범위를 최대한 넓게 바꿔줍니다.
		flags |= ImGuiTreeNodeFlags_SpanFullWidth;

		if (_isEnableArrowClickToOpen) flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		if (_isSelected) flags |= ImGuiTreeNodeFlags_Selected;
		if (_isLeaf) flags |= ImGuiTreeNodeFlags_Leaf;

		bool opened;

		// 검색이 된 트리 노드이면 하이라이팅을 실시합니다.
		if (_isSearched)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, DUOLEditor::ImGuiHelper::ToImVec4(DUOLEditor::ImGuiHelper::SELECTED_COLOR));

			opened = ImGui::TreeNodeEx(DUOLCommon::StringHelper::ToString(_name + _tstringID).c_str(), flags);

			ImGui::PopStyleColor();
		}
		else 
			opened = ImGui::TreeNodeEx(DUOLCommon::StringHelper::ToString(_name + _tstringID).c_str(), flags);

		if (ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
		{
			_clickedEvent.Invoke();

			if (ImGui::IsMouseDoubleClicked(0))
			{
				_doubleClickedEvent.Invoke();
			}
		}

		// 여기다 어떤 함수 추가하면 드래그 앤 드랍 기능 할 수 있음..
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("TREE_NODE_DATA", _dragAndDropData, _dragAndDropDataSize);
			ImGui::Text(DUOLCommon::StringHelper::ToString(TEXT("Tree Node Data ...")).c_str());
			ImGui::EndDragDropSource();
		}

		if (opened)
		{
			if (!_prevOpened)
				_openedEvent.Invoke();

			_isOpened = true;

			ExecuteAllAddOns();

			DrawWidgets();

			ImGui::TreePop();
		}
		else
		{
			if (_prevOpened)
				_closedEvent.Invoke();

			_isOpened = false;


			ExecuteAllAddOns();
		}
	}
}