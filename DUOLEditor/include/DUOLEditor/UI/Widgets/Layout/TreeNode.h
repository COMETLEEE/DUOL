#pragma once
#include "DUOLCommon/Event/Event.h"

#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"
#include "DUOLEditor/UI/Widgets/WidgetGroupBase.h"

namespace DUOLEditor
{
	/**
	 * \brief Column �������� ���� Widget ���� �׷����ϰ� ���� ������ �� �ֵ��� ���ִ� ����
	 */
	class TreeNode : public DataWidgetBase<DUOLCommon::tstring>, public WidgetGroupBase
	{
	public:
		TreeNode(const DUOLCommon::tstring& name = TEXT(""), bool isEnableArrowClickToOpen = false);

		void Open();

		void Close();

		bool GetIsOpened();

		// TreeNode Search ��� �߰�
		void SetIsSearched(bool value);

		bool GetIsSearched() const;

		void* GetDragAndDropData() const;

		void SetDragAndDropData(void* data);

		const uint64_t& GetDragAndDropDataSize() const;

		void SetDragAndDropDataSize(const uint64_t& size);

	protected:
		virtual void Draw_Impl() override;

	public:
		DUOLCommon::tstring _name;

		bool _isSelected;

		bool _isLeaf;

		bool _isSearched;

		DUOLCommon::Event<void> _clickedEvent;

		DUOLCommon::Event<void> _clickUpEvent;

		DUOLCommon::Event<void> _doubleClickedEvent;

		DUOLCommon::Event<void> _openedEvent;

		DUOLCommon::Event<void> _closedEvent;

	private:
		bool _isEnableArrowClickToOpen;

		bool _shouldOpen;

		bool _shouldClose;

		bool _isOpened;

		bool _prevOpened;

		bool _prevMouseClick;

		void* _dragAndDropData;

		uint64_t _dragAndDropDataSize;
 	};
}