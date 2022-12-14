#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief display a input text field.
	 */
	class InputText : public DataWidgetBase<DUOLCommon::tstring>
	{
	public:
		InputText(const DUOLCommon::tstring& text = TEXT(""), const DUOLCommon::tstring& label = TEXT(""));

	protected:
		virtual void Draw_Impl() override;

	public:
		DUOLCommon::tstring _text;

		DUOLCommon::tstring _label;

		bool _selectAllOnClick;

		DUOLCommon::Event<void, DUOLCommon::tstring> _textChangedEvent;

		DUOLCommon::Event<void, DUOLCommon::tstring> _enterPressedEvent;
	};
}