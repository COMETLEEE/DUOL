#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief Widget that can be added to a menu list.
	 */
	class MenuItem : public DataWidgetBase<bool>
	{
	public:
		MenuItem(const DUOLCommon::tstring& name, const DUOLCommon::tstring& shortcut = TEXT(""), bool checkable = false, bool checked = false);

	protected:
		virtual void Draw_Impl() override;

	public:
		DUOLCommon::tstring _name;

		DUOLCommon::tstring _shortcut;

		bool _checkable;

		bool _checked;

		DUOLCommon::Event<void> _clickedEvent;

		DUOLCommon::Event<void, bool> _valueChangedEvent;

	private:
		bool _isSelected;
	};
}