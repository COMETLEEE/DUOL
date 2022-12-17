#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief widget that display a list of values that the user can select.
	 */
	class CheckBox : public DataWidgetBase<bool>
	{
	public:
		CheckBox(bool value = false, const DUOLCommon::tstring& label = TEXT(""));

	protected:
		virtual void Draw_Impl() override;

	public:
		bool _value;

		DUOLCommon::tstring _label;

		DUOLCommon::Event<void, bool> _valueChangedEvent;
	};
}