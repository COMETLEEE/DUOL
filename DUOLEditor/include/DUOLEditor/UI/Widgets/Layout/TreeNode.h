#pragma once
#include "DUOLCommon/Event/Event.h"

#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"
#include "DUOLEditor/UI/Widgets/WidgetGroupBase.h"

namespace DUOLEditor
{
	/**
	 * \brief Column 형식으로 여러 Widget 들을 그룹핑하고 접어 관리할 수 있도록 해주는 위젯
	 */
	class TreeNode : public DataWidgetBase<DUOLCommon::tstring>, public WidgetGroupBase
	{
	public:
		TreeNode(const DUOLCommon::tstring& name = TEXT(""), bool isEnableArrowClickToOpen = false);

		void Open();

		void Close();

		bool GetIsOpened();

	protected:
		virtual void Draw_Impl() override;

	public:
		DUOLCommon::tstring _name;

		bool _isSelected;

		bool _isLeaf;

		DUOLCommon::Event<void> _clickedEvent;

		DUOLCommon::Event<void> _doubleClickedEvent;

		DUOLCommon::Event<void> _openedEvent;

		DUOLCommon::Event<void> _closedEvent;

	private:
		bool _isEnableArrowClickToOpen;

		bool _shouldOpen;

		bool _shouldClose;

		bool _isOpened;
 	};
}